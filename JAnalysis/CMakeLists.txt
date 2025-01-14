add_executable(plot-data-no-correction plotDataNoCorrection.cxx)
target_link_libraries(plot-data-no-correction PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(plot-data-no-correction PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)

add_executable(event-plate-res eventPlaneResolution.cxx)
target_link_libraries(event-plate-res PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(event-plate-res PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)