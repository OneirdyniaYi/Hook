cmake_minimum_required(VERSION 3.1)
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)
project(Hook)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations -Wno-reorder")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations -Wno-reorder")
# linux  -rdynamic

set(LIB_SRC
    hook/log.cc
    )

add_library(Hook_lib SHARED ${LIB_SRC})
#add_library(Hook_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (Hook_static PROPERTIES OUTPUT_NAME "Hook_lib")

add_executable(testlog tests/test.cc)
add_dependencies(testlog Hook_lib)
target_link_libraries(testlog Hook_lib)

SET(EXECUTABLE_OUTPUT_PATH ${PROJIECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJIECT_SOURCE_DIR}/lib)
