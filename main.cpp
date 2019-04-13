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

vector<bool_op_t> b; // The boolean instructions (B in the paper)
function_t start;
function_t arrival;
function_t unsorted_arrival;
function_t unsorted_function;
map<int, set<function_t> > f1_succ;
map<int, set<function_t> > f2_succ;
int count_list;
int shortest_path = numeric_limits<int>::max();
string implementation_id = "_";

int max_GE = 0;
bool verbose = false;
int l = 1200;
bool one_expand = false;
bool write_in_file = false;
int omp_nb_threads = 20;
string conf_file = "UMC180nm.conf";
int max_ram = 30;

int gate_not1  = false;
int gate_and2  = false;
int gate_nand2 = false;
int gate_or2   = false;
int gate_nor2  = false;
int gate_xor2  = false;
int gate_xnor2 = false;
int gate_moai1 = false;
int gate_maoi1 = false;
int gate_nand3 = false;
int gate_nor3  = false;
int gate_or3   = false;
int gate_and3  = false;
int gate_andn2  = false;
int gate_orn2   = false;

int not1_cost;
int and2_cost;
int nand2_cost;
int or2_cost;
int nor2_cost;
int xor2_cost;
int xnor2_cost;
int maoi1_cost;
int moai1_cost;
int nand3_cost;
int nor3_cost;
int and3_cost;
int or3_cost;
int andn2_cost;
int orn2_cost;

int gate_rnot1 = false;
int gate_cnot1 = false;
int gate_ccnot2 = false;
int gate_cccnot2 = false;
int gate_fredkin2_1 = false;
int gate_fredkin2_2 = false;
int gate_fredkin2_41 = false;
int gate_fredkin2_42 = false;

int rnot1_cost;
int cnot1_cost;
int ccnot2_cost;
int cccnot2_cost;
int fredkin2_1_cost;
int fredkin2_2_cost;
int fredkin2_41_cost;
int fredkin2_42_cost;


function_t parse_function(string str)
{
  if(str.size() > 0)
  {
    if(str.size() == 19)
    {
      stringstream ss(str);
      string number;
      function_t res;
      int i = -1;
      while(getline(ss, number, ' ') && ++i < N) res.bit_slice[i] = stoul(number, nullptr, 16);
      return res;
    }
    if(str.size() == 16)
    {
      uint16_t x1, x2, x3, x4;
      string tmp = str.substr(0, 4);
      x1 = (uint16_t) stoul(tmp, nullptr, 16);
      tmp = str.substr(4, 4);
      x2 = (uint16_t) stoul(tmp, nullptr, 16);
      tmp = str.substr(8, 4);
      x3 = (uint16_t) stoul(tmp, nullptr, 16);
      tmp = str.substr(12, 4);
      x4 = (uint16_t) stoul(tmp, nullptr, 16);
      array<uint16_t, 4> s = {x1, x2, x3, x4};
      return parse_function(bit_slice(&s));
    }
    return function_t({0x00ff, 0x0f0f, 0x3333, 0x5555});
  }
  else return function_t({0x00ff, 0x0f0f, 0x3333, 0x5555});
}

void all_gates()
{
  gate_not1  = true;
  gate_and2  = true;
  gate_nand2 = true;
  gate_or2   = true;
  gate_nor2  = true;
  gate_xor2  = true;
  gate_xnor2 = true;
  gate_moai1 = true;
  gate_maoi1 = true;
  gate_nand3 = true;
  gate_nor3  = true;
  gate_or3   = true;
  gate_and3  = true;
  gate_andn2  = true;
  gate_orn2   = true;
}

void reversible_gates()
{
  gate_rnot1 = true;
  gate_cnot1 = true;
  gate_ccnot2 = true;
  gate_cccnot2 = true;
  gate_fredkin2_1 = true;
  gate_fredkin2_2 = true;
  gate_fredkin2_41 = true;
  gate_fredkin2_42 = true;
}


