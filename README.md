
# LIGHTER-R #
LIGHTER-R is a tool to (near-) optimally implement a given 4 × 4 SBox in using a given reversible logic library (with an associated cost metric). This is an extension of the tool LIGHTER ([original publication](https://tosc.iacr.org/index.php/ToSC/article/view/806), [archived version](https://eprint.iacr.org/2017/101), [source codes](http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz)); which only works for 4 × 4 SBoxes the classical computing paradigm, namely for ASIC and CPU implementations.

> The source codes are a supporting material for the paper:
**Optimizing SBoxes For Circuit Implementation: From Classical To Reversible**
by Vishnu Asutosh Dasu, Anubhab Baksi, Sumanta Sarkar and Anupam Chattopadhyay.


## How to use ##
  
   `./non-lin-search [-v] [-w] [-u] [-q]
                   [-l NUMBER] [-p NUMBER] [-r NUMBER]
                   [-i SBOX] [-o SBOX]
                   [-f FILE]`

    -v : Verbose mode
    -u : Expand only [-i] function (identity function by default)
    -w : Write graphe in file
    
    -q : Enable all reversible gates

    -l <value> : Define an upper bound on the cost of the implementations generated
    -p <value> : Define the number of threads used
    -r <value> : Define an upper bound on the RAM used by the algorithm

    -i <function> : define start function
    -o <function> : define arrival function

    -f <file> : Define which logic library you want to use

See the "README-lighter" file (which is the original readme content for the vanilla LIGHTER) for more information.

## Implemented Reversible Logic Libraries ##
| Libray        | Quantum Cost  | Two-qubit Cost  | Gate Count |
|:-------------:|:-------------:|:---------------:|:----------:|



## Notes ##
1. The `-a` option should not be used while proceesing LIGHTER-R (i.e., with the `-q` option). 
2. As the Fredkin gates have more than one effective output lines (the output lines which are not readily any input line), those gates are implemented separately.
3. *The source codes come with no warranty. See the LICENSE file for more details.*
4. *We invite you to contribute: Please feel free to suggest/contribute new changes or fix bugs or add documentation.*

## Contributors ##
* [Vishnu Asutosh Dasu](<vishnu.asutosh@learner.manipal.edu>)
* [Anubhab Baksi](anubhab001@e.ntu.edu.sg)
* Sumanta Sarkar, 
* Anupam Chattopadhyay