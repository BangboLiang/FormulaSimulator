# FormulaSiulator1 F1
Here is an Optical/Electrical transport simulator with task scheduler and simulator.

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