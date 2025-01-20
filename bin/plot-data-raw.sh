#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <rval>"
  exit 1
fi

rval="$1"

centmin="30"
centmax="50"

qmin="0"
qmax="100"
reso="0.606452"
echo "Running plot-data-raw with q2min=${qmin}, q2max=${qmax}, centmin=${centmin}, centmax=${centmax}, jet Radius=${rval}"
./build/JAnalysis/plot-data-raw "${qmin}" "${qmax}" "${centmin}" "${centmax}" "${rval}" "${reso}"

qmin="0"
qmax="30"
reso="0.54"
echo "Running plot-data-raw with q2min=${qmin}, q2max=${qmax}, centmin=${centmin}, centmax=${centmax}, jet Radius=${rval}"
./build/JAnalysis/plot-data-raw "${qmin}" "${qmax}" "${centmin}" "${centmax}" "${rval}" "${reso}"

qmin="70"
qmax="100"
reso="0.68"
echo "Running plot-data-raw with q2min=${qmin}, q2max=${qmax}, centmin=${centmin}, centmax=${centmax}, jet Radius=${rval}"
./build/JAnalysis/plot-data-raw "${qmin}" "${qmax}" "${centmin}" "${centmax}" "${rval}" "${reso}"
