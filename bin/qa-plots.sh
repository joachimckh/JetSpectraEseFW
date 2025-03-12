#!/bin/bash

path="/Users/joachimcarlokristianhansen/jet_analysis/hyperloop_data/LHC23_PbPb_pass4_goldenRuns/jet/366086/AnalysisResults.root"

./analysis/build/src/Tasks/qa-plots "${path}"