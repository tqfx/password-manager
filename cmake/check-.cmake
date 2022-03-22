# include modules
include(${CMAKE_CURRENT_LIST_DIR}/value.cmake)
if("${CMAKE_C_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR
  "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
  append_flag(-Wall -Wextra -Wpedantic)
  append_flag(-Wdouble-promotion)
  append_flag(-Wnull-dereference)
  append_flag(-Wmissing-include-dirs)
  append_flag(-Wswitch-default -Wswitch-enum)
  append_flag(-Walloca)
  append_flag(-Wattributes)
  append_flag(-Wfloat-equal)
  append_flag(-Wshadow)
  append_flag(-Wundef)
  append_flag(-Wunused -Wunused-macros)
  append_flag(-Wcast-qual -Wcast-align)
  append_flag(-Wconversion)
  append_flag(-Wdate-time)
  append_flag(-Waggregate-return)
  append_flag(-Wmissing-declarations)
  append_flag(-Wpacked -Wpadded)
  append_flag(-Wredundant-decls)
  append_flag(-Winline)
  append_flag(-Winvalid-pch)
  append_flag(-Wdisabled-optimization)
  append_flag(-Wstack-protector)
  append_flag_cc(-Winit-self)
  append_flag_cc(-Wc++-compat)
  append_flag_cc(-Wbad-function-cast)
  append_flag_cc(-Wstrict-prototypes)
  append_flag_cc(-Wold-style-definition)
  append_flag_cc(-Wmissing-prototypes)
  append_flag_cc(-Wnested-externs)
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