int bool_op_cost(uint8_t op)
{
  switch (op)
  {
    case NOT1              : return not1_cost;
    case RNOT1             : return rnot1_cost;
    case CNOT1             : return cnot1_cost;
    case CCNOT2            : return ccnot2_cost;
    case CCCNOT2           : return cccnot2_cost;
    case FREDKIN2_1        : return fredkin2_1_cost;
    case FREDKIN2_2        : return fredkin2_2_cost;
    case FREDKIN2_41       : return fredkin2_41_cost;
    case FREDKIN2_42       : return fredkin2_42_cost;
    
    case XOR2              : return xor2_cost;
    case XNOR2             : return xnor2_cost;
    case MAOI1             : return maoi1_cost;
    case MOAI1             : return moai1_cost;
    case MOAI1_NAND2       : return moai1_cost + nand2_cost;
    case MOAI1_NOR2        : return moai1_cost + nor2_cost;
    case MOAI1_AND2        : return moai1_cost + and2_cost;
    case MOAI1_OR2         : return moai1_cost + or2_cost;
    case MOAI1_NAND3       : return moai1_cost + nand3_cost;
    case MOAI1_NOR3        : return moai1_cost + nor3_cost;
    case MAOI1_NAND3       : return maoi1_cost + nand3_cost;
    case MAOI1_NOR3        : return maoi1_cost + nor3_cost;
    case MOAI1_AND2_NAND2  : return moai1_cost + and2_cost + nand2_cost;
    case MOAI1_OR2_NOR2    : return moai1_cost + or2_cost + nor2_cost;
    case MOAI1_NAND2_NAND2 : return moai1_cost + 2*nand2_cost;
    case MOAI1_NOR2_NOR2   : return moai1_cost + 2*nor2_cost;
    case MOAI1_NAND2_OR2   : return moai1_cost + nand2_cost + or2_cost;
    case MOAI1_NOR2_AND2   : return moai1_cost + nor2_cost + and2_cost;
    case MOAI1_NAND2_NOR2  : return moai1_cost + nand2_cost + nor2_cost;
    case MOAI1_NOR2_NAND2  : return moai1_cost + nor2_cost + nand2_cost;
    case MOAI1_AND2_OR2    : return moai1_cost + and2_cost + or2_cost;
    case MOAI1_OR2_AND2    : return moai1_cost + or2_cost + and2_cost;
    case MOAI1_AND2_NOR2   : return moai1_cost + and2_cost + nor2_cost;
    case MOAI1_OR2_NAND2   : return moai1_cost + or2_cost + nand2_cost;
    case MAOI1_NAND2_NAND2 : return maoi1_cost + nand2_cost + nand2_cost;
    case MAOI1_NOR2_NOR2   : return maoi1_cost + nor2_cost + nor2_cost;
    case MAOI1_AND2_NAND2  : return maoi1_cost + and2_cost + nand2_cost;
    case MAOI1_OR2_NOR2    : return maoi1_cost + or2_cost + nor2_cost;
    case MAOI1_AND2_OR2    : return maoi1_cost + and2_cost + or2_cost;
    case MAOI1_OR2_AND2    : return maoi1_cost + or2_cost + and2_cost;
    case MAOI1_AND2_NOR2   : return maoi1_cost + and2_cost + nor2_cost;
    case MAOI1_OR2_NAND2   : return maoi1_cost + or2_cost + nand2_cost;
    case MAOI1_NAND2_OR2   : return maoi1_cost + nand2_cost + or2_cost;
    case MAOI1_NOR2_AND2   : return maoi1_cost + nor2_cost + and2_cost;
    case MAOI1_NAND2_NOR2  : return maoi1_cost + nand2_cost + nor2_cost;
    case MAOI1_NOR2_NAND2  : return maoi1_cost + nor2_cost + nand2_cost;
    case MAOI1_NAND2       : return maoi1_cost + nand2_cost;
    case MAOI1_NOR2        : return maoi1_cost + nor2_cost;
    case MAOI1_AND2        : return maoi1_cost + and2_cost;
    case MAOI1_OR2         : return maoi1_cost + or2_cost;
    case MAOI1_AND3        : return maoi1_cost + and3_cost;
    case MOAI1_NAND2_AND2  : return moai1_cost + nand2_cost + and2_cost;
    case MAOI1_AND2_AND2   : return maoi1_cost + and2_cost + and2_cost;
    case MAOI1_OR3         : return maoi1_cost + or3_cost;
    case MOAI1_NOR2_OR2    : return moai1_cost + nor2_cost + or2_cost;
    case MAOI1_OR2_OR2     : return maoi1_cost + or2_cost + or2_cost;
    case MOAI1_AND3        : return moai1_cost + and3_cost;
    case MAOI1_NAND2_AND2  : return maoi1_cost + nand2_cost + and2_cost;
    case MOAI1_AND2_AND2   : return moai1_cost + and2_cost + and2_cost;
    case MOAI1_OR3         : return moai1_cost + or3_cost;
    case MAOI1_NOR2_OR2    : return maoi1_cost + nor2_cost + or2_cost;
    case MOAI1_OR2_OR2     : return moai1_cost + or2_cost + or2_cost;

    case XOR2_AND2         : return xor2_cost + and2_cost;
    case XOR2_OR2          : return xor2_cost + or2_cost;
    case XOR2_ANDN2        : return xor2_cost + andn2_cost;
    case XOR2_ORN2         : return xor2_cost + orn2_cost;

    case XOR2_AND2_AND2    : return xor2_cost + 2*and2_cost;
    case XOR2_AND2_OR2     : return xor2_cost + and2_cost + or2_cost;
    case XOR2_AND2_ANDN2   : return xor2_cost + and2_cost + andn2_cost;
    case XOR2_AND2_ORN2    : return xor2_cost + and2_cost + orn2_cost;

    case XOR2_OR2_AND2     : return xor2_cost + and2_cost + or2_cost;
    case XOR2_OR2_OR2      : return xor2_cost + 2*or2_cost;
    case XOR2_OR2_ANDN2    : return xor2_cost + andn2_cost + or2_cost;
    case XOR2_OR2_ORN2     : return xor2_cost + or2_cost + orn2_cost;

    case XOR2_ANDN2_AND2    : return xor2_cost + andn2_cost + and2_cost;
    case XOR2_ANDN2_OR2     : return xor2_cost + andn2_cost + or2_cost;
    case XOR2_ANDN2_ANDN2   : return xor2_cost + 2*andn2_cost;
    case XOR2_ANDN2_ORN2    : return xor2_cost + andn2_cost + orn2_cost;

    case XOR2_ORN2_AND2     : return xor2_cost + and2_cost + orn2_cost;
    case XOR2_ORN2_OR2      : return xor2_cost + orn2_cost + or2_cost;
    case XOR2_ORN2_ANDN2    : return xor2_cost + andn2_cost + orn2_cost;
    case XOR2_ORN2_ORN2     : return xor2_cost + 2*orn2_cost;

    default                : return -1;
  }
}

