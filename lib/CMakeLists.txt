include(FetchContent)
cmake_policy(SET CMP0077 NEW)

set(BUILD_TESTING OFF)

FetchContent_Declare(liba
  GIT_REPOSITORY https://github.com/tqfx/liba.git
  GIT_TAG 0.1.0
  )

FetchContent_MakeAvailable(liba)
