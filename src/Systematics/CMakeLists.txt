add_executable(sysEPFT0C sysEPFT0C.cxx)
target_link_libraries(sysEPFT0C PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(sysEPFT0C PRIVATE ${CMAKE_SOURCE_DIR}/src/Core/include)

add_executable(sysEvent sysEv.cxx)
target_link_libraries(sysEvent PRIVATE Core ${ROOT_LIBRARIES})
target_include_directories(sysEvent PRIVATE ${CMAKE_SOURCE_DIR}/src/Core/include)