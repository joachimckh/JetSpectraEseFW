cmake_minimum_required(VERSION 3.18)
project(JetCoreLibrary LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HOME_DIR $ENV{HOME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} -O3")

find_package(ROOT REQUIRED COMPONENTS RIO Hist)
include_directories(${ROOT_INCLUDE_DIRS})
set(ROOT_ROOTCLING_EXECUTABLE "${HOME_DIR}/alisw/sw/osx_arm64/ROOT/v6-32-06-alice1-local1/bin/rootcling")


include_directories(${HOME_DIR}/alisw/sw/osx_arm64/RooUnfold/latest-o2/include)
link_directories(${HOME_DIR}/alisw/sw/osx_arm64/RooUnfold/latest-o2/lib)

add_library(JetCore SHARED)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/JetCoreDict.cxx
           ${CMAKE_BINARY_DIR}/JetCoreDict_rdict.pcm
           ${CMAKE_BINARY_DIR}/JetCoreDict.rootmap
    COMMAND ${ROOT_ROOTCLING_EXECUTABLE} -f ${CMAKE_BINARY_DIR}/JetCoreDict.cxx -c
            -I${CMAKE_SOURCE_DIR}/include
            -I${HOME_DIR}/alisw/sw/osx_arm64/RooUnfold/latest-o2/include
            ${CMAKE_SOURCE_DIR}/include/JEfficiency.hpp
            ${CMAKE_SOURCE_DIR}/include/JEFW.hpp
            ${CMAKE_SOURCE_DIR}/include/JEUtility.hpp
            ${CMAKE_SOURCE_DIR}/include/JEWrapper.hpp
            ${CMAKE_SOURCE_DIR}/include/JESys.hpp
            ${CMAKE_SOURCE_DIR}/include/CoreLinkDef.hpp
    DEPENDS ${CMAKE_SOURCE_DIR}/include/JEfficiency.hpp
            ${CMAKE_SOURCE_DIR}/include/JEFW.hpp
            ${CMAKE_SOURCE_DIR}/include/JEUtility.hpp
            ${CMAKE_SOURCE_DIR}/include/JEWrapper.hpp
            ${CMAKE_SOURCE_DIR}/include/JESys.hpp
            ${CMAKE_SOURCE_DIR}/include/CoreLinkDef.hpp
)

target_sources(JetCore PRIVATE ${CMAKE_BINARY_DIR}/JetCoreDict.cxx)
target_include_directories(JetCore PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(JetCore PRIVATE ${ROOT_LIBRARIES} RooUnfold)

add_subdirectory(src)
