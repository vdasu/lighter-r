'''
t-count.py - counter of T gates in a reversible circuit.
Copyright (C) 2016  <Dmitry V. Zakablukov>
E-mail: dmitriy.zakablukov@gmail.com
Web: https://github.com/dmitry-zakablukov/ReversibleLogicGenerator

This script is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This script is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with This script.  If not, see <http://www.gnu.org/licenses/>.

Usage: python t-count.py <folder_with_circuits>
All circuits should be in TFC or REAL format.

Output (on screen) will be of the form:
<circuit_name> : n = <input_lines_number>, gc = <gate_complexity>, t-count = <t-count>
'''

import os
import sys

# Returns (T-count, A) for a TOF-k ((k-1)-CNOT) gate, where A is of boolean type
# A == True, if ancilla line is required 
def getTcount(k, n):
    # T-count calculation rules
    if k < 3:
        return (0, False)
    if k == 3:
        return (7, False)
    
    # check free lines count
    if n - k < int((n - 2) / 2):
        # using Lemma 7.3 from "Elementary gates for quantum computation"
        ancilla = (k == n)
        t_count = 0
        
        m = int((k + 1) / 2)
        
        t, _ = getTcount(m + 1, k + 1)
        t_count += 2 * (t - 1 * ancilla)
        
        t, _ = getTcount(k - m, k + 1)
        t_count += 2 * (t - 1 * ancilla)
        
        return (t_count, ancilla)
        
    # Table 1 from "On the advantages of using relative phase Toffolis..."
    return (8 * k - 16, False)

for root, dirs, files in os.walk(sys.argv[1]):
    for file in files:
        with open(os.path.join(root, file), 'rt') as f:
            lines = f.readlines()
            
            n = None
            gc = 0
            weight = 0
            ancilla = False
            
            begin = False            
            for line in lines:
                value = line.lower()
                if value[:2] == '.v':
                    parts = value.split()
                    if len(parts) == 2:
                        parts = parts[1].split(',')
                    else:
                        parts = parts[1:]
                    n = len(parts)
                    continue
                    
                if value[:5] == 'begin' or value[:6] == '.begin':
                    begin = True
                    continue
                    
                if begin and value[:1] == 't':
                    parts = value.split()
                    if len(parts) == 2:
                        parts = parts[1].split(',')
                    else:
                        parts = parts[1:]
                    k = len(parts)
                    
                    t_count, a = getTcount(k, n)
                    
                    weight += t_count
                    ancilla = ancilla or a
                    gc += 1

            print '%s : n = %u, gc = %u, t-count = %u%s' % (file, n, gc, weight, '*' if ancilla else '')