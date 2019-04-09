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

#ifndef MITM_H
#define MITM_H

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <vector>
#include <inttypes.h>
#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include <set>
#include <cmath>
#include "omp.h"
#include <limits>
#include <sstream>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "static_sort.h"
#pragma GCC diagnostic pop

#define N 4
#define UINT_ uint16_t

#define NOT1 1

#define XOR2  2
#define XNOR2 3

#define MAOI1 4
#define MOAI1 5

#define MOAI1_NAND2 6
#define MOAI1_NOR2  7
#define MOAI1_AND2  8
#define MOAI1_OR2   9

#define MAOI1_NAND2 10
#define MAOI1_NOR2  11
#define MAOI1_AND2  12
#define MAOI1_OR2   13

#define MOAI1_NAND3      14
#define MAOI1_AND3       15
#define MOAI1_NAND2_AND2 16
#define MAOI1_AND2_AND2  17
#define MOAI1_NOR3       18
#define MAOI1_OR3        19
#define MOAI1_NOR2_OR2   20
#define MAOI1_OR2_OR2    21
#define MAOI1_NAND3      22
#define MOAI1_AND3       23
#define MAOI1_NAND2_AND2 24
#define MOAI1_AND2_AND2  25
#define MAOI1_NOR3       26
#define MOAI1_OR3        27
#define MAOI1_NOR2_OR2   28
#define MOAI1_OR2_OR2    29

#define MOAI1_AND2_NAND2  30
#define MOAI1_OR2_NOR2    31
#define MAOI1_NAND2_NAND2 32
#define MAOI1_NOR2_NOR2   33

#define MOAI1_NAND2_NAND2 34
#define MOAI1_NOR2_NOR2   35
#define MAOI1_AND2_NAND2  36
#define MAOI1_OR2_NOR2    37

#define MOAI1_NAND2_OR2   38
#define MOAI1_NOR2_AND2   39
#define MAOI1_AND2_OR2    40
#define MAOI1_OR2_AND2    41

#define MOAI1_NAND2_NOR2  42
#define MOAI1_NOR2_NAND2  43
#define MAOI1_AND2_NOR2   44
#define MAOI1_OR2_NAND2   45

#define MOAI1_AND2_OR2    46
#define MOAI1_OR2_AND2    47
#define MAOI1_NAND2_OR2   48
#define MAOI1_NOR2_AND2   49

#define MOAI1_AND2_NOR2   50
#define MOAI1_OR2_NAND2   51
#define MAOI1_NAND2_NOR2  52
#define MAOI1_NOR2_NAND2  53

/*
  Software boolean operations
*/
#define XOR2_AND2        58
#define XOR2_OR2         59
#define XOR2_ANDN2       60
#define XOR2_ORN2        61

#define XOR2_AND2_AND2   62
#define XOR2_AND2_OR2    63
#define XOR2_AND2_ANDN2  64
#define XOR2_AND2_ORN2   65

#define XOR2_OR2_AND2    66
#define XOR2_OR2_OR2     67
#define XOR2_OR2_ANDN2   68
#define XOR2_OR2_ORN2    69

#define XOR2_ANDN2_AND2  70
#define XOR2_ANDN2_OR2   71
#define XOR2_ANDN2_ANDN2 72
#define XOR2_ANDN2_ORN2  73

#define XOR2_ORN2_AND2   74
#define XOR2_ORN2_OR2    75
#define XOR2_ORN2_ANDN2  76
#define XOR2_ORN2_ORN2   77

#define RNOT1      78
#define CNOT1      79
#define CCNOT2     80
#define FREDKIN2_1 81
#define FREDKIN2_2 82


using namespace std;

void print_uint32(uint32_t n);
void print_uint16(uint16_t n);
void print_uint8(uint8_t n);
void swap(uint16_t *a, uint16_t *b);
string uint8_to_string(uint8_t n);

struct function_t
{
  /*
    the bit-sliced representation of the function
  */
  array<UINT_, N> bit_slice{ {} };

  /*
    the line which was in the previous function
  */
  UINT_ prev = 0;

  /*
    index of changed line
  */
  uint8_t info_line = 0;
  /*
    identifier for last boolean operation
  */
  uint8_t info_op = 0;

  function_t(){};

  function_t(array<UINT_, N> a)
  {
    for (size_t i = 0; i < N; i++)
      bit_slice[i] = a[i];
  }

