set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/JEfficiency.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/JEFW.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/JEUtility.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/JEWrapper.cxx
  ${CMAKE_CURRENT_SOURCE_DIR}/JESys.cxx
  ${CMAKE_BINARY_DIR}/JetCoreDict.cxx
)

target_sources(JetCore PRIVATE ${SOURCES})
# target_include_directories(JetCore PUBLIC ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(JetCore PUBLIC ${ROOT_LIBRARIES} RooUnfold)