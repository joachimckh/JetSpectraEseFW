add_executable(plot-data-raw plotDataRaw.cxx)
target_link_libraries(plot-data-raw PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(plot-data-raw PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(event-plate-res eventPlaneResolution.cxx)
target_link_libraries(event-plate-res PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(event-plate-res PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(plot-data-raw-combined plotDataRawCombined.cxx)
target_link_libraries(plot-data-raw-combined PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(plot-data-raw-combined PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(plot-data-raw-combined-new plotDataRawCombined_new.cxx)
target_link_libraries(plot-data-raw-combined-new PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(plot-data-raw-combined-new PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(pt-raw-inclusive ptSpectraRawNoEse.cxx)
target_link_libraries(pt-raw-inclusive PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(pt-raw-inclusive PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(various-plots various.cxx)
target_link_libraries(various-plots PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(various-plots PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(correction-steps eventPlaneCorrectionSteps.cxx)
target_link_libraries(correction-steps PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(correction-steps PRIVATE)

add_executable(compare-v2 jetv2.cxx)
target_link_libraries(compare-v2 PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(compare-v2 PRIVATE)
                          
