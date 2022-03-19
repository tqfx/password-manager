# include modules
include(${CMAKE_CURRENT_LIST_DIR}/append_option.cmake)
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
