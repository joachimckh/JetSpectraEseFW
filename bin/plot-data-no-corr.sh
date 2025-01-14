#!/bin/bash
qmin="1"
qmax="100"
centmin="30"
centmax="50"
rval="0.2"

echo "Running plot-data-no-correction with q2min=${qmin}, q2max=${qmax}, centmin=${centmin}, centmax=${centmax}, jet Radius=${rval}"
./build/JAnalysis/plot-data-no-correction "${qmin}" "${qmax}" "${centmin}" "${centmax}" "${rval}"