void init_b(string conf_file)
{
  ifstream input(conf_file);
  string key, value;
  for(string line; getline( input, line ); )
  {
    istringstream is_line(line);
    if(getline(is_line, key, '='))
    {
      if(getline(is_line, value))
      {
        if(key == "not1_cost") not1_cost = stod(value)*100;
        if(key == "and2_cost") and2_cost = stod(value)*100;
        if(key == "nand2_cost") nand2_cost = stod(value)*100;
        if(key == "or2_cost") or2_cost = stod(value)*100;
        if(key == "nor2_cost") nor2_cost = stod(value)*100;
        if(key == "xor2_cost") xor2_cost = stod(value)*100;
        if(key == "xnor2_cost") xnor2_cost = stod(value)*100;
        if(key == "maoi1_cost") maoi1_cost = stod(value)*100;
        if(key == "moai1_cost") moai1_cost = stod(value)*100;
        if(key == "nand3_cost") nand3_cost = stod(value)*100;
        if(key == "nor3_cost") nor3_cost = stod(value)*100;
        if(key == "and3_cost") and3_cost = stod(value)*100;
        if(key == "or3_cost") or3_cost = stod(value)*100;
        if(key == "andn2_cost") andn2_cost = stod(value)*100;
        if(key == "orn2_cost") orn2_cost = stod(value)*100;

        //Reversible gates
        if(key == "rnot1_cost") rnot1_cost = stod(value)*100;
        if(key == "cnot1_cost") cnot1_cost = stod(value)*100;
        if(key == "ccnot2_cost") ccnot2_cost = stod(value)*100;
        if(key == "fredkin2_1_cost") fredkin2_1_cost = stod(value)*100;
        if(key == "fredkin2_2_cost") fredkin2_2_cost = stod(value)*100;
      }
    }
  }

  /*
    initilisation of the set of bolean operations
  */
  if(gate_not1) b.push_back({NOT1, bool_op_cost(NOT1)});
  if(gate_xor2)
  {
    b.push_back({XOR2, bool_op_cost(XOR2)});
    if(gate_and2)
    {
      b.push_back({XOR2_AND2, bool_op_cost(XOR2_AND2)});
      b.push_back({XOR2_AND2_AND2, bool_op_cost(XOR2_AND2_AND2)});
      if(gate_or2) b.push_back({XOR2_AND2_OR2, bool_op_cost(XOR2_AND2_OR2)});
      if(gate_andn2) b.push_back({XOR2_AND2_ANDN2, bool_op_cost(XOR2_AND2_ANDN2)});
      if(gate_orn2) b.push_back({XOR2_AND2_ORN2, bool_op_cost(XOR2_AND2_ORN2)});
    }
    if(gate_or2)
    {
      b.push_back({XOR2_OR2, bool_op_cost(XOR2_OR2)});
      b.push_back({XOR2_OR2_OR2, bool_op_cost(XOR2_OR2_OR2)});
      if(gate_and2) b.push_back({XOR2_OR2_AND2, bool_op_cost(XOR2_OR2_AND2)});
      if(gate_andn2) b.push_back({XOR2_OR2_ANDN2, bool_op_cost(XOR2_OR2_ANDN2)});
      if(gate_orn2) b.push_back({XOR2_OR2_ORN2, bool_op_cost(XOR2_OR2_ORN2)});
    }
    if(gate_andn2)
    {
      b.push_back({XOR2_ANDN2, bool_op_cost(XOR2_ANDN2)});
      b.push_back({XOR2_ANDN2_ANDN2, bool_op_cost(XOR2_ANDN2_ANDN2)});
      if(gate_or2) b.push_back({XOR2_ANDN2_OR2, bool_op_cost(XOR2_ANDN2_OR2)});
      if(gate_and2) b.push_back({XOR2_ANDN2_AND2, bool_op_cost(XOR2_ANDN2_AND2)});
      if(gate_orn2) b.push_back({XOR2_ANDN2_ORN2, bool_op_cost(XOR2_ANDN2_ORN2)});
    }
    if(gate_orn2)
    {
      b.push_back({XOR2_ORN2, bool_op_cost(XOR2_ORN2)});
      b.push_back({XOR2_ORN2_ORN2, bool_op_cost(XOR2_ORN2_ORN2)});
      if(gate_and2) b.push_back({XOR2_ORN2_AND2, bool_op_cost(XOR2_ORN2_AND2)});
      if(gate_andn2) b.push_back({XOR2_ORN2_ANDN2, bool_op_cost(XOR2_ORN2_ANDN2)});
      if(gate_or2) b.push_back({XOR2_ORN2_OR2, bool_op_cost(XOR2_ORN2_OR2)});
    }
  }
  if(gate_xnor2) b.push_back({XNOR2, bool_op_cost(XNOR2)});
  if(gate_maoi1)
  {
    if(gate_and3) b.push_back({MAOI1_AND3, bool_op_cost(MAOI1_AND3)});
    if(gate_or3) b.push_back({MAOI1_OR3, bool_op_cost(MAOI1_OR3)});
    if(gate_nor3) b.push_back({MAOI1_NOR3, bool_op_cost(MAOI1_NOR3)});
    if(gate_nand3) b.push_back({MAOI1_NAND3, bool_op_cost(MAOI1_NAND3)});
    b.push_back({MAOI1, bool_op_cost(MAOI1)});
    if(gate_nand2)
    {
      b.push_back({MAOI1_NAND2, bool_op_cost(MAOI1_NAND2)});
      b.push_back({MAOI1_NAND2_NAND2, bool_op_cost(MAOI1_NAND2_NAND2)});
      if(gate_nor2) b.push_back({MAOI1_NAND2_NOR2, bool_op_cost(MAOI1_NAND2_NOR2)});
      if(gate_and2) b.push_back({MAOI1_NAND2_AND2, bool_op_cost(MAOI1_NAND2_AND2)});
      if(gate_or2) b.push_back({MAOI1_NAND2_OR2, bool_op_cost(MAOI1_NAND2_OR2)});
    }
    if(gate_nor2)
    {
      b.push_back({MAOI1_NOR2, bool_op_cost(MAOI1_NOR2)});
      b.push_back({MAOI1_NOR2_NOR2, bool_op_cost(MAOI1_NOR2_NOR2)});
      if(gate_and2) b.push_back({MAOI1_NOR2_AND2, bool_op_cost(MAOI1_NOR2_AND2)});
      if(gate_or2) b.push_back({MAOI1_NOR2_OR2, bool_op_cost(MAOI1_NOR2_OR2)});
      if(gate_nand2) b.push_back({MAOI1_NOR2_NAND2, bool_op_cost(MAOI1_NOR2_NAND2)});
    }
    if(gate_or2)
    {
      b.push_back({MAOI1_OR2_OR2, bool_op_cost(MAOI1_OR2_OR2)});
      b.push_back({MAOI1_OR2, bool_op_cost(MAOI1_OR2)});
      if(gate_and2) b.push_back({MAOI1_OR2_AND2, bool_op_cost(MAOI1_OR2_AND2)});
      if(gate_nor2) b.push_back({MAOI1_OR2_NOR2, bool_op_cost(MAOI1_OR2_NOR2)});
      if(gate_nand2) b.push_back({MAOI1_OR2_NAND2, bool_op_cost(MAOI1_OR2_NAND2)});
    }
    if(gate_and2)
    {
      b.push_back({MAOI1_AND2, bool_op_cost(MAOI1_AND2)});
      b.push_back({MAOI1_AND2_AND2, bool_op_cost(MAOI1_AND2_AND2)});
      if(gate_nand2) b.push_back({MAOI1_AND2_NAND2, bool_op_cost(MAOI1_AND2_NAND2)});
      if(gate_nor2) b.push_back({MAOI1_AND2_NOR2, bool_op_cost(MAOI1_AND2_NOR2)});
      if(gate_or2) b.push_back({MAOI1_AND2_OR2, bool_op_cost(MAOI1_AND2_OR2)});
    }
  }
  if(gate_moai1)
  {
    b.push_back({MOAI1, bool_op_cost(MOAI1)});
    if(gate_or3) b.push_back({MOAI1_OR3, bool_op_cost(MOAI1_OR3)});
    if(gate_and3) b.push_back({MOAI1_AND3, bool_op_cost(MOAI1_AND3)});
    if(gate_nor3) b.push_back({MOAI1_NOR3, bool_op_cost(MOAI1_NOR3)});
    if(gate_nand3) b.push_back({MOAI1_NAND3, bool_op_cost(MOAI1_NAND3)});
    if(gate_or2)
    {
      b.push_back({MOAI1_OR2_OR2, bool_op_cost(MOAI1_OR2_OR2)});
      b.push_back({MOAI1_OR2, bool_op_cost(MOAI1_OR2)});
      if(gate_nor2) b.push_back({MOAI1_OR2_NOR2, bool_op_cost(MOAI1_OR2_NOR2)});
      if(gate_and2) b.push_back({MOAI1_OR2_AND2, bool_op_cost(MOAI1_OR2_AND2)});
      if(gate_nand2) b.push_back({MOAI1_OR2_NAND2, bool_op_cost(MOAI1_OR2_NAND2)});
    }
    if(gate_nor2)
    {
      b.push_back({MOAI1_NOR2, bool_op_cost(MOAI1_NOR2)});
      b.push_back({MOAI1_NOR2_NOR2, bool_op_cost(MOAI1_NOR2_NOR2)});
      if(gate_or2) b.push_back({MOAI1_NOR2_OR2, bool_op_cost(MOAI1_NOR2_OR2)});
      if(gate_and2) b.push_back({MOAI1_NOR2_AND2, bool_op_cost(MOAI1_NOR2_AND2)});
      if(gate_nand2) b.push_back({MOAI1_NOR2_NAND2, bool_op_cost(MOAI1_NOR2_NAND2)});
    }
    if(gate_and2)
    {
      b.push_back({MOAI1_AND2_AND2, bool_op_cost(MOAI1_AND2_AND2)});
      b.push_back({MOAI1_AND2, bool_op_cost(MOAI1_AND2)});
      if(gate_nand2) b.push_back({MOAI1_AND2_NAND2, bool_op_cost(MOAI1_AND2_NAND2)});
      if(gate_or2) b.push_back({MOAI1_AND2_OR2, bool_op_cost(MOAI1_AND2_OR2)});
      if(gate_nor2) b.push_back({MOAI1_AND2_NOR2, bool_op_cost(MOAI1_AND2_NOR2)});
    }
    if(gate_nand2)
    {
      b.push_back({MOAI1_NAND2, bool_op_cost(MOAI1_NAND2)});
      b.push_back({MOAI1_NAND2_NAND2, bool_op_cost(MOAI1_NAND2_NAND2)});
      if(gate_and2) b.push_back({MOAI1_NAND2_AND2, bool_op_cost(MOAI1_NAND2_AND2)});
      if(gate_or2) b.push_back({MOAI1_NAND2_OR2, bool_op_cost(MOAI1_NAND2_OR2)});
      if(gate_nor2) b.push_back({MOAI1_NAND2_NOR2, bool_op_cost(MOAI1_NAND2_NOR2)});
    }
  }

  if(gate_rnot1) 
  {
    b.push_back({RNOT1, bool_op_cost(RNOT1)});
  }
  if(gate_cnot1) 
  {
    b.push_back({CNOT1, bool_op_cost(CNOT1)});
  }
  if(gate_ccnot2)
  {
    b.push_back({CCNOT2, bool_op_cost(CCNOT2)});
  }
  if(gate_cccnot2)
  {
    b.push_back({CCCNOT2, bool_op_cost(CCCNOT2)});
  }
  if(gate_fredkin2_1)
  {
    b.push_back({FREDKIN2_1, bool_op_cost(FREDKIN2_1)});
  }
  if(gate_fredkin2_2)
  {
    b.push_back({FREDKIN2_2, bool_op_cost(FREDKIN2_2)});
  }
  if(gate_fredkin2_41)
  {
    b.push_back({FREDKIN2_41, bool_op_cost(FREDKIN2_41)});
  }
  if(gate_fredkin2_42)
  {
    b.push_back({FREDKIN2_42, bool_op_cost(FREDKIN2_42)});
  }

  sort(b.begin(), b.end());

  b.erase(remove_if(b.begin(), b.end(),
      [](bool_op_t bool_op)
      {
        static set<vector<function_t>> check_tt;
        const function_t f1({0x00ff, 0x0f0f, 0x3333, 0x5555});
        vector<function_t> f;
        bool_op_fun(bool_op.op_id, f1, &f);
        sort(f.begin(), f.end());
        auto check = check_tt.insert(f);
        return !check.second;
      }), b.end());

}

