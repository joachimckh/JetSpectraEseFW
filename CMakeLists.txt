cmake_minimum_required(VERSION 3.18)
project(JetSpectra LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HOME_DIR $ENV{HOME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} -O3")

find_package(ROOT REQUIRED COMPONENTS Core RIO Hist)
include_directories(${ROOT_INCLUDE_DIRS})
set(ROOT_ROOTCLING_EXECUTABLE "${HOME_DIR}/alisw/sw/osx_arm64/ROOT/v6-32-06-alice1-local1/bin/rootcling")


include_directories(${HOME_DIR}/alisw/sw/osx_arm64/RooUnfold/latest-o2/include)
link_directories(${HOME_DIR}/alisw/sw/osx_arm64/RooUnfold/latest-o2/lib)

add_subdirectory(Core)
add_subdirectory(Tasks)
