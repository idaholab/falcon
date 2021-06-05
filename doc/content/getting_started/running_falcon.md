# Running FALCON

As an example, you can copy one of the FALCON tests in a new folder

```
$ mkdir ~/projects/problems
$ cp ~/projects/falcon/tests/PT_TH_faust ~/projects/problems
$ cd ~/projects/problems/PT_TH_faust
```

and run the test case either in serial mode

```
falcon-opt -i inp.i
```

or in parallel mode

```
mpirun -np 4 falcon-opt -i inp.i
```

# Visualize Your FALCON Test Results

FALCON's default ExodusII format data files can be read by many open-source post-processing software packages, for example, [ParaView](http://www.paraview.org/), and [VisIt](https://wci.llnl.gov/simulation/computer-codes/visit/).
