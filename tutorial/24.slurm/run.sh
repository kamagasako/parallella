#!/bin/sh

#SBATCH --nodes=2
#SBATCH --ntasks-per-node=2

mpirun ./main

exit 0
