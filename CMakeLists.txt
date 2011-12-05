cmake_minimum_required(VERSION 2.8)

file(GLOB HEADERS "*.h")

# Enable OpenMP in UNIX.
# Enable multicore builds in MSVC.
find_package(OpenMP REQUIRED)
if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
else(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif(UNIX)

# Library targets:
#   sudokill_core - shared objects for sudokill
project(sudokill_core)
set(SRCS
    "sudokill_core.cpp")
add_library(sudokill_core STATIC ${SRCS} ${HEADERS})

## Executable targets:
##   sudokill - the main solution
##   sudokill_gtest - all tests
#
#project(sudokill)
#set(SRCS
#    "sudokill.cpp")
#add_executable(sudokill ${SRCS} ${HEADERS})
#target_link_libraries(sudokill sudokill_core)
#if(WIN32)
#	target_link_libraries(sudokill Ws2_32)
#endif(WIN32)

if(HPS_GTEST_ENABLED)
  project(sudokill_gtest)
  set(SRCS
      "sudokill_gtest.cpp")
  include_directories(${GTEST_INCLUDE_DIRS})
  add_executable(sudokill_gtest ${SRCS} ${HEADERS})
  target_link_libraries(sudokill_gtest sudokill_core gtest)
  add_test(sudokill_gtest sudokill_gtest)
endif(HPS_GTEST_ENABLED)

project(sudokill_all)
