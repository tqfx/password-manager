cmake_minimum_required(VERSION 3.10)

if(DEFINED ENV{VCPKG_INSTALLATION_ROOT} AND VCPKG_TARGET_TRIPLET)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()
if("${VCPKG_TARGET_TRIPLET}" MATCHES "x86-mingw(.*)")
  include(cmake/mingw32.cmake)
elseif("${VCPKG_TARGET_TRIPLET}" MATCHES "x64-mingw(.*)")
  include(cmake/mingw64.cmake)
endif()
find_program(INCLUDE_WHAT_YOU_USE include-what-you-use)
find_program(CLANG_FORMAT clang-format)
find_program(CLANG_TIDY clang-tidy)
find_program(CPPCHECK cppcheck)
find_program(CPPLINT cpplint)
include(cmake/core.cmake)

project(m VERSION 0.1.1 LANGUAGES C DESCRIPTION "Password Manager" HOMEPAGE_URL "https://github.com/tqfx/password-manager.git")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Specifies the build type on single-configuration generators." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()
include(cmake/standard.cmake)

option(ENABLE_DOXYGEN "Enable doxygen" ON)
option(ENABLE_WARNINGS "Enable warnings" OFF)
option(ENABLE_SANITIZE "Enable sanitize" OFF)
option(ENABLE_CPPCHECK "Enable cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
option(ENABLE_IYWU "Enable include-what-you-use" OFF)
option(ENABLE_IPO "Enable interprocedural optimization" OFF)

if(ENABLE_DOXYGEN)
  find_package(Doxygen OPTIONAL_COMPONENTS dot mscgen dia)
endif()
if(ENABLE_WARNINGS)
  include(cmake/warnings.cmake)
endif()
if(ENABLE_SANITIZE)
  include(cmake/sanitize.cmake)
endif()
if(ENABLE_IPO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ENABLE_IPO)
endif()

if(CMAKE_TOOLCHAIN_FILE AND VCPKG_TARGET_TRIPLET)
  find_package(unofficial-sqlite3)
  find_package(cJSON)
endif()

add_subdirectory(lib)

include(CheckIncludeFiles)
check_include_files(getopt.h HAS_GETOPT_H)
if(NOT HAS_GETOPT_H)
  find_path(GETOPT_INCLUDE_DIRS NAMES getopt.h)
  find_library(GETOPT_LIBRARIES NAMES getopt)
endif()

file(GLOB SOURCE src/*.*)
file(GLOB HEADER include/*.*)
add_library(${PROJECT_NAME} ${SOURCE} ${HEADER})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES C_VISIBILITY_PRESET hidden)
if(ENABLE_IYWU AND INCLUDE_WHAT_YOU_USE)
  set_target_properties(${PROJECT_NAME} PROPERTIES C_INCLUDE_WHAT_YOU_USE "${INCLUDE_WHAT_YOU_USE}")
endif()
if(ENABLE_CLANG_TIDY AND CLANG_TIDY)
  set_target_properties(${PROJECT_NAME} PROPERTIES C_CLANG_TIDY "${CLANG_TIDY};--fix")
endif()
if(ENABLE_CPPCHECK AND CPPCHECK)
  set_target_properties(${PROJECT_NAME} PROPERTIES C_CPPCHECK "${CPPCHECK};--enable=warning,performance")
endif()
if(ENABLE_IPO)
  set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
endif()
target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<C_COMPILER_ID:GNU,Clang,AppleClang,IntelLLVM>:-Wall -Wextra -Wpedantic>
  $<$<C_COMPILER_ID:MSVC>:/W4 /sdl> $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>
  )
if(MINGW)
  target_link_options(${PROJECT_NAME} PUBLIC
    $<$<BOOL:${cJSON_FOUND}>:-fstack-protector-strong>
    -static-libgcc
    -static-libstdc++
    -Wl,-Bstatic,--whole-archive
    -lwinpthread
    -Wl,--no-whole-archive
    )
endif()
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/lib>
  $<INSTALL_INTERFACE:include>
  )
target_link_libraries(${PROJECT_NAME} PUBLIC
  liba::a unofficial::sqlite3::sqlite3 cjson
  )

file(GLOB_RECURSE SOURCE src/cli/*.*)

add_executable(cli ${SOURCE})
set_target_properties(cli PROPERTIES OUTPUT_NAME m)
set_target_properties(cli PROPERTIES C_VISIBILITY_PRESET hidden)
if(ENABLE_IYWU AND INCLUDE_WHAT_YOU_USE)
  set_target_properties(cli PROPERTIES C_INCLUDE_WHAT_YOU_USE "${INCLUDE_WHAT_YOU_USE}")
endif()
if(ENABLE_CLANG_TIDY AND CLANG_TIDY)
  set_target_properties(cli PROPERTIES C_CLANG_TIDY "${CLANG_TIDY};--fix")
endif()
if(ENABLE_CPPCHECK AND CPPCHECK)
  set_target_properties(cli PROPERTIES C_CPPCHECK "${CPPCHECK};--enable=warning,performance")
endif()
if(ENABLE_IPO)
  set_target_properties(cli PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)
endif()
target_compile_options(cli PRIVATE
  $<$<C_COMPILER_ID:GNU,Clang,AppleClang,IntelLLVM>:-Wall -Wextra -Wpedantic>
  $<$<C_COMPILER_ID:MSVC>:/W4 /sdl> $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>
  )
target_link_options(cli PRIVATE $<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>)
target_compile_definitions(cli PRIVATE _POSIX_C_SOURCE=200809L)
target_link_libraries(cli PRIVATE ${PROJECT_NAME})
if(NOT HAS_GETOPT_H)
  target_include_directories(cli PRIVATE ${GETOPT_INCLUDE_DIRS})
  target_link_libraries(cli PRIVATE ${GETOPT_LIBRARIES})
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
  DESTINATION include FILES_MATCHING
  REGEX ".*\.\(h\|hh\|hxx\|hpp\)$"
  )
install(TARGETS ${PROJECT_NAME} cli
  EXPORT ${PROJECT_NAME}-targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )
install(EXPORT ${PROJECT_NAME}-targets
  FILE ${PROJECT_NAME}-config.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
  )

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_LIST_DIR}/LICENSE.txt)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_LIST_DIR}/README.md)
set(CPACK_SOURCE_IGNORE_FILES
  ${CMAKE_CURRENT_LIST_DIR}/.git
  ${CMAKE_CURRENT_LIST_DIR}/.vscode
  ${CMAKE_CURRENT_LIST_DIR}/.github
  ${CMAKE_CURRENT_LIST_DIR}/.gitignore
  ${CMAKE_CURRENT_LIST_DIR}/.gitmodules
  ${CMAKE_CURRENT_LIST_DIR}/.gitattributes
  ${CMAKE_CURRENT_LIST_DIR}/build
  ${CMAKE_CURRENT_LIST_DIR}/html
  )
set(CPACK_GENERATOR ZIP)
include(CPack)

if(CLANG_FORMAT)
  file(GLOB_RECURSE SOURCE include/*.h src/*.[ch] tests/*.[ch]*)
  # https://clang.llvm.org/docs/ClangFormat.html
  add_custom_target(${PROJECT_NAME}-format
    COMMAND ${CLANG_FORMAT} -style=file -i ${SOURCE} --verbose
    )
endif()
if(DOXYGEN_FOUND)
  # Project related configuration options
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
    "__STDC_VERSION__=201112L"
    "__cplusplus=201703L"
    "A_RESTRICT:="
    "A_INLINE:="
    "A_PUBLIC:="
    "A_HIDDEN:="
    "A_ASM:="
    )
  # Configuration options related to external references
  # Configuration options related to the dot tool
  if(Doxygen_dot_FOUND)
    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_INTERACTIVE_SVG YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
  endif()
  # https://www.doxygen.nl/manual/config.html
  doxygen_add_docs(${PROJECT_NAME}-doc
    ${CMAKE_CURRENT_LIST_DIR}/README.md
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
    )
endif()
