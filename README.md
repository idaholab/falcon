# FALCON

FALCON is a finite-element geothermal reservoir simulation and analysis code for coupled and fully implicit Thermo-Hydro-Mechanical-Chemical (THMC) geosystems based on the MOOSE framework mainly developed by [Idaho National Laboratory](http://www.inl.gov). It solves the coupled governing equations for fluid flow, heat transfer, rock deformation and fracturing, and chemical reactions in geological porous media. Due to the flexibility in the virtual abstract physics and transport interfaces, FALCON is highly extensible and can accommodate both multi-species and multi-phase formulations.

Some of the capability at your fingertips:

* Fully-coupled, fully-implicit solver
* Dimension independent physics
* Automatically parallel (largest runs >100,000 CPU cores!)
* Modular development simplifies code reuse
* Built-in mesh adaptivity
* Dimension agnostic, parallel geometric search (for contact related applications)
* Flexible, plugable graphical user interface

# Citing FALCON

Yidong Xia, Mitchell Plummer, Earl Mattson, Robert Podgorney, and Ahmad Ghassemi. "Design, modeling, and evaluation of a doublet heat extraction model in enhanced geothermal systems." Renewable Energy, 105 (2017): 232-247, DOI: 10.1016/j.renene.2016.12.064.

# Documentation

Please navigate to the folder /doc/publications/ for the papers that employed FALCON for modeling & simulation. 

# Getting Started

Most of the instructions below are applicable to both the Mac OS X and Linux Ubuntu operating systems. Differences will be pointed out when necessary.

## Git Installation and Configuration

It is suggested that developers and/or users download and install the latest stable version of Git at [https://git-scm.com](https://git-scm.com). For information on how to use Git, see [https://git-scm.com/doc](https://git-scm.com/doc).

The first thing you should do after you install Git is to set your user name and e-mail address.

```
$ git config --global user.name "Your Name"
$ git config --global user.email your_email@example.com
```

You can use the `git config --list` command to list all the settings Git can find at that point:

```
$ git config --list
user.name=John Doe
user.email=johndoe@example.com
color.status=auto
color.branch=auto
color.interactive=auto
color.diff=auto
...
```

## GitHub Account

Create a personal account at GitHub [https://github.com/join](https://github.com/join) if you do not have one.

### Generate SSH Keys

The original complete post is on [GitHub Help](https://help.github.com/articles/generating-ssh-keys/#platform-mac), which covers Mac, Linux, and Windows operating systems.

SSH keys are a way to identify trusted computers, without involving passwords. The steps below will walk you through generating an SSH key and adding the public key to your GitHub account.

#### Step 1. Generate a new SSH key (only if you have never done it)

* With Terminal open, copy and paste the text below. Make sure you substitute in your GitHub email address.

```
$ ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
# Creates a new ssh key, using the provided email as a label
# Generating public/private rsa key pair.
```

* We strongly suggest keeping the default settings as they are, so when you're prompted to "Enter a file in which to save the key", just press **Enter** to continue.

```
Enter file in which to save the key (/Users/you/.ssh/id_rsa): [Press enter]
```

* You'll be asked to enter a passphrase, just press **Enter** to continue.

```
Enter passphrase (empty for no passphrase): [Press enter]
Enter same passphrase again: [Press enter]
```

#### Step 2. Add your SSH key to your account

To configure your GitHub account to use your SSH key:

Copy the SSH key (the contents of the id_rsa.pub file) to your clipboard. Warning: It's important to copy the key exactly without adding newlines or whitespace.

* For Mac OS X

```
pbcopy < ~/.ssh/id_rsa.pub
```

* For Linux Ubuntu

```
$ sudo apt-get install xclip
$ xclip -sel clip < ~/.ssh/id_rsa.pub
```

Add the copied key to GitHub:

* In the top right corner of any page, click your profile photo, then click **Settings**.

* In the user settings sidebar, click **SSH keys**.

* Click **Add SSH key**.

* In the Title field, add a descriptive label for the new key. For example, if you're using a personal Mac, you might call this key "Personal MacBook Air".

* Paste your key into the "Key" field.

* Click **Add key**.

> Note that it is a similar way to add your SSH key to your INL's internal GitLab account at [https://hpcgitlab.inl.gov/profile](https://hpcgitlab.inl.gov/profile), if you have one.


## MOOSE Framework
For installation instructions on your personal desktop and/or laptop, see the official website: [http://mooseframework.com/getting-started/](http://mooseframework.com/getting-started/).

If you want to use MOOSE on INL's HPC, you do not need to build libMesh on Falcon, Fission, Quark, etc. The MOOSE team has built libMesh for you on the HPC machines. In other words, all you need to do with MOOSE is to git clone the MOOSE repository on the designated location.

Now suppose you are already in your home directory on the Falcon HPC, and `vi ~/.bash_login`. *Remove* the following if they are already in the file

```
if [[ -f /apps/projects/moose/moose_environment ]]; then
    source /apps/projects/moose/moose_environment
fi

module load moose-dev-gcc 
```

Then copy & paste the following into the file

```
module load use.moose moose-dev-gcc
```

Now you can save and quit the file, and copy & paste the following into the terminal

```
source ~/.bash_login
```

More infomation on how to run your test cases on a compute node on the HPC will be updated soon.

## FALCON

### Clone FALCON

FALCON is hosted on GitHub and can be cloned directly from there using Git. We recommend placing FALCON alongside with MOOSE in the **projects** folder(from your home directory):

```
$ cd ~/projects
$ git clone git@github.com:idaholab/falcon.git 
$ cd ~/projects/falcon
$ git checkout master 
```

> Note: The "*master*" branch of MOOSE is the "*stable*" branch that will only be updated after all tests are passing. This protects you from the day-to-day churn in the FALCON repository.

### Compile and Test It

```
$ cd ~/projects/falcon
$ make -j4
$ ./run_tests
```

If everything is good then all of the tests will pass.


## Setting Environment Variables

Open the system environment configuration file by `vi ~/.bash_profile` in Mac OS X, or `vi ~/.bashrc` in Linux Ubuntu, or `vi ~/.bash_login` your INL's HPC home directory, and add in the following text.

```
#... MOOSE environment settings
if [ -f /opt/moose/environments/moose_profile ]; then
    . /opt/moose/environments/moose_profile
fi

#... Show your Git branch name on the terminal prompt
parse_git_branch() {
git branch 2> /dev/null |sed -e '/^[^*]/d' -e 's/* \(.*\)/ (\1)/'
}
PS1="\[\033[1;34m\][\u]\[\033[1;32m\][\w]\[\033[0m\]\$(parse_git_branch)\[\033[00m\]> "

#... Paths to applications
export PATH=/usr/local/git/bin:$PATH
export PATH=~/projects/trunk/moose/gui:$PATH
export PATH=~/projects/falcon:$PATH
```


## How to Run Your FALCON Test Case in Terminal

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


## How to Visualize Your FALCON Test Results

FALCON's default ExodusII format data files can be read by many open-source post-processing software packages, for example, [ParaView](http://www.paraview.org/), and [VisIt](https://wci.llnl.gov/simulation/computer-codes/visit/).


# Contributing

For information on how to contribute code changes to FALCON please see ...

# Contact

For technical questions: Yidong Xia ([yidong.xia@inl.gov](yidong.xia@inl.gov))

For business outreach: Robert Podgorney ([robert.podgorney@inl.gov](robert.podgorney@inl.gov))
