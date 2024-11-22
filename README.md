# F1 FormulaSiulator1 
Here is an Optical/Electrical transport simulator with task scheduler and simulator.

# Before you start

You should ensure that your machine has installed [Goole OR-Tools(CPP API)](https://developers.google.com/optimization).

# How to use

1. Set LD_LIBRARY_PATH
```bash
export LD_LIBRARY_PATH=YourPathToORTools/lib/:$LD_LIBRARY_PATH
```
You should change `YourPathToORTools` to your own OR tool's root directory.

example:
```bash
export LD_LIBRARY_PATH=~/or-tools_x86_64_Ubuntu-22.04_cpp_v9.11.4210/lib/:$LD_LIBRARY_PATH
```

You can also change the file: config.sh for convenience.
```bash
source config.sh
```

2. Compile

You can use make to compile, before that, you should change `INCLUDE_PATH` and `LIB_PATH` of Makefile to your own paths. After that, you can use `make`:

```bash
make all
```

If you want to compile a single src file, type like this:
```bash
g++ -I YourPathToORTools/include/ ORexamples.cc -L YourPathToORTools/lib/ -lortools -o ORexamples
```
You should change `YourPathToORTools` to your own OR tool's root directory.

3. Use

For using ILP solver to get a serial of Optical configure, and the TX time,  type:
```bash
./OpticalSolution
```

Using [Solstice](https://dl.acm.org/doi/pdf/10.1145/2716281.2836126) to get a serial of Optical configure, and the TX time, type:
```bash
./Solstice
```

For generating the Pipeline parallelism's workflow, use:
```bash
make
./PPTaskGenerator <workernum> <forwardtime> <backwardtime> <batchsize> <simulatetime>
```
This will generate the workflow in file: ./PPTaskGenResult.txt

---
For Electrical Tranport Simulation, use:
```bash
make
./ElectricalTransport
```
This will read the workflow in PPTaskGenResult.txt, then print the result on terminal.