add_library(Core SHARED)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/Core/CoreDict.cxx
           ${CMAKE_BINARY_DIR}/Core/CoreDict_rdict.pcm
           ${CMAKE_BINARY_DIR}/Core/CoreDict.rootmap
    COMMAND ${ROOT_ROOTCLING_EXECUTABLE} -f ${CMAKE_BINARY_DIR}/Core/CoreDict.cxx -c
            ${CMAKE_SOURCE_DIR}/Core/include/JEfficiency.hpp
            ${CMAKE_SOURCE_DIR}/Core/include/JEFW.hpp
            ${CMAKE_SOURCE_DIR}/Core/include/JEUtility.hpp
            ${CMAKE_SOURCE_DIR}/Core/include/CoreLinkDef.hpp
    DEPENDS ${CMAKE_SOURCE_DIR}/Core/include/JEfficiency.hpp
            ${CMAKE_SOURCE_DIR}/Core/include/JEFW.hpp
            ${CMAKE_SOURCE_DIR}/Core/include/JEUtility.hpp
            ${CMAKE_SOURCE_DIR}/Core/include/CoreLinkDef.hpp
)

target_sources(Core PRIVATE ${CMAKE_BINARY_DIR}/Core/CoreDict.cxx)
target_include_directories(Core PRIVATE ${CMAKE_SOURCE_DIR}/Core/include)
target_link_libraries(Core PRIVATE ${ROOT_LIBRARIES} RooUnfold)

add_subdirectory(src)
