add_executable(eff-proc eff_proc.cxx)
target_include_directories(eff-proc PRIVATE ${JETCORE_DIR}/include)

add_executable(data-proc data_proc.cxx)
target_include_directories(data-proc PRIVATE ${JETCORE_DIR}/include)

add_executable(unfold-proc unfold_proc.cxx)
target_include_directories(unfold-proc PRIVATE ${JETCORE_DIR}/include)

add_executable(v2-proc v2_proc.cxx)
target_include_directories(v2-proc PRIVATE ${JETCORE_DIR}/include)

add_executable(background background.cxx)
target_include_directories(background PRIVATE ${JETCORE_DIR}/include)

add_executable(compare-pt-yield compare_spectra.cxx)
target_include_directories(compare-pt-yield PRIVATE ${JETCORE_DIR}/include)

add_executable(compare-pt-rhophi compare-rhophi.cxx)
target_include_directories(compare-pt-rhophi PRIVATE ${JETCORE_DIR}/include)

add_executable(compare-online-offline compare-offline-online.cxx)
target_include_directories(compare-online-offline PRIVATE ${JETCORE_DIR}/include)

add_executable(qa-plots makeQA.cxx)
target_include_directories(qa-plots PRIVATE ${JETCORE_DIR}/include)




target_link_libraries(eff-proc PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES} RooUnfold)
target_link_libraries(data-proc PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(unfold-proc PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES} RooUnfold)
target_link_libraries(v2-proc PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(background PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(compare-pt-yield PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(compare-pt-rhophi PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(compare-online-offline PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(qa-plots PRIVATE ${JETCORE_LIBRARIES} ${ROOT_LIBRARIES})