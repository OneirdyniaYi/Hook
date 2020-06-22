cmake_minimum_required(VERSION 3.1)
project(Hook)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")


set(LIB_SRC
    logger.cpp
    )

add_library(Hook_lib SHARED ${LIB_SRC})
#add_library(Hook_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (Hook_static PROPERTIES OUTPUT_NAME "Hook_lib")

add_executable(testlog tests/test.cc)
add_dependencies(testlog Hook_lib)
target_link_libraries(testlog Hook_lib)

SET(EXECUTABLE_OUTPUT_PATH ${PROJIECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJIECT_SOURCE_DIR}/lib)
