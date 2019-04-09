/**
 * Optimizing Implementations of Lightweight Building Blocks
 *
 *  Copyright 2017 by
 *    Jade Tourteaux <Jade[dot]Tourteaux[at]gmail.com>
 *    Jérémy Jean <Jean[dot]Jeremy[at]gmail.com>
 *
 * This file is part of some open source application.
 * 
 * Some open source application is free software: you can redistribute 
 * it and/or modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 * 
 * Some open source application is distributed in the hope that it will 
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#include "mitm.h"

extern vector<bool_op_t> b;
extern function_t start;
extern function_t arrival;
extern function_t unsorted_arrival;
extern map<int, set<function_t> > f1_succ;
extern map<int, set<function_t> > f2_succ;
extern int count_list;
extern bool verbose;
extern bool one_expand;
extern int omp_nb_threads;
extern int shortest_path;
extern int max_ram;

long nodes_cmp = 0;
long max_nodes;

void mitm(function_t f1, function_t f2, int l)
{
  /*
    initilisation of the two graphs
    we start by adding the function f and the identity
  */
  unsorted_arrival = f2;
  f1.sort();
  f2.sort();
  f1_succ.insert(pair<int, set<function_t>>(0, {f1}));
  f2_succ.insert(pair<int, set<function_t>>(0, {f2}));
  start = f1;
  arrival = f2;
  max_nodes = max_ram * 100000000 / sizeof(function_t);

  /*
    we expand the two graphs
  */
  count_list = 0;
  while(count_list <= l)
  {
    expand(&f1_succ, &f2_succ, count_list);
    if(!one_expand) expand(&f2_succ, &f1_succ, count_list);
    if(verbose && (count_list%100) == 0) cout << "\033[0;32m" << count_list << "\033[0m" << endl;
    count_list++;
    if(2*count_list > shortest_path) exit_m();
  }
  if(verbose) print_graphe_info();
  exit_m();
}


void expand(map<int, set<function_t> > *current,
            map<int, set<function_t> > *opposite,
            int lambda)
{
  auto to_expand = current->find(lambda);
  if( to_expand != current->end())
  {
    for(auto bool_op : ::b)
    {
      int bool_op_cost = bool_op.op_cost;
      vector<function_t> successors;

      for(auto fun : (*to_expand).second)
        bool_op_fun(bool_op.op_id, fun, &successors);

      if(successors.size() != 0)
      {
        auto s = current->find(lambda+bool_op.op_cost);
        set<function_t> new_list;
        if(s != current->end())
        {
          v_list_process(lambda, bool_op_cost, &successors, current, &new_list, opposite);
          s->second.insert(new_list.begin(), new_list.end());
        }
        else
        {
           v_list_process(lambda, bool_op_cost, &successors, current, &new_list, opposite);
           current->insert(pair<int,set<function_t>>(lambda+bool_op.op_cost, new_list));
        }
      }
    }
  }
}



void v_list_process(int lambda, int op_cost, vector<function_t> *tmp,
                    map<int, set<function_t> > *current,
                    set<function_t> *to_insert, map<int,set<function_t> > *opposite)
{
  function_t fun_found;
  int vect_found;

  sort(tmp->begin(), tmp->end());
  tmp->erase(unique(tmp->begin(), tmp->end()), tmp->end());

  #pragma omp parallel for private(fun_found, vect_found) num_threads(omp_nb_threads)
  for(auto it_tmp = tmp->begin(); it_tmp < tmp->end(); it_tmp++)
  {
    if(!is_in_graphe(lambda, op_cost, (*it_tmp), current))
    {
      #pragma omp critical
      {
        to_insert->insert(*it_tmp);
        nodes_cmp++;
        if(nodes_cmp == max_nodes) exit_m();
      }
      if(is_in_graphe_collision(lambda, op_cost, (*it_tmp), &fun_found, &vect_found, opposite))
      {
        #pragma omp critical
        {
          //system("notify-send 'Collision'");
          get_implementation((*it_tmp), fun_found, vect_found, current, opposite);
        }
      }
    }
  }
}

bool is_in_graphe(int lambda, int op_cost, function_t f, map<int, set<function_t> > *g)
{
  for(auto it = g->begin(); it != g->end() ; it++)
  {
    if((*it).second.find(f) != (*it).second.end())
    {
      if((*it).first > lambda+op_cost)
      {
        #pragma omp critical
        {
          (*it).second.erase(f);
        }
        return false;
      }
      return true;
    }
  }
  return false;
}


bool is_in_graphe_collision(int lambda, int op_cost, function_t f,
                            function_t *fun_found,
                            int *vect_found,
                            map<int, set<function_t> > *g)
{
  for(auto it = g->begin(); it != g->end(); it++)
  {
    //cout << lambda << " " << (*it).first << " "<< endl;
    if( lambda + (*it).first + op_cost >= shortest_path ) return false;
    if((*it).second.find(f) != (*it).second.end())
    {
      *vect_found = (*it).first;
      *fun_found = *((*it).second.find(f));
      return true;
    }
  }
  return false;
}
