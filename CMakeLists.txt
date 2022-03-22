if(${CMAKE_TOOLCHAIN_FILE})
  include(${CMAKE_TOOLCHAIN_FILE})
endif()
if("${VCPKG_TARGET_TRIPLET}" MATCHES "x86-mingw(.*)")
  include(cmake/mingw32.cmake)
elseif("${VCPKG_TARGET_TRIPLET}" MATCHES "x64-mingw(.*)")
  include(cmake/mingw64.cmake)
endif()

cmake_minimum_required(VERSION 3.10)

project(manager VERSION 0.1.1 LANGUAGES C)

include(cmake/check.cmake OPTIONAL)
include(cmake/output.cmake OPTIONAL)
include(cmake/standard.cmake OPTIONAL)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
elseif("${CMAKE_BUILD_TYPE}" MATCHES "[Dd]ebug")
  if(UNIX)
    include(cmake/sanitizer.cmake OPTIONAL)
  endif()
endif()

add_subdirectory(deps)

include(CheckIncludeFiles)
check_include_files(getopt.h HAVE_GETOPT_H)
if (NOT HAVE_GETOPT_H)
  find_path(getopt_INCLUDE_DIRS NAMES getopt.h)
  find_library(getopt_LIBRARIES NAMES getopt)
endif()
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(cJSON CONFIG REQUIRED)

unset(SOURCE)
aux_source_directory(src SOURCE)
add_library(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} a
  unofficial::sqlite3::sqlite3
  cjson
  )
if(MINGW)
  target_link_options(${PROJECT_NAME} INTERFACE
    -fstack-protector-strong
    )
endif()
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src
  )
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:include>
  )

unset(SOURCE)
aux_source_directory(src/cli SOURCE)
add_executable(m ${SOURCE})
target_link_libraries(m ${PROJECT_NAME})
if (NOT HAVE_GETOPT_H)
  target_include_directories(m PRIVATE ${getopt_INCLUDE_DIRS})
  target_link_libraries(m ${getopt_LIBRARIES})
endif()

cmake_policy(SET CMP0077 NEW)
option(BUILD_TESTING "Enable building tests" OFF)
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

install(TARGETS ${PROJECT_NAME} m
  EXPORT ${PROJECT_NAME}-target
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )
install(EXPORT ${PROJECT_NAME}-target
  FILE ${PROJECT_NAME}-config.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
  )
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
  DESTINATION include FILES_MATCHING
  REGEX ".*\.\(h\|hh\|hxx\|hpp\)$"
  )

# https://www.doxygen.nl/manual/config.html
find_package(Doxygen OPTIONAL_COMPONENTS dot mscgen dia)
if(DOXYGEN_FOUND)
  # Project related configuration options
  set(DOXYGEN_PROJECT_BRIEF "Password Manager")
  set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
  # Build related configuration options
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_EXTRACT_PRIVATE YES)
  set(DOXYGEN_EXTRACT_STATIC YES)
  # Configuration options related to warning and progress messages
  # Configuration options related to the input files
  set(DOXYGEN_RECURSIVE YES)
  # Configuration options related to source browsing
  set(DOXYGEN_INLINE_SOURCES YES)
  # Configuration options related to the alphabetical class index
  # Configuration options related to the HTML output
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_USE_MATHJAX YES)
  # Configuration options related to the LaTeX output
  # Configuration options related to the RTF output
  # Configuration options related to the man page output
  # Configuration options related to the XML output
  # Configuration options related to the DOCBOOK output
  # Configuration options for the AutoGen Definitions output
  # Configuration options related to Sqlite3 output
  # Configuration options related to the Perl module output
  # Configuration options related to the preprocessor
  set(DOXYGEN_MACRO_EXPANSION YES)
  set(DOXYGEN_EXPAND_ONLY_PREDEF YES)
  set(DOXYGEN_PREDEFINED
    "__cplusplus"
    "__BEGIN_DECLS:="
    "__END_DECLS:="
    "__NONNULL():="
    "__NONNULL_ALL:="
    "__RESULT_USE_CHECK:="
    "__ALWAYS_INLINE:="
    "__WEAK:="
    "__USED:="
    "__UNUSED:="
    "__ALIGNED():="
    "__PACKED:="
    "__PACKED_STRUCT:="
    "__PACKED_UNION:="
    "__ASM:="
    "__INLINE:="
    "__RESTRICT:="
    "__STATIC_INLINE:="
    "__ATTR_PRINTF():="
    )
  # Configuration options related to external references
  # Configuration options related to the dot tool
  if(Doxygen_dot_FOUND)
    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_INTERACTIVE_SVG YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
  endif()
  doxygen_add_docs(doc_${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/README.md
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
    )
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_LIST_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_LIST_DIR}/README.md)
set(CPACK_SOURCE_IGNORE_FILES
  ${CMAKE_CURRENT_LIST_DIR}/.git
  ${CMAKE_CURRENT_LIST_DIR}/.vscode
  ${CMAKE_CURRENT_LIST_DIR}/.github
  ${CMAKE_CURRENT_LIST_DIR}/.gitignore
  ${CMAKE_CURRENT_LIST_DIR}/.gitmodules
  ${CMAKE_CURRENT_LIST_DIR}/.clang-format
  ${CMAKE_CURRENT_LIST_DIR}/.gitattributes
  ${CMAKE_CURRENT_LIST_DIR}/build
  )
set(CPACK_GENERATOR ZIP)
include(CPack)
