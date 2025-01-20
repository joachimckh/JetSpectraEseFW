add_executable(plot-data-raw plotDataRaw.cxx)
target_link_libraries(plot-data-raw PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(plot-data-raw PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)

add_executable(event-plate-res eventPlaneResolution.cxx)
target_link_libraries(event-plate-res PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(event-plate-res PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)

add_executable(plot-data-raw-combined plotDataRawCombined.cxx)
target_link_libraries(plot-data-raw-combined PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(plot-data-raw-combined PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)

add_executable(pt-raw-inclusive ptSpectraRawNoEse.cxx)
target_link_libraries(pt-raw-inclusive PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(pt-raw-inclusive PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)

add_executable(various-plots various.cxx)
target_link_libraries(various-plots PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(various-plots PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)

                          