#!/bin/bash
#PBS -N GeothermalBattery
#PBS -l select=30:ncpus=1:mpiprocs=1
#PBS -l place=free:shared
#PBS -l walltime=24:00:00
#PBS -joe
#PBS -ko

JOB_NUM=${PBS_JOBID%%\.*}
cd $PBS_O_WORKDIR

#ln -s $HOME/$PBS_JOBNAME.o$JOB_NUM $PBS_JOBNAME.o$JOB_NUM

FALCON_APP=$HOME/falcon/falcon-opt
INFILE=$HOME/falcon/sweep/mesh_fine.i

NSLOTS=`cat $PBS_NODEFILE | wc -l`
date
echo $NSLOTS
mpiexec -n $NSLOTS $FALCON_APP -i $INFILE
date

#rm $PBS_JOBNAME.o$JOB_NUM
#mv $HOME/$PBS_JOBNAME.o$JOB_NUM $PBS_JOBNAME.o$JOB_NUM

