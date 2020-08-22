### Introduction
This work is based on the boolean UC construction, and it supports arithmetic circuits. It contains a UC translator and an MPC translator.

### Overall Usage
First run the UC translator on the circuit we want to protect to get a universal circuit file and a programming code file. Then run the MPC translator on the universal circuit to get a circuit that is able to run by our MPC. Finally, add the inputs in the beginning of the programming code file (input gates are in the front in default), and we can run the MPC protocol.

### UC translator Usage
Put the input circuit (SHDL format) in ./circuits/ and run
./UC <input> -version 4
the output is in output/circuits. See the original README.md for more details.
The gramma for the newly supported universal arithmetic gate is:
operator L p\_1 p\_2 p\_3 p\_4
(in replace of table [...] in the original SHDL file.)

### MPC translator Usage
In ./checker, run
./UC\_to\_circuit <input> <output>
The generated output can be directly used as the input circuit to our MPC protocol. The generated programming code file (in the last step) can be directly used as input file (after adding  
