#!/bin/bash
fRun="325738"
A="FT0C"
B="TPCpos"
C="TPCneg"

echo "Running event-plate-res with run=${fRun} and A=${A} B=${B} C=${C}"
./build/JAnalysis/event-plate-res "${fRun}" "${A}" "${B}" "${C}"
