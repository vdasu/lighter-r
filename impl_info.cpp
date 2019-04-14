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

extern function_t start;
extern function_t arrival;
extern function_t unsorted_arrival;
extern function_t unsorted_function;
extern map<int, set<function_t> > f1_succ;
extern map<int, set<function_t> > f2_succ;
extern int count_list;
extern int max_impl;
extern int shortest_path;
extern string implementation_id;
extern string conf_file;

int cost_1;
int cost_2;

void update_unsorted_function(function_t f)
{
  auto get_diff_index = [](function_t f1, function_t f2)
  {
    int i = 0;
    int j = 0;
    bool found = false;
    while(i < N)
    {
      while(j < N)
      {
        if(f1.bit_slice[i] == f2.bit_slice[j])
        {
          found = true;
          break;
        }
        j++;
      }
      if(found == false) return i;
      found = false;
      i++;
      j = 0;
    }
    return -1;
  };

  int i = get_diff_index(f, unsorted_function);
  int j = get_diff_index(unsorted_function, f);

  unsorted_function.bit_slice[j] = f.bit_slice[i];

  unsorted_function.prev = f.prev;
  unsorted_function.info_line = f.info_line;
  unsorted_function.info_op = f.info_op;

  UINT_ mod = f.bit_slice[f.info_line];

  for(int i = 0; i < N; i++)
    if(unsorted_function.bit_slice[i] == mod) unsorted_function.info_line = i;
}

string get_permut(function_t f1, function_t f2, char a, char b)
{
  auto get_index = [](int n, function_t f1, function_t f2)
  {
    int i = 0;
    while( i < N )
    {
      if(f1.bit_slice[n] == f2.bit_slice[i]) return i;
      i++;
    }
    return -1;
  };

  string s;
  int i = 0;
  while( i < N )
  {
    s = s + a + "[" + to_string(i) + "] = ";
    s = s + b + "[" + to_string(get_index(i, f2, f1)) + "];\n";
    i++;
  }
  return s;
}

void get_string_impl(string *s, function_t f, bool reverse)
{
  if( !reverse ) s->insert(0, bool_op_to_string(f, f.info_op, reverse));
  else s->append(bool_op_to_string(f, f.info_op, reverse));
}

int get_semi_impl(function_t f,
                    int count_vlist,
                    map<int, set<function_t> > *g,
                    string *s,
                    bool reverse)
{
  int cost = bool_op_cost(f.info_op);
  auto found = g->at(0).find(f);
  unsorted_function = f;
  while(count_vlist >= 0)
  {
    update_unsorted_function(f);
    get_string_impl(s, unsorted_function, reverse);
    f.bit_slice[f.info_line] = f.prev;
    f.sort();
    count_vlist -= bool_op_cost(f.info_op);
    if( count_vlist == 0) break;
    found = g->at(count_vlist).find(f);
    f = *found;
    cost += bool_op_cost(f.info_op);
  }

  unsorted_function.bit_slice[unsorted_function.info_line] = unsorted_function.prev;

  if( !reverse )
  {
    s->insert(0, "\n" + get_permut(start, unsorted_function, 'F', 'X') + "\n");
    s->insert(0, "// from : " + start.to_string() + "\n");
  }
  else
  {
    s->append("\n" + get_permut(unsorted_function, unsorted_arrival, 'X', 'F') + "\n");
    s->append("//to : " + unsorted_arrival.to_string() + "\n");
  }
    return cost;
}

void get_implementation(function_t f1,
                      function_t f2,
                      int v,
                      map<int, set<function_t> > *g1,
                      map<int, set<function_t> > *g2)
{
  static int impl_number = 0;

  string s1, s2;
  if(g1 == &f1_succ)
  {
    cost_1 = get_semi_impl(f1, count_list+bool_op_cost(f1.info_op), g1, &s1, false);
    cost_2 = get_semi_impl(f2, v, g2, &s2, true);
  }
  else
  {
    cost_1 = get_semi_impl(f1, count_list+bool_op_cost(f1.info_op), g1, &s1, true);
    cost_2 = get_semi_impl(f2, v, g2, &s2, false);
  }
  unsorted_function.sort();
  if(cost_1+cost_2 < shortest_path)
  {
     cout << "Generating implementation " << impl_number << endl;
     shortest_path = cost_1+cost_2;
     int beg = conf_file.rfind("/");
     int end = conf_file.rfind(".");
     ofstream impl("implementation" + implementation_id + to_string(impl_number) + ".c");
     if(unsorted_function == start) impl << s2 << s1;
     else if(unsorted_function == arrival) impl << s1 << s2;
     impl << "// Cost : " << double(cost_1 + cost_2)/100 << endl;
     impl << "// Logic Library : " << conf_file.substr(beg+1, end-beg-1) << endl;
     cout << "(cost : " << cost_1 << " + " << cost_2 << " -> " << (double)(cost_1 + cost_2)/100 << "GE)" << endl;
     impl.close();
     impl_number ++;
 }
}
