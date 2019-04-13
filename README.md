
# LIGHTER-R #
LIGHTER-R is a tool to (near-)optimally implement a given 4 × 4 SBox in using logic gates from a reversible logic library (with an associated cost metric). This is an extension of the tool LIGHTER [[original publication](https://tosc.iacr.org/index.php/ToSC/article/view/806), [archived publication](https://eprint.iacr.org/2017/101), [source codes](http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz)]; which only works for 4 × 4 SBoxes the classical computing paradigm, namely for ASIC and CPU implementations. 

The source codes, hosted at [GitHub](https://github.com/vdasu/lighter-r), are a supporting component for the paper:
**Optimizing SBoxes For Circuit Implementation: From Classical To Reversible**
by Vishnu Asutosh Dasu, Anubhab Baksi, Sumanta Sarkar and Anupam Chattopadhyay.

Please feel free to suggest/contribute new features or fix bugs or add documentation.

## How to Use ##

### Command Line Usage ###
    ./non-lin-search [-v] [-w] [-u] [-q]
                   [-l NUMBER] [-p NUMBER] [-r NUMBER]
                   [-i SBOX] [-o SBOX]
                   [-f FILE] [-s IMPLEMENTATION_ID]

    -v : Verbose mode
    -u : Expand only [-i] function (identity function by default)
    -w : Write graphe in file
    
    -q : Enable all reversible gates (essential for LIGHTER-R)

    -l <value> : Define an upper bound on the cost of the implementations generated
    -p <value> : Define the number of threads used
    -r <value> : Define an upper bound on the RAM used by the algorithm

    -i <function> : Define start function
    -o <function> : Define arrival function

    -f <file> : Define the logic library with the associated cost metric

    -s <string> : Define implementation information

Apart from `-q` and `-s`, rest options are adopted from the vanilla LIGHTER; and the `-a` option is omitted. The `-q` option is necessary for LIGHTER-R to work; and the `-s` option takes a string (such as the cipher name) and associates the string to the names of the C-like implementation files.

### Examples ###

## Implemented Reversible Logic Gates ##

|            Gate            |      Library      |Quantum Cost | Two-qubit Cost  | Gate Count |   Symbol     |
|:--------------------------:|:-----------------:|:-----------:|:---------------:|:----------:|:------------:|
|   NOT (Tof<sub>1</sub>)    | NCT/ MCT/ MCT+MCF |      1      |         0       |     1      |  |
|   CNOT  (Tof<sub>2</sub>)  | NCT/ MCT/ MCT+MCF |      1      |         1       |     1      |  |
|  CCNOT (Tof<sub>3</sub>)   | NCT/ MCT/ MCT+MCF |      5      |         5       |     1      |  |
|      Tof<sub>4</sub>       |    MCT/ MCT+MCF   |      13     |         5       |     1      |  |
|     Fred<sub>3</sub>       |    MCF/ MCT+MCF   |      7      |         5       |     1      |  |
|     Fred<sub>4</sub>       |    MCF/ MCT+MCF   |      15     |         5       |     1      |  |

The quantum cost and the two-qubit cost metrics are adopted from the [RCVIWER+](https://ceit.aut.ac.ir/QDA/RCV.htm) (version 2.5) tool. It shows the two-qubit cost for the Fred<sub>4</sub> gate is shown as −5, we treat this as an editorial mistake and consider the cost as 5.

### Convention for Symbols ###

## Notes ##
1. The `-a` option should not be used while proceesing LIGHTER-R (i.e., together with `-q`). 
2. As the Fredkin gates have more than one effective output lines (the output lines which are not readily any input line), those gates are implemented separately:
    1. Fred<sub>3</sub>
    2. Fred<sub>4</sub>
3. The Peres (Per<sub>3</sub>) gate is implemented in the [peres](https://github.com/vdasu/lighter-r/tree/peres) branch, but this brance is not under active development
4. *The source codes come with no warranty. See the [LICENSE](./LICENSE) file for more details.*


### Changes Made in Vanilla LIGHTER ###

0. All basic functionalities are preserved — so the vanilla LIGHTER can be utilized (including the `-a` option)
1. `software.conf` is renamed as [`CPU.conf`](./conf_files/CPU.conf) and all the libraries (`.conf` files) are moved to the [conf_files](./conf_files) directory
2. The description of the SBox (such as `0123456789ABCDEF`) is used automatically in the names of the C-like implementation files when `-s` is not used
3. Minor update is made in the [Makefile](./Makefile) to suppress a warning (`utils.cpp:65:31: warning: ignoring return value of ‘int system(const char*)’, declared with attribute warn_unused_result [-Wunused-result]`)

## Contributors ##
* [Vishnu Asutosh Dasu](mailto:vishnu.asutosh@learner.manipal.edu)
* [Anubhab Baksi](mailto:anubhab001@e.ntu.edu.sg)
* Sumanta Sarkar
* Anupam Chattopadhyay
