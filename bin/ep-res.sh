#!/bin/bash
fRun="nothing"
while [[ "$#" -gt 0 ]]; do
  case $1 in
    --fRun) nEvents="$2"; shift ;; 
    *) echo "Unknown parameter passed: $1"; exit 1 ;;
  esac
  shift
done

echo "Running event-plate-res with run=${fRun}"
./build/Tasks/event-plate-res "${fRun}"
