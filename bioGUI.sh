#!/usr/bin/env sh

SCRIPT=$(readlink -f $0)
CURDIR=`dirname $SCRIPT`

echo $CURDIR

LD_LIBRARY_PATH=$CURDIR:$LD_LIBRARY_PATH $CURDIR/bioGUI
