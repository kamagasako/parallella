#!/bin/sh

ESDK=${EPIPHANY_HOME}
ELIBS=${ESDK}/tools/host/lib
EINCS=${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/fast.ldf

gcc main.c -o main -I${EINCS} -L${ELIBS} -le-hal

exit 0
