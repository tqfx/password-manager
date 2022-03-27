if("${VCPKG_TARGET_TRIPLET}" MATCHES "x86-mingw(.*)")
  include(cmake/mingw32.cmake)
elseif("${VCPKG_TARGET_TRIPLET}" MATCHES "x64-mingw(.*)")
  include(cmake/mingw64.cmake)
endif()

cmake_minimum_required(VERSION 3.10)

project(mp VERSION 0.1.1 LANGUAGES C)

add_subdirectory(lib)
include(cmake/check.cmake)
include(cmake/standard.cmake)

if("${CMAKE_C_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR
  "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
  append_flag(-Wall -Wextra -Wpedantic)
  check_flag(-Wdouble-promotion)
  check_flag(-Wnull-dereference)
  check_flag(-Wmissing-include-dirs)
  check_flag(-Wswitch-default -Wswitch-enum)
  check_flag(-Walloca)
  check_flag(-Wattributes)
  check_flag(-Wfloat-equal)
  check_flag(-Wshadow)
  check_flag(-Wundef)
  check_flag(-Wunused -Wunused-macros)
  check_flag(-Wcast-qual -Wcast-align)
  check_flag(-Wconversion)
  check_flag(-Wdate-time)
  check_flag(-Waggregate-return)
  check_flag(-Wmissing-declarations)
  check_flag(-Wpacked -Wpadded)
  check_flag(-Wredundant-decls)
  check_flag(-Winline)
  check_flag(-Winvalid-pch)
  check_flag(-Wdisabled-optimization)
  check_flag(-Wstack-protector)
  check_flag_cc(-Winit-self)
  check_flag_cc(-Wc++-compat)
  check_flag_cc(-Wbad-function-cast)
  check_flag_cc(-Wstrict-prototypes)
  check_flag_cc(-Wold-style-definition)
  check_flag_cc(-Wmissing-prototypes)
  check_flag_cc(-Wnested-externs)
elseif("${CMAKE_C_COMPILER_ID}" MATCHES "MSVC" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  append_flag(/nologo /W4 /sdl)
  if("${VCPKG_TARGET_TRIPLET}" MATCHES "(.*)windows-static(.*)")
    value_remove(CMAKE_CXX_FLAGS_DEBUG /MDd)
    value_append(CMAKE_CXX_FLAGS_DEBUG /MTd)
    value_remove(CMAKE_CXX_FLAGS_RELEASE /MD)
    value_append(CMAKE_CXX_FLAGS_RELEASE /MT)
    value_remove(CMAKE_CXX_FLAGS_MINSIZEREL /MD)
    value_append(CMAKE_CXX_FLAGS_MINSIZEREL /MT)
    value_remove(CMAKE_CXX_FLAGS_RELWITHDEBINFO /MD)
    value_append(CMAKE_CXX_FLAGS_RELWITHDEBINFO /MT)
  endif()
endif()
# GNU
if("${CMAKE_C_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  check_flag(-Wstringop-overflow -Wstringop-truncation)
  check_flag(-Walloc-zero)
  check_flag(-Wduplicated-branches -Wduplicated-cond)
  check_flag(-Wzero-length-bounds)
  check_flag(-Wtrampolines)
  check_flag(-Wunsafe-loop-optimizations)
  check_flag(-Wno-pedantic-ms-format)
  check_flag(-Wlogical-op)
endif()
# Clang
if("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
  check_flag(-Wshadow-all)
  check_flag(-Wzero-length-array)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
elseif("${CMAKE_BUILD_TYPE}" MATCHES "[Dd]ebug" AND UNIX)
  if("${CMAKE_C_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR
    "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    append_flag(-fno-omit-frame-pointer -fsanitize=address)
    append_flag(-fsanitize=undefined)
    if(NOT ("${CMAKE_C_COMPILER_ID}" MATCHES "Apple[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Apple[Cc]lang"))
      append_flag(-fsanitize=leak)
    endif()
  elseif("${CMAKE_C_COMPILER_ID}" MATCHES "MSVC" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    append_flag(/fsanitize=address)
  endif()
endif()

if(NOT ANDROID)
  foreach(lang ${enabled_languages})
    if(CMAKE_${lang}_LINK_OPTIONS_NO_PIE)
      value_append(CMAKE_${lang}_LINK_FLAGS ${CMAKE_${lang}_LINK_OPTIONS_NO_PIE})
    endif()
  endforeach()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

include(CheckIncludeFiles)
check_include_files(getopt.h HAS_GETOPT_H)
if (NOT HAS_GETOPT_H)
  find_path(GETOPT_INCLUDE_DIRS NAMES getopt.h)
  find_library(GETOPT_LIBRARIES NAMES getopt)
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
if (NOT HAS_GETOPT_H)
  target_include_directories(m PRIVATE ${GETOPT_INCLUDE_DIRS})
  target_link_libraries(m ${GETOPT_LIBRARIES})
endif()

cmake_policy(SET CMP0077 NEW)
option(BUILD_TESTING "Enable building tests" OFF)
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
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
