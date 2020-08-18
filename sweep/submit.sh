#!/bin/bash
#PBS -N GeothermalBattery
#PBS -l select=500:ncpus=1:mpiprocs=1
#PBS -l place=free:shared
#PBS -l walltime=120:00:00
#PBS -joe
#PBS -ko

module load use.moose
module load PETSc

JOB_NUM=${PBS_JOBID%%\.*}
cd $PBS_O_WORKDIR

#ln -s $HOME/$PBS_JOBNAME.o$JOB_NUM $PBS_JOBNAME.o$JOB_NUM

FALCON_APP=$HOME/projects/falcon/falcon-opt
INFILE=$/projects/geotes/master.i

NSLOTS=`cat $PBS_NODEFILE | wc -l`
date
echo $NSLOTS
mpiexec -n $NSLOTS ~/projects/falcon/falcon-opt -i ./master.i
date

#rm $PBS_JOBNAME.o$JOB_NUM
#mv $HOME/$PBS_JOBNAME.o$JOB_NUM $PBS_JOBNAME.o$JOB_NUM

