#!/usr/bin/env sh
echo "USAGE: ./check_install_modules.sh <sudopw>"

mkdir -p ./auto_test

for IMOD in `ls ./install_templates/*.igui`;
do
 
    TNAME=`basename $IMOD .igui`
    (nc -q 0 -l 33334 > auto_test/$TNAME.gui)&

    sh $IMOD "/home/mjoppich/.local/share/bioGUI" "$1" "1" "127.0.0.1" "33334"

done