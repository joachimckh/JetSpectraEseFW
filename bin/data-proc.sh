#!/bin/bash
fRun="325059"
qmin="70"
qmax="100"
centmin="30"
centmax="50"
rval="0.2"

echo "Running data_proc with run=${fRun}, q2min=${qmin}, q2max=${qmax}, centmin=${centmin}, centmax=${centmax}, jet Radius=${rval}"
./build/Tasks/data-proc "${fRun}" "${qmin}" "${qmax}" "${centmin}" "${centmax}" "${rval}"
