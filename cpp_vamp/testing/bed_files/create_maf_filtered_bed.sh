#!/bin/bash

#SBATCH --ntasks 2
# SBATCH --cpus-per-task 1
#SBATCH --mem 0
#SBATCH --time 2-00:15:00
#SBATCH --output=create_prunned_bed_080_maf005.log

input_bed_loc=/nfs/scistore13/robingrp/human_data/geno/chr/
out_bed_loc=/nfs/scistore13/robingrp/human_data/adepope_preprocessing/VAMPJune2022/cpp_VAMP/testing/bed_files

module purge
module load plink/1.90
module list

# thr=099
# thr=095
# thr=090
thr=080
# thr=049
# thr=036

plink --bfile ${input_bed_loc}/ukb22828_UKB_EST_v3_all --maf 0.05 --make-bed --out ${out_bed_loc}/ukb22828_UKB_EST_v3_all_prunned_${thr}_maf_005



