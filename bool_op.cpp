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

void bool_op_simple_gates(function_t f, vector<function_t> *v, int operation)
{
  function_t fun_tmp = f;
  
  if(operation == RNOT1) 
  {
    for(int i = 0; i < N; i++) 
    {
      fun_tmp.bit_slice[i] = ~(f.bit_slice[i]);
      fun_tmp.info_line = (uint8_t)i;
      fun_tmp.info_op = (uint8_t)operation;
      fun_tmp.prev = f.bit_slice[i];
      fun_tmp.sort();
      v->push_back(fun_tmp);
      fun_tmp = f;
    }
  }
  else if(operation == NOT1)
  {
    for (int i = 0; i < N; i++)
    {
      fun_tmp.bit_slice[i] = ~(f.bit_slice[i]);
      fun_tmp.info_line = (uint8_t)i;
      fun_tmp.info_op = (uint8_t)operation;
      fun_tmp.prev = f.bit_slice[i];
      fun_tmp.sort();
      v->push_back(fun_tmp);
      fun_tmp = f;
    }
  }
  else
  {
    for(int i = 0; i < N; i++)
    {
      for(int j = 0; j < N; j++)
      {
        if(i != j)
        {
          switch (operation)
          {
            case MAOI1 :
            case XOR2  : fun_tmp.bit_slice[i] = (f.bit_slice[i])^(f.bit_slice[j]); break;
            case MOAI1 :
            case XNOR2 : fun_tmp.bit_slice[i] = ~((f.bit_slice[i])^(f.bit_slice[j])); break;
            case CNOT1 : fun_tmp.bit_slice[i] = (f.bit_slice[i])^(f.bit_slice[j]); 
                         int _ = f.bit_slice[i]; 
                         break;
          }
          fun_tmp.prev = f.bit_slice[i];
          fun_tmp.info_line = (uint8_t)i;
          fun_tmp.info_op = (uint8_t)operation;
          fun_tmp.sort();
          v->push_back(fun_tmp);
          fun_tmp = f;
        }
      }
      fun_tmp = f;
    }
  }
}

