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

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

uint16_t MAOI1(uint16_t x, uint16_t y, uint16_t z, uint16_t t)
{
  return (~((x & y) | (~(z | t))));
}

uint16_t MOAI1(uint16_t x, uint16_t y, uint16_t z, uint16_t t)
{
  return (~((x | y) & (~(z & t))));
}

uint16_t NAND2(uint16_t x, uint16_t y)
{
  return ~(x & y);
}

uint16_t NOR2(uint16_t x, uint16_t y)
{
  return ~(x | y);
}

uint16_t NAND3(uint16_t x, uint16_t y, uint16_t z)
{
  return ~(x & y & z);
}

uint16_t NOR3(uint16_t x, uint16_t y, uint16_t z)
{
  return ~(x | y | z);
}

uint16_t AND3(uint16_t x, uint16_t y, uint16_t z)
{
  return (x & y & z);
}

uint16_t OR3(uint16_t x, uint16_t y, uint16_t z)
{
  return (x | y | z);
}

uint16_t AND2(uint16_t x, uint16_t y)
{
  return (x & y);
}

uint16_t OR2(uint16_t x, uint16_t y)
{
  return (x | y);
}

uint16_t XOR2(uint16_t x, uint16_t y)
{
  return (x ^ y);
}

uint16_t XNOR2(uint16_t x, uint16_t y)
{
  return ~(x ^ y);
}

uint16_t NOT1(uint16_t x)
{
  return ~x;
}

int main()
{
  uint16_t X[4] = {0x00FF, 0x0F0F, 0x3333, 0x5555};
  uint16_t F[4];
  int i;

  #include "implementation_0.c"

  i = 0;
  while( i < 4)
  {
    printf("%x ", X[i]);
    i++;
  }
  printf("\n");
  return 0;
}
