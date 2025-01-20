#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <rval>"
  exit 1
fi

rval="$1"

./build/JAnalysis/pt-raw-inclusive "${rval}"
