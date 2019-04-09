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

extern map<int, set<function_t> > f1_succ;
extern map<int, set<function_t> > f2_succ;
extern bool write_in_file;

void print_graphe_info()
{
  int tot_f1 = 0; int tot_f2 = 0;

  cout << "\n\033[0;31mf1_succ : \033[0m";

  auto it = f1_succ.begin();

  while (it != f1_succ.end())
  {
    cout << (*it).first << " (" << (*it).second.size() << ") ";
    tot_f1 += ((*it).second.size());
    it++;
  }
  cout << "\n\033[1;33mTotal = " << tot_f1;
  cout << "\n\033[0;31mf2_succ : \033[0m";
  it = f2_succ.begin();
  while (it != f2_succ.end())
  {
    cout << (*it).first << " (" << (*it).second.size() << ") ";
    tot_f2 += ((*it).second.size());
    it++;
  }
  cout << "\n\033[1;33mTotal = " << tot_f2 << "\033[0m\n";
}

void exit_m()
{
  if(write_in_file)
  {
    graphe_to_file(&f1_succ, "f1");
    graphe_to_file(&f2_succ, "f2");
  }
  system("notify-send 'EXIT'");
  exit(0);
}

void graphe_to_file(map<int, set<function_t> > *graphe,
                    string graphe_name)
{
  string name;
  int tot_number = 0;
  /*
    Generating file_info
  */
  ofstream file_info(graphe_name + "_infos.txt");
  file_info << "Number of lists in graph : "
            << graphe->size()
            << endl;
  map<int, set<function_t> >::iterator it_graphe = graphe->begin();
  while(it_graphe != graphe->end())
  {
    if((*it_graphe).second.size() != 0)
    {
      tot_number += (*it_graphe).second.size();
      file_info << "Number of nodes in list "
              << (*it_graphe).first
              << " : "
              <<  (*it_graphe).second.size()
              << endl;
    }
    if((*it_graphe).second.size() != 0)
    {
      name = graphe_name
            + string("_list_")
            + to_string((*it_graphe).first)
            + string(".txt");

      ofstream file_list(name);
      /*
        Generating *_list_n.txt
      */
      if(file_list.is_open())
      {
        set<function_t>::iterator it_curr = (*it_graphe).second.begin();
        while(it_curr != (*it_graphe).second.end())
        {
          file_list << (*it_curr).to_string()<<endl;
          it_curr++;
        }
      }
      file_list.close();
    }
    it_graphe++;
  }
  file_info << "Total number of nodes in graphe : "
            << to_string(tot_number)
            << endl;
  file_info.close();
}

string bit_slice(array<uint16_t, 4> *sbox)
{
  stringstream ss;
  uint16_t tmp = 0x0;
  auto it = sbox->begin();
  array<uint16_t, 4> s1, s2, s3, s4;
  int cmpt = 0;
  while (it != sbox->end())
  {
    s1[cmpt] = *it&0xf000;
    s2[cmpt] = (*it&0x0f00) << 4;
    s3[cmpt] = (*it&0x00f0) << 8;
    s4[cmpt] = (*it&0x000f) << 12;
    it++;
    cmpt++;
  }

  cmpt = 0;
  uint16_t mask = 0x8000;
  for (size_t i = 0; i < 4; i++)
  {
    while (cmpt < 4)
    {
      tmp = (s1[cmpt])&mask;
      tmp = tmp | ((s2[cmpt]&mask)>>1);
      tmp = tmp | ((s3[cmpt]&mask)>>2);
      tmp = tmp | ((s4[cmpt]&mask)>>3);
      tmp <<=  i;
      tmp >>= 12;
      ss << hex << tmp;
      cmpt ++;
    }
    cmpt = 0;
    ss << " ";
    mask >>= 1;
  }
  string res = ss.str();
  res.pop_back();
  return res;
}

void print_uint16(uint16_t n){
  int i = 16;
  while(i != 0)
  {
    printf("%c", (n & 0x8000) == 0 ? '0' : '1');
    n <<= 1;
    i--;
  }
  printf("\n");
}

void print_uint8(uint8_t n){
  int i = 8;
  while(i != 0)
  {
    printf("%c", (n & 0x80) == 0 ? '0' : '1');
    n <<= 1;
    i--;
  }
  printf("\n");
}

string uint8_to_string(uint8_t n)
{
  string s;
  int i = 8;
  while(i != 0)
  {
    (n & 0x80) == 0 ? s+='0' : s+='1';
    n <<= 1;
    i--;
  }
  return s;
}

void print_uint32(uint32_t n){
  int i = 32;
  while(i != 0)
  {
    printf("%c", (n & 0x80000000) == 0 ? '0' : '1');
    n <<= 1;
    i--;
  }
  printf("\n");
}
