#!/bin/bash

echo "merge 2 bins"

PROJSTR="lamp421"
echo "${PROJ_STR}"

FILESIZE=$(expr 60 \* 1024)
DATESTR=$(date +%m%d-%H%M)

#OUTPUT="${PROJSTR}-combo-${DATESTR}.bin"
OUTPUT="lamp421-all-0327.bin"

INFILE1="../lampbl421-0327-none.bin"
INFILE2="../lamp421-0322-ota.bin"

INFILE1_SIZE=$(ls -l "${INFILE1}" | awk '{print $5}')
INFILE2_SIZE=$(ls -l "${INFILE2}" | awk '{print $5}')

# 16KB
OFFSET=$(expr 16 \* 1024)

echo "Output file is:" "${OUTPUT}"
echo "Input file 1 is:" "$INFILE1_SIZE"
echo "Input file 2 is:" "$INFILE2_SIZE"
echo "Offset is:" "${OFFSET}"

echo "Create a ${FILESIZE} byte bin"
</dev/zero head -c ${FILESIZE} | tr '\0' '\377' > "${OUTPUT}"

#dd conv=notrunc if="${INFILE1}" of="${OUTPUT}" bs=1 count="${INFILE1_SIZE}" seek=0



dd conv=notrunc if="${INFILE2}" of="${OUTPUT}" bs=1 count="${INFILE2_SIZE}" seek="${OFFSET}"

sync

dd conv=notrunc if="${INFILE1}" of="${OUTPUT}" bs=1 count="${INFILE1_SIZE}" seek=0

sync
