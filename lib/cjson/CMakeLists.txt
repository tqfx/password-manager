cmake_minimum_required(VERSION 3.10)

project(cjson VERSION 1.7.15 LANGUAGES C
  DESCRIPTION "Ultralightweight JSON parser in ANSI C"
  HOMEPAGE_URL "https://github.com/DaveGamble/cJSON"
  )

add_library(${PROJECT_NAME} cJSON.c)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
  $<INSTALL_INTERFACE:include>
  )

set_target_properties(${PROJECT_NAME} PROPERTIES C_VISIBILITY_PRESET hidden)
target_compile_definitions(${PROJECT_NAME} PUBLIC CJSON_EXPORT_SYMBOLS CJSON_API_VISIBILITY)

install(FILES cJSON.h DESTINATION include)
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )
install(EXPORT ${PROJECT_NAME}-targets
  FILE ${PROJECT_NAME}-targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME}
  )