int main(int argc, char *argv[])
{

  function_t f1({0x00ff, 0x0f0f, 0x3333, 0x5555});
  function_t f2({0xa555, 0xc03f, 0xb878, 0x29b3});
  //function_t f2({0x00ff, 0x0f0f, 0x3333, 0x5555});

  struct option longopts[] = {
   { "not1", no_argument, &gate_not1, 1},
   { "and2", no_argument, &gate_and2, 1},
   { "nand2", no_argument, &gate_nand2, 1},
   { "or2", no_argument, &gate_or2, 1},
   { "nor2", no_argument, &gate_nor2, 1},
   { "xor2", no_argument, &gate_xor2, 1},
   { "xnor2", no_argument, &gate_xnor2, 1},
   { "moai1", no_argument, &gate_moai1, 1},
   { "maoi1", no_argument, &gate_maoi1, 1},
   { "nand3", no_argument, &gate_nand3, 1},
   { "nor3", no_argument, &gate_nor3, 1},
   { "and3", no_argument, &gate_and3, 1},
   { "or3", no_argument, &gate_or3, 1},
   { "andn2", no_argument, &gate_andn2, 1},
   { "orn2", no_argument, &gate_orn2, 1},
   {0, 0, 0, 0}
  };

  int opt;
  while ((opt = getopt_long(argc, argv, "i:o:m:l:vuwaqp:f:r:s:", longopts, NULL)) != EOF)
  {
    switch(opt)
    {
      case 'a': all_gates(); break;
      case 'q': reversible_gates(); break;
      case 'v': verbose = true; break;
      case 'u': one_expand = true; break;
      case 'w': write_in_file = true; break;
      case 'i': f1 = parse_function(optarg); break;
      case 'o': f2 = parse_function(optarg); break;
      case 'l': l = abs(atoi(optarg))*50; cout << "Lambda : "<< optarg << endl; break;
      case 'p': omp_nb_threads = (atoi(optarg));  cout << "Thread number : "<< omp_nb_threads<<endl;break;
      case 'f': conf_file = optarg; break;
      case 'r': max_ram = atoi(optarg); break;
      case 's': implementation_id = optarg + implementation_id; break;
      case '?': fprintf(stderr, "wrong cmd line");
    }
  }

  cout << "\nFrom : " << f1.to_string() << endl;
  cout << "To   : " << f2.to_string() << endl;

  init_b(conf_file);
  mitm(f1, f2, l);

  return 0;
}
