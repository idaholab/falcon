## Clone FALCON

FALCON is hosted on [GitHub](https://github.com/idaholab/falcon) and should be cloned directly
from there using [git](https://git-scm.com/). We recommend creating the directory, `~/projects`, to put
all of your FALCON related work.

To clone FALCON, run the following commands in a terminal.

```bash
mkdir ~/projects
cd ~/projects
git clone https://github.com/idaholab/falcon.git
cd falcon
git checkout master
git submodule update --init moose
```

## Compile and Test FALCON

After cloning, FALCON should be compiled and tested before use. 

```bash
conda activate moose
cd ~/projects/falcon
make -j 4
./run_tests -j 4
```

!alert note
The number 4 should be replaced with the number of processors you have available on your system.

If the installation was successful you should see most of the tests passing (some tests will be
skipped depending on your system environment).

You now have a working copy of FALCON.

## Update FALCON

FALCON does not use traditional versioning, is under heavy development, and is being updated
continuously. Therefore, it is critical that you continue to update FALCON regularly to get the
latest features. Following are the steps to update FALCON.

First update the MOOSE environment:

```bash
conda activate moose
conda update --all
```

Then, update FALCON using the following commands:

```bash
cd ~/projects/falcon
git fetch origin
git rebase origin/master
git submodule update
```

Compile and test your fresh copy.

```bash
cd ~/projects/falcon
make -j4
./run_tests -j4
```
