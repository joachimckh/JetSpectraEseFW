add_executable(plot-data-no-correction plotDataNoCorrection.cxx)
target_link_libraries(plot-data-no-correction PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(plot-data-no-correction PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)