### Introduction
This work is based on the boolean UC construction, and it supports arithmetic circuits. It contains a UC translator and an MPC translator.

### Overall Usage
First run the UC translator on the circuit we want to protect to get a universal circuit file and a programming code file. Then run the MPC translator on the universal circuit to get a circuit that is able to run by our MPC. Finally, add the inputs in the beginning of the programming code file (input gates are in the front in default), and we can run our MPC protocol to get results.

### UC translator Usage
Put the input circuit (SHDL format) in circuits/ and run
```
./UC <input> -version 4
```
the output is in output/circuits. See README\_booleanUC.md (the original README for boolean universal circuits) for more details. If you don't use version 4 some bugs may occur, these bugs exist in the original construction and I have no idea for them.

The grammar for the newly supported universal arithmetic gate is:
```
operator L p1 p2 p3 p4
```
(in replace of `table [...]` in the original SHDL file.) For example, an addition gate can be:
```
3 gate arity 2 operator L 0 1 1 0 inputs [ 0 1 ]
```

### MPC translator Usage
In checker/, run
```
./UC_to_circuit <input> <output>
```
The generated output can be directly used as the input circuit to our MPC protocol. (The circuit format is the same as libscapi circuits.) The generated programming code file (in the last step) can be directly used as input file (after adding inputs in the front of it).

### MPC protocol
See [this](https://github.com/hanjunLi/MPCHonestMajorityNoTriples) for more details.
