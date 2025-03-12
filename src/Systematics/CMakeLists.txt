add_executable(sysEPFT0C sysEPFT0C.cxx)
target_include_directories(sysEPFT0C PRIVATE ${JETCORE_DIR}/include)



add_executable(sysEvent sysEv.cxx)
target_include_directories(sysEvent PRIVATE ${JETCORE_DIR}/include)



target_link_libraries(sysEPFT0C PRIVATE ${ROOT_LIBRARIES} ${JETCORE_LIBRARIES})
target_link_libraries(sysEvent PRIVATE ${ROOT_LIBRARIES} ${JETCORE_LIBRARIES})