void bool_op_double_gates(function_t f, vector<function_t> *v, int operation)
{
  auto two_inputs = [](const function_t f, int i, int a, int b, int operation)
  {
    auto fun_tmp = f;
    switch (operation)
    {
      case MAOI1_NAND2 :
      case MOAI1_AND2  : fun_tmp.bit_slice[i] = ~(f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N])^f.bit_slice[i];break;
      case MAOI1_NOR2  :
      case MOAI1_OR2   : fun_tmp.bit_slice[i] = ~(f.bit_slice[(i+a)%N]|f.bit_slice[(i+b)%N])^f.bit_slice[i];break;
      case XOR2_AND2   :
      case MAOI1_AND2  :
      case MOAI1_NAND2 : fun_tmp.bit_slice[i] = (f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N])^f.bit_slice[i];break;
      case CCNOT2      : { 
                          fun_tmp.bit_slice[i] = (f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N])^f.bit_slice[i];
                          int _ = f.bit_slice[(i+a)%N];
                          int __ = f.bit_slice[(i+b)%N];
                          break;
                         }
      case FREDKIN2_31 : { 
                          fun_tmp.bit_slice[i] = (~f.bit_slice[(i+a)%N]&f.bit_slice[i])|(f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]);
                          int _ = f.bit_slice[(i+a)%N];
                          int __ = (~f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N])|(f.bit_slice[(i+a)%N]&f.bit_slice[i]);
                          break;
                         }
      case FREDKIN2_32 : { 
                          fun_tmp.bit_slice[i] = (~f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N])|(f.bit_slice[(i+a)%N]&f.bit_slice[i]);
                          int _ = f.bit_slice[(i+a)%N];
                          int __ = (~f.bit_slice[(i+a)%N]&f.bit_slice[i])|(f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]);
                          break;                         
                         }

      case XOR2_OR2    :
      case MAOI1_OR2   :
      case MOAI1_NOR2  : fun_tmp.bit_slice[i] = (f.bit_slice[(i+a)%N]|f.bit_slice[(i+b)%N])^f.bit_slice[i];break;
      case XOR2_ANDN2  : fun_tmp.bit_slice[i] = (~f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N])^f.bit_slice[i];break;
      case XOR2_ORN2   : fun_tmp.bit_slice[i] = (~f.bit_slice[(i+a)%N]|f.bit_slice[(i+b)%N])^f.bit_slice[i];break;
   }
    fun_tmp.prev = f.bit_slice[i];
    fun_tmp.info_line = i;
    fun_tmp.info_op = operation;
    fun_tmp.sort();
    return fun_tmp;
  };

  auto three_inputs = [](const function_t f, int i, int a, int b, int c, int operation)
  {
    auto fun_tmp = f;
    switch (operation)
    {
      case XOR2_AND2_AND2   :
      case MAOI1_AND3       :
      case MOAI1_NAND2_AND2 :
      case MAOI1_AND2_AND2  :
      case MOAI1_NAND3      : fun_tmp.bit_slice[i] = (f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])^f.bit_slice[i];break;
      case XOR2_OR2_OR2     :
      case MAOI1_OR3        :
      case MOAI1_NOR2_OR2   :
      case MAOI1_OR2_OR2    :
      case MOAI1_NOR3       : fun_tmp.bit_slice[i] = (f.bit_slice[(i+a)%N]|f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N])^f.bit_slice[i];break;
      case MOAI1_AND3       :
      case MAOI1_NAND2_AND2 :
      case MOAI1_AND2_AND2  :
      case MAOI1_NAND3      : fun_tmp.bit_slice[i] = ~(f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])^f.bit_slice[i];break;
      case MOAI1_OR3        :
      case MAOI1_NOR2_OR2   :
      case MOAI1_OR2_OR2    :
      case MAOI1_NOR3       : fun_tmp.bit_slice[i] = ~(f.bit_slice[(i+a)%N]|f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N])^f.bit_slice[i];break;
      case CCCNOT2          : {
                                fun_tmp.bit_slice[i] = (f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])^f.bit_slice[i];
                                int _ = f.bit_slice[(i+a)%N];
                                int __ = f.bit_slice[(i+b)%N];
                                int ___ = f.bit_slice[(i+c)%N];
                                break;
                              }
      case FREDKIN2_41     : {
                                fun_tmp.bit_slice[i] = (~(f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])) | 
                                                        (f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]&f.bit_slice[i]);
                                int _ = f.bit_slice[(i+a)%N];
                                int __ = f.bit_slice[(i+b)%N];
                                int ___ = (~(f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]&f.bit_slice[i])) | 
                                          (f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N]);
                                break;
                             }
      case FREDKIN2_42    : {
                                fun_tmp.bit_slice[i] = (~(f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]&f.bit_slice[i])) | 
                                                        (f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N]);
                                int _ = f.bit_slice[(i+a)%N];
                                int __ = f.bit_slice[(i+b)%N];
                                int ___ = (~(f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])) | 
                                          (f.bit_slice[(i+a)%N]&f.bit_slice[(i+b)%N]&f.bit_slice[i]);
                                break;
                             }
    }
    fun_tmp.prev = f.bit_slice[i];
    fun_tmp.info_line = (uint8_t)i;
    fun_tmp.info_op = (uint8_t)operation;
    fun_tmp.sort();
    return fun_tmp;
  };

  switch (operation)
  {
    case MAOI1_AND3       :
    case MOAI1_NAND2_AND2 :
    case MAOI1_AND2_AND2  :
    case MAOI1_OR3        :
    case MOAI1_NOR2_OR2   :
    case MAOI1_OR2_OR2    :
    case MOAI1_AND3       :
    case MAOI1_NAND2_AND2 :
    case MOAI1_AND2_AND2  :
    case MOAI1_OR3        :
    case MAOI1_NOR2_OR2   :
    case MOAI1_OR2_OR2    :
    case MOAI1_NAND3      :
    case MOAI1_NOR3       :
    case MAOI1_NAND3      :
    case MAOI1_NOR3       : 
    case XOR2_AND2_AND2   :
    case XOR2_OR2_OR2     :
    case CCCNOT2          :
    case FREDKIN2_41      :
    case FREDKIN2_42      :
                       for (int i = 0; i < N; i++)
                       {
                         v->push_back(three_inputs(f, i, 1, 2, 3, operation));
                       }
                       break;

    case MAOI1_NAND2 :
    case MAOI1_NOR2  :
    case MAOI1_AND2  :
    case MAOI1_OR2   :
    case MOAI1_NAND2 :
    case MOAI1_NOR2  :
    case MOAI1_AND2  :
    case MOAI1_OR2   :
    case XOR2_AND2   :
    case XOR2_OR2    :
    case XOR2_ANDN2  :
    case XOR2_ORN2   :
    case CCNOT2      :
    case FREDKIN2_31 :
    case FREDKIN2_32 :
                       for (int i = 0; i < N; i++)
                       {
                         v->push_back(two_inputs(f, i, 1, 2, operation));
                         v->push_back(two_inputs(f, i, 2, 3, operation));
                         v->push_back(two_inputs(f, i, 1, 3, operation));
                       }
                       break;
  }
}

