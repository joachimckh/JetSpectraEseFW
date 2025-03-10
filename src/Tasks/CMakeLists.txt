add_executable(eff-proc eff_proc.cxx)
target_link_libraries(eff-proc PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(eff-proc PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(data-proc data_proc.cxx)
target_link_libraries(data-proc PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(data-proc PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(unfold-proc unfold_proc.cxx)
target_link_libraries(unfold-proc PRIVATE Core ${ROOT_LIBRARIES} RooUnfold)
target_include_directories(unfold-proc PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(v2-proc v2_proc.cxx)
target_link_libraries(v2-proc PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(v2-proc PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(background background.cxx)
target_link_libraries(background PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(background PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(compare-pt-yield compare_spectra.cxx)
target_link_libraries(compare-pt-yield PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(compare-pt-yield PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(compare-pt-rhophi compare-rhophi.cxx)
target_link_libraries(compare-pt-rhophi PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(compare-pt-rhophi PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(compare-online-offline compare-offline-online.cxx)
target_link_libraries(compare-online-offline PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(compare-online-offline PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(qa-plots makeQA.cxx)
target_link_libraries(qa-plots PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(qa-plots PRIVATE 
                          ${CMAKE_SOURCE_DIR}/src/Core/include)

