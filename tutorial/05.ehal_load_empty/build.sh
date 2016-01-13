#!/bin/sh

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

gcc main.c -o main -I${EINCS} -L${ELIBS} -le-hal -le-loader
e-gcc -T ${ELDF} e-hello.c -o e-hello.elf
e-objcopy --srec-forceS3 --output-target srec e-hello.elf e-hello.srec

exit 0
