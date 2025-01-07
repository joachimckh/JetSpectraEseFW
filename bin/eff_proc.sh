#!/bin/bash
fRun="284907"
while [[ "$#" -gt 0 ]]; do
  case $1 in
    --fRun) nEvents="$2"; shift ;; 
    *) echo "Unknown parameter passed: $1"; exit 1 ;;
  esac
  shift
done

echo "Running eff_proc with run=${fRun}"
./build/Tasks/eff-proc "${fRun}"
