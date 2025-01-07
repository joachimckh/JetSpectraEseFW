add_executable(eff-proc eff_proc.cxx)
target_link_libraries(eff-proc PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(eff-proc PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)

add_executable(data-proc data_proc.cxx)
target_link_libraries(data-proc PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(data-proc PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)

add_executable(event-plate-res eventPlaneResolution.cxx)
target_link_libraries(event-plate-res PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(event-plate-res PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)

add_executable(unfold-proc unfold_proc.cxx)
target_link_libraries(unfold-proc PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(unfold-proc PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)

add_executable(v2-proc v2_proc.cxx)
target_link_libraries(v2-proc PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(v2-proc PRIVATE 
                          ${CMAKE_SOURCE_DIR}/Core/include)