  bool operator < (const function_t& f) const
  {
    for (int i = 0; i < N; i++)
    {
      if(bit_slice[i] != f.bit_slice[i])
        return bit_slice[i] < f.bit_slice[i];
    }
    return bit_slice[N-1] < f.bit_slice[N-1];
  }

  bool operator == (const function_t& f) const
  {
    bool tmp = true;
    for (int i = 0; i < N; i++)
    {
      tmp = tmp &&  bit_slice[i] == f.bit_slice[i];
    }
    return tmp;
  }

  void print() const
  {
    cout << "------>\n";
    for (int i = 0; i < N; i++)
    {
      printf("%d %04X\n",i , bit_slice[i]);
    }
    printf("%04X\n", prev);
    print_uint8(info_line);
    print_uint8(info_op);
  }

  string to_string() const
  {
    string str;
    char tmp[6];
    for(int i = 0; i < N; i++)
    {
      sprintf(tmp, "%04X ", bit_slice[i]);
      str.append(tmp);
    }
    return str;
  }

  /*
    Sort the lines of the function matrix
    Ascending order
    Ex :
          0101010101010101
          0011001100110011
          0000111100001111
          0000000000000000
  */
  void sort()
  {
    UINT_ mod = bit_slice[info_line];
    StaticSort<N> staticSort;
    staticSort(bit_slice);
    for(int i = 0; i < N; i++)
    {
      if(bit_slice[i] == mod) info_line = i;
    }
  }

  bool is_permutation() const
  {
    int i = 16;
    uint8_t tmp = 0x0;
    std::array<UINT_, N> copy = bit_slice;
    int verif[16] = {0};
    while(i != 0)
    {
      for(int j = 0; j < N; j++)
      {
        if((copy[j] & 0x8000) != 0) tmp = tmp|(0x1<<j);
        copy[j] <<= 1;
      }
      i--;
      verif[(int)tmp] = 1;
      tmp = 0x0;
    }
    i = 0;
    while(i < 16)
    {
      if( verif[i] == 0 ) return false;
      i++;
    }
    return true;
  }
};

string bit_slice(array<uint16_t, 4> *sbox);

struct bool_op_t
{
  int op_id;
  int op_cost;

  bool operator < (const bool_op_t& b) const
  {
    return op_cost < b.op_cost;
  }

  bool operator == (const bool_op_t& b) const
  {
    return op_cost == b.op_cost;
  }

};
int bool_op_cost(uint8_t op);

/*
  mitm initializes the graph, then calls expand function on both sides of the graph.
*/
void mitm(function_t f1, function_t f2, int lambda);

/*
  expand adds the succesors of the nodes int the lambda set to the graphe
*/
void expand(map<int, set<function_t> > *g_curr,
            map<int, set<function_t> > *g_opp,
            int lambda);

/*
  v_list_process takes the tmp vector filled by the bool_op_* functions.
  It checks each element of this vector :
    . If an element is not in current graphe it adds it
    . If an element is in opposite graphe it raises a collision.
  It fills the set to_insert
*/
void v_list_process(int lambda,int op_cost, vector<function_t> *tmp,    map<int, set<function_t> > *current,
                    set<function_t> *to_insert, map<int,set<function_t> > *opposite);

bool is_in_graphe(int lambda, int op_cost,function_t f, map<int, set<function_t> > *g);
bool is_in_graphe_collision(int lambda,int op_cost, function_t f,
                            function_t *fun_found,
                            int *vect_found,
                            map<int, set<function_t> > *g);

void get_implementation(function_t f1,
                        function_t f2,
                        int v,
                        map<int, set<function_t> > *g1,
                        map<int, set<function_t> > *g2);
/*
  The bool_op_fun function put in the vector v the successors of the
  function f by the boolean operation op_id.

  bool_op_to_string return the C implementation
  of the edge arriving to the node. ie, the boolean operation which
  change the previous matrix to the function_t f matrix.
  the string inv_bool_op gives the reverse boolean operation.
*/
void bool_op_fun( int op_id, function_t f, vector<function_t> *v);
string bool_op_to_string(const function_t f, uint8_t op, bool reverse);

string uint8_to_string(uint8_t n);
string bit_slice(array<uint16_t, 4> *sbox);
void exit_m();
void print_graphe_info();
void graphe_to_file(map<int, set<function_t> > *graphe, string graphe_name);

#endif /* MITM_H */
