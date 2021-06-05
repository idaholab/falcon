# Install MOOSE

FALCON is based on, and requires, the [!ac](MOOSE) Framework.
To install [!ac](MOOSE), click the link below that corresponds to your operating system/platform and follow the instructions:

- [Linux and MacOS](getting_started/installation/falcon_conda.md)
- [getting_started/installation/falcon_windows10.md]

!alert note title=Further Instructions
For further and/or more advanced installation instructions, such as how to install on HPC clusters, please see the [instructions](https://mooseframework.inl.gov/getting_started/installation/index.html) on the main [MOOSE] website.

## Clone FALCON

FALCON is hosted on GitHub and can be cloned directly from there using Git. We recommend placing FALCON alongside with MOOSE in the *projects* folder (from your home directory):

```
$ cd ~/projects
$ git clone git@github.com:idaholab/falcon.git
$ cd ~/projects/falcon
$ git checkout master
```

!alert note title=Branches
The "*master*" branch of FALCON is the "*stable*" branch that will only be updated after all tests are passing. This protects you from the day-to-day churn in the FALCON repository.

## Compile and Test FALCON

Once you've cloned FALCON and checked out your branch, compile and test it using:

```
$ cd ~/projects/falcon
$ make -j4
$ ./run_tests
```

If everything is good then all of the tests will pass. Note that there some be some "*heavy*" tests that are skipped by default because they time longer to complete, to run those as well use:

```
$ cd ~/projects/falcon
$ make -j4
$ ./run_tests --all-tests
```

There are many options for run_tests, but the -j option shown above is by far the most widely used. It tells the run_tests script how many processors to utilize on your local machine for running tests concurrently. Put the correct number there (instead of 4). The script will then go find and run your tests and display the results. To see more options for run_tests you can invoke it with -h.

## Update FALCON

FALCON does not use traditional versioning, is always under development, and is being updated on a regular basis. Therefore, it is important that you continue to update FALCON (and MOOSE) as you use it; weekly to monthly updates are recommended.

If you are using the MOOSE Conda environment (recommended), you should always perform an update to both Conda, MOOSE, and FALCON. If you update one, always update the others:

```
conda activate moose
conda update --all
```

To update your MOOSE repository use the following commands.

```
cd ~/projects/moose
git fetch origin
git rebase origin/master
```

Then return to FALCON, re-compile, and test.

```
cd ~/projects/falcon
make clobberall
make -j4
./run_tests -j4
```
