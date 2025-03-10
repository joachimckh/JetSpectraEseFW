add_library(Core SHARED)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/src/Core/CoreDict.cxx
           ${CMAKE_BINARY_DIR}/src/Core/CoreDict_rdict.pcm
           ${CMAKE_BINARY_DIR}/src/Core/CoreDict.rootmap
    COMMAND ${ROOT_ROOTCLING_EXECUTABLE} -f ${CMAKE_BINARY_DIR}/src/Core/CoreDict.cxx -c
            -I${CMAKE_SOURCE_DIR}/src/Core/include
            -I${HOME_DIR}/alisw/sw/osx_arm64/RooUnfold/latest-o2/include
            ${CMAKE_SOURCE_DIR}/src/Core/include/JEfficiency.hpp
            ${CMAKE_SOURCE_DIR}/src/Core/include/JEFW.hpp
            ${CMAKE_SOURCE_DIR}/src/Core/include/JEUtility.hpp
            ${CMAKE_SOURCE_DIR}/src/Core/include/JEWrapper.hpp
            ${CMAKE_SOURCE_DIR}/src/Core/include/JESys.hpp
            ${CMAKE_SOURCE_DIR}/src/Core/include/CoreLinkDef.hpp
    DEPENDS ${CMAKE_SOURCE_DIR}/src/Core/include/JEfficiency.hpp
            ${CMAKE_SOURCE_DIR}/src/Core/include/JEFW.hpp
            ${CMAKE_SOURCE_DIR}/src/Core/include/JEUtility.hpp
            ${CMAKE_SOURCE_DIR}/src/Core/include/JEWrapper.hpp
            ${CMAKE_SOURCE_DIR}/src/Core/include/JESys.hpp
            ${CMAKE_SOURCE_DIR}/src/Core/include/CoreLinkDef.hpp
)

target_sources(Core PRIVATE ${CMAKE_BINARY_DIR}/src/Core/CoreDict.cxx)
target_include_directories(Core PRIVATE ${CMAKE_SOURCE_DIR}/src/Core/include)
target_link_libraries(Core PRIVATE ${ROOT_LIBRARIES} RooUnfold)

add_subdirectory(src)
