cmake_minimum_required(VERSION 3.8)

ADD_SUBDIRECTORY(googletest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/parse.cpp
        header/parse.h
        src/main.cpp
        src/manager.cpp
        header/manager.h
        src/genuse.cpp
        header/genuse.h
        src/shuntingYard.cpp
        header/shuntingYard.h
        src/token.cpp
        header/token.h)

add_executable(mainTest unit_tests/test.cpp)
add_executable(rshell ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(mainTest gtest)
TARGET_COMPILE_DEFINITIONS(mainTest PRIVATE gtest_disable_pthreads=ON)