void bool_op_triple_gates(function_t f, vector<function_t> *v, int operation)
{
  auto double_gates = [](const function_t f, int i, int a, int b, int c, int operation)
  {
    auto fun_tmp = f;
    switch (operation)
    {
      case MAOI1_NAND2_NAND2 :
      case MOAI1_AND2_NAND2  : fun_tmp.bit_slice[i] = ~(~(f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])&f.bit_slice[(i+a)%N])^f.bit_slice[i];break;
      case MAOI1_NOR2_NOR2   :
      case MOAI1_OR2_NOR2    : fun_tmp.bit_slice[i] = ~(~(f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N])|f.bit_slice[(i+a)%N])^f.bit_slice[i];break;
      case MAOI1_AND2_NAND2  :
      case MOAI1_NAND2_NAND2 : fun_tmp.bit_slice[i] = (~(f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])&f.bit_slice[(i+a)%N])^f.bit_slice[i];break;
      case MAOI1_OR2_NOR2    :
      case MOAI1_NOR2_NOR2   : fun_tmp.bit_slice[i] = (~(f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N])|f.bit_slice[(i+a)%N])^f.bit_slice[i];break;
      case XOR2_AND2_OR2     :
      case MAOI1_AND2_OR2    :
      case MOAI1_NAND2_OR2   : fun_tmp.bit_slice[i] = ((f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N])&f.bit_slice[(i+a)%N])^f.bit_slice[i];break;
      case XOR2_OR2_AND2     :
      case MAOI1_OR2_AND2    :
      case MOAI1_NOR2_AND2   : fun_tmp.bit_slice[i] = ((f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])|f.bit_slice[(i+a)%N])^f.bit_slice[i];break;
      case MAOI1_AND2_NOR2   :
      case MOAI1_NAND2_NOR2  : fun_tmp.bit_slice[i] = (~(f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N])&f.bit_slice[(i+a)%N])^f.bit_slice[i];break;
      case MAOI1_OR2_NAND2   :
      case MOAI1_NOR2_NAND2  : fun_tmp.bit_slice[i] = (~(f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])|f.bit_slice[(i+a)%N])^f.bit_slice[i];break;
      case MAOI1_NAND2_OR2   :
      case MOAI1_AND2_OR2    : fun_tmp.bit_slice[i] = ~((f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N])&f.bit_slice[(i+a)%N])^f.bit_slice[i];break;
      case MAOI1_NOR2_AND2   :
      case MOAI1_OR2_AND2    : fun_tmp.bit_slice[i] = ~((f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])|f.bit_slice[(i+a)%N])^f.bit_slice[i];break;
      case MAOI1_NAND2_NOR2  :
      case MOAI1_AND2_NOR2   : fun_tmp.bit_slice[i] = ~(~(f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N])&f.bit_slice[(i+a)%N])^f.bit_slice[i];break;
      case MAOI1_NOR2_NAND2  :
      case MOAI1_OR2_NAND2   : fun_tmp.bit_slice[i] = ~(~(f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N])|f.bit_slice[(i+a)%N])^f.bit_slice[i];break;

      case XOR2_OR2_ANDN2    : fun_tmp.bit_slice[i] = ( f.bit_slice[(i+a)%N] | (~f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      case XOR2_OR2_ORN2     : fun_tmp.bit_slice[i] = ( f.bit_slice[(i+a)%N] | (~f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      case XOR2_AND2_ANDN2   : fun_tmp.bit_slice[i] = ( f.bit_slice[(i+a)%N] & (~f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      case XOR2_AND2_ORN2    : fun_tmp.bit_slice[i] = ( f.bit_slice[(i+a)%N] & (~f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      case XOR2_ANDN2_AND2   : fun_tmp.bit_slice[i] = ( ~f.bit_slice[(i+a)%N] & (f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      case XOR2_ANDN2_OR2    : fun_tmp.bit_slice[i] = ( ~f.bit_slice[(i+a)%N] & (f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      case XOR2_ANDN2_ANDN2  : fun_tmp.bit_slice[i] = ( ~f.bit_slice[(i+a)%N] & (~f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      case XOR2_ANDN2_ORN2   : fun_tmp.bit_slice[i] = ( ~f.bit_slice[(i+a)%N] & (~f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      case XOR2_ORN2_AND2    : fun_tmp.bit_slice[i] = ( ~f.bit_slice[(i+a)%N] | (f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      case XOR2_ORN2_OR2     : fun_tmp.bit_slice[i] = ( ~f.bit_slice[(i+a)%N] | (f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      case XOR2_ORN2_ANDN2   : fun_tmp.bit_slice[i] = ( ~f.bit_slice[(i+a)%N] | (~f.bit_slice[(i+b)%N]&f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
      case XOR2_ORN2_ORN2    : fun_tmp.bit_slice[i] = ( ~f.bit_slice[(i+a)%N] | (~f.bit_slice[(i+b)%N]|f.bit_slice[(i+c)%N]) )^f.bit_slice[i];break;
    }
    fun_tmp.prev = f.bit_slice[i];
    fun_tmp.info_line = (uint8_t)i;
    fun_tmp.info_op = (uint8_t)operation;
    fun_tmp.sort();
    return fun_tmp;
  };

  for (int i = 0; i < N; i++)
  {
    v->push_back(double_gates(f, i, 1, 2, 3, operation));
    v->push_back(double_gates(f, i, 2, 3, 1, operation));
    v->push_back(double_gates(f, i, 3, 1, 2, operation));
  }
}


void bool_op_fun( int op_id, function_t f, vector<function_t> *v)
{
  switch (op_id)
  {
    case NOT1  :
    case XOR2  :
    case XNOR2 :
    case MAOI1 :
    case MOAI1 : 
    case RNOT1 :
    case CNOT1 : bool_op_simple_gates(f, v, op_id); break;

    case MOAI1_NAND2      :
    case MOAI1_NOR2       :
    case MOAI1_AND2       :
    case MOAI1_OR2        :
    case MAOI1_NAND3      :
    case MAOI1_NOR3       :
    case MOAI1_NAND3      :
    case MOAI1_NOR3       :
    case XOR2_AND2        :
    case XOR2_OR2         :
    case XOR2_ANDN2       :
    case XOR2_ORN2        :
    case XOR2_OR2_OR2     :
    case XOR2_AND2_AND2   :

    case MAOI1_NAND2      :
    case MAOI1_NOR2       :
    case MAOI1_AND2       :
    case MAOI1_OR2        :
    case MAOI1_AND3       :
    case MOAI1_NAND2_AND2 :
    case MAOI1_AND2_AND2  :
    case MAOI1_OR3        :
    case MOAI1_NOR2_OR2   :
    case MAOI1_OR2_OR2    :
    case MOAI1_AND3       :
    case MAOI1_NAND2_AND2 :
    case MOAI1_AND2_AND2  :
    case MOAI1_OR3        :
    case MAOI1_NOR2_OR2   :
    case MOAI1_OR2_OR2    : 
    case CCNOT2           : 
    case CCCNOT2          : 
    case FREDKIN2_31      : 
    case FREDKIN2_32      : 
    case FREDKIN2_41      : 
    case FREDKIN2_42      : bool_op_double_gates(f, v, op_id); break;

    case MOAI1_AND2_NAND2  :
    case MOAI1_OR2_NOR2    :
    case MOAI1_NAND2_NAND2 :
    case MOAI1_NOR2_NOR2   :
    case MOAI1_NAND2_OR2   :
    case MOAI1_NOR2_AND2   :
    case MOAI1_NAND2_NOR2  :
    case MOAI1_NOR2_NAND2  :
    case MOAI1_AND2_OR2    :
    case MOAI1_OR2_AND2    :
    case MOAI1_AND2_NOR2   :
    case MOAI1_OR2_NAND2   :

    case XOR2_OR2_AND2     :
    case XOR2_OR2_ANDN2    :
    case XOR2_OR2_ORN2     :

    case XOR2_AND2_OR2     :
    case XOR2_AND2_ANDN2   :
    case XOR2_AND2_ORN2    :

    case XOR2_ANDN2_AND2   :
    case XOR2_ANDN2_OR2    :
    case XOR2_ANDN2_ANDN2  :
    case XOR2_ANDN2_ORN2   :

    case XOR2_ORN2_AND2    :
    case XOR2_ORN2_OR2     :
    case XOR2_ORN2_ANDN2   :
    case XOR2_ORN2_ORN2    :

    case MAOI1_NAND2_NAND2 :
    case MAOI1_NOR2_NOR2   :
    case MAOI1_AND2_NAND2  :
    case MAOI1_OR2_NOR2    :
    case MAOI1_AND2_OR2    :
    case MAOI1_OR2_AND2    :
    case MAOI1_AND2_NOR2   :
    case MAOI1_OR2_NAND2   :
    case MAOI1_NAND2_OR2   :
    case MAOI1_NOR2_AND2   :
    case MAOI1_NAND2_NOR2  :
    case MAOI1_NOR2_NAND2  : bool_op_triple_gates(f, v, op_id); break;
  }
}
