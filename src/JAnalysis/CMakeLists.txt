add_executable(plot-data-raw plotDataRaw.cxx)
target_include_directories(plot-data-raw PRIVATE ${JETCORE_DIR}/include)

add_executable(event-plate-res eventPlaneResolution.cxx)
target_include_directories(event-plate-res PRIVATE ${JETCORE_DIR}/include)

add_executable(plot-data-raw-combined plotDataRawCombined.cxx)
target_include_directories(plot-data-raw-combined PRIVATE ${JETCORE_DIR}/include)

add_executable(plot-data-raw-combined-new plotDataRawCombined_new.cxx)
target_include_directories(plot-data-raw-combined-new PRIVATE ${JETCORE_DIR}/include)

add_executable(pt-raw-inclusive ptSpectraRawNoEse.cxx)
target_include_directories(pt-raw-inclusive PRIVATE ${JETCORE_DIR}/include)

add_executable(various-plots various.cxx)
target_include_directories(various-plots PRIVATE ${JETCORE_DIR}/include)

add_executable(correction-steps eventPlaneCorrectionSteps.cxx)
target_include_directories(correction-steps PRIVATE)

add_executable(compare-v2 jetv2.cxx)
target_include_directories(compare-v2 PRIVATE)
                          
target_link_libraries(plot-data-raw PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES} RooUnfold)
target_link_libraries(event-plate-res PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES} RooUnfold)
target_link_libraries(plot-data-raw-combined PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES} RooUnfold)
target_link_libraries(plot-data-raw-combined-new PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES} RooUnfold)
target_link_libraries(pt-raw-inclusive PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(various-plots PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(correction-steps PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(compare-v2 PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES})