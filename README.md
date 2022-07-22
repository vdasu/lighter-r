
# LIGHTER-R #
LIGHTER-R is a tool to (near-)optimally implement a given 4 × 4 SBox using logic gates from a reversible logic library (with an associated cost metric). This is an extension of the tool LIGHTER [hyperlinked [original publication](https://tosc.iacr.org/index.php/ToSC/article/view/806), [archived publication](https://eprint.iacr.org/2017/101), [source codes](http://jeremy.jean.free.fr/pub/fse2018_layer_implementations.tar.gz)]; which only works for 4 × 4 SBoxes the classical computing paradigm, namely for ASIC and CPU implementations. 

The source code, hosted on [GitHub](https://github.com/vdasu/lighter-r), is a supporting component for the paper:
**LIGHTER-R: Optimized Reversible Circuit Implementation For SBoxes**
by [Vishnu Asutosh Dasu](mailto:vishnu.asutosh@learner.manipal.edu), [Anubhab Baksi](mailto:anubhab001@e.ntu.edu.sg), Sumanta Sarkar and Anupam Chattopadhyay [[IEEE SoCC 2019](http://socc2019.ieee-socc.org/)/[publication](https://ieeexplore.ieee.org/abstract/document/9088027)].

Please feel free to suggest/contribute new features or fix bugs or add documentation.

## Command Line Usage ##
    ./non-lin-search [-v] [-w] [-u] [-q]
                   [-l NUMBER] [-p NUMBER] [-r NUMBER]
                   [-i SBOX] [-o SBOX]
                   [-f FILE] [-s IMPLEMENTATION_ID]

    -v : Verbose mode
    -u : Expand only [-i] function (identity function by default)
    -w : Write graph output in a file
    
    -q : Enable all reversible gates (essential for LIGHTER-R)

    -l <value> : Define an upper bound on the cost of the implementations generated
    -p <value> : Define the number of threads used
    -r <value> : Define an upper bound on the RAM used by the algorithm

    -i <function> : Define start function
    -o <function> : Define arrival function

    -f <file> : Define the logic library with the associated cost metric

    -s <string> : Define implementation information

Apart from `-q` and `-s`, rest of the options are adopted from the vanilla LIGHTER; and the `-a` option is omitted. The `-q` option is necessary for LIGHTER-R to work; and the `-s` option takes a string (such as the cipher name) and associates the string to the names of the C-like implementation files.


## Implemented Reversible Logic Gates ##
<!-- **Fredkin gates are not supported**
|            Gate            |      Library      |Quantum Cost | Two-qubit Cost  | Gate Count |           Symbol           |
|:--------------------------:|:-----------------:|:-----------:|:---------------:|:----------:|:--------------------------:|
|   NOT (Tof<sub>1</sub>)    | NCT/ MCT/ MCT+MCF |      1      |         0       |     1      |            RNOT1           |
|   CNOT  (Tof<sub>2</sub>)  | NCT/ MCT/ MCT+MCF |      1      |         1       |     1      |            CNOT1           |
|  CCNOT (Tof<sub>3</sub>)   | NCT/ MCT/ MCT+MCF |      5      |         5       |     1      |            CCNOT2          |
|      Tof<sub>4</sub>       |    MCT/ MCT+MCF   |      13     |         5       |     1      |            CCCNOT2         |
|     Fred<sub>3</sub>       |    MCF/ MCT+MCF   |      7      |         5       |     1      |  FREDKIN2_31, FREDKIN2_32  |
|     Fred<sub>4</sub>       |    MCF/ MCT+MCF   |      15     |         5       |     1      |  FREDKIN2_41, FREDKIN2_42  |

The quantum cost and the two-qubit cost metrics are adopted from the [RCVIWER+](https://ceit.aut.ac.ir/QDA/RCV.htm) (version 2.5) tool. It shows the two-qubit cost for the Fred<sub>4</sub> gate as −5, we treat this as an editorial mistake and consider the cost as 5.
-->

|            Gate            | Library  |Quantum Cost | Two-qubit Cost  | Gate Count |           Symbol  |
|:--------------------------:|:--------:|:-----------:|:---------------:|:----------:|:--------------------------:|
|   NOT (Tof<sub>1</sub>)    | NCT/ MCT |      1      |         0       |     1      |            RNOT1           |
|   CNOT  (Tof<sub>2</sub>)  | NCT/ MCT |      1      |         1       |     1      |            CNOT1           |
|  CCNOT (Tof<sub>3</sub>)   | NCT/ MCT |      5      |         5       |     1      |            CCNOT2          |
|      Tof<sub>4</sub>       |    MCT   |      13     |         5       |     1      |            CCCNOT2         |


### Naming Convention ###
The number following the logic gate symbol denotes the number of primitive logic gates used to implement the gate. For example, CNOT1 uses 1 XOR gate in its implementation. In the case of Tof<sub>3</sub> XOR and AND gates are used, hence CCNOT2.
<!--The 3 and 4 in the symbol for the FREDKIN gates represent the input size in bits. The 1 and 2 denote which output line is being used. For example, FREDKIN2_32 is the symbol for the 3 bit FREDKIN gate where the second output line is used.
-->

### TFC Support ###
Inside the [/reversible-helper](./reversible-helper) directory, the [`lighter_to_tfc.py`](./reversible-helper/lighter_to_tfc.py) file allows to convert a generated C file to the [.tfc](http://webhome.cs.uvic.ca/~dmaslov/mach-read.html) format. To use it, pass the source file in the command line. The output file will have the same name and will be generated in the same direcroty as the source file.

## Notes ##

1. The `-a` option should not be used while proceesing LIGHTER-R (i.e., together with `-q`).
2. The Peres (Per<sub>3</sub>) gate is implemented in the [peres](https://github.com/vdasu/lighter-r/tree/peres) branch, but this branch is not under active development.
3. Apparently LIGHTER does not support logic gates with 0 cost. Through empirical tests, we observe that it does not work (basically, prints nothing) if the cost is less than 0.01. For this reason, the cost for the NOT (Tof<sub>1</sub>) gate has been substituted for 0.01 instead in all three (NCT, MCT and MCT+MCF) libraries.
4. LIGHTER-R requires `g++11` or greater.

### Changes From Vanilla LIGHTER ###

1. All basic functionalities are preserved — so the vanilla LIGHTER can be utilized (including the `-a` option)
2. `software.conf` is renamed as [`CPU.conf`](./libraries/CPU.conf) and all the libraries (`.conf` files) are moved to the [/libraries](./libraries) directory
3. The description of the SBox (such as <tt>0123456789ABCDEF</tt>) is used automatically in the names of the C-like implementation files when `-s` is not used
4. Minor update is made in the [`Makefile`](./Makefile) to suppress a warning (`utils.cpp:65:31: warning: ignoring return value of ‘int system(const char*)’, declared with attribute warn_unused_result [-Wunused-result]`)
