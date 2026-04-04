#!/bin/bash

backup_name="mineBackup"

common_dir="/mnt/c/Users/bhg08/Desktop/rtos/bc_source/bc and source/SOFTWARE/uCOS-II"
bc_source="$common_dir/SOURCE"
lab1_dir_source="$common_dir/EX1_x86L/BC45/SOURCE"

mode=$1
# /mnt/c/Users/bhg08/Desktop/rtos/bc_source/bc and source/SOFTWARE/uCOS-II/EX1_x86L/BC45/SOURCE/TEST.C
if [ "$mode" = "0" ]; then
    echo "Mine to other."

    mv "$lab1_dir_source/TEST.C" "$lab1_dir_source/TEST_${backup_name}.C"
    mv "$lab1_dir_source/TEST_other.C" "$lab1_dir_source/TEST.C"

    mv "$bc_source/OS_CORE.C" "$bc_source/OS_CORE_${backup_name}.C"
    mv "$bc_source/OS_CORE_other.C" "$bc_source/OS_CORE.C"

    mv "$bc_source/uCOS_II.H" "$bc_source/uCOS_II_${backup_name}.H"
    mv "$bc_source/uCOS_II_other.H" "$bc_source/uCOS_II.H"

elif [ "$mode" = "1" ]; then
    echo "Other to mine."

    mv "$lab1_dir_source/TEST.C" "$lab1_dir_source/TEST_other.C"
    mv "$lab1_dir_source/TEST_${backup_name}.C" "$lab1_dir_source/TEST.C"

    mv "$bc_source/OS_CORE.C" "$bc_source/OS_CORE_other.C"
    mv "$bc_source/OS_CORE_${backup_name}.C" "$bc_source/OS_CORE.C"

    mv "$bc_source/uCOS_II.H" "$bc_source/uCOS_II_other.H"
    mv "$bc_source/uCOS_II_${backup_name}.H" "$bc_source/uCOS_II.H"

else
    echo "Usage: $0 [0|1]"
    echo "0 = mine -> other"
    echo "1 = other -> mine"
    exit 1
fi