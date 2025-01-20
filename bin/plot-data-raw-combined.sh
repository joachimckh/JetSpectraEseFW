#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <rval>"
  exit 1
fi

rval="$1"


echo "Running plot-data-raw-combined with jet radius=${rval}"
./build/JAnalysis/plot-data-raw-combined "${rval}"
