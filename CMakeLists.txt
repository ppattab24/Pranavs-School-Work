cmake_minimum_required(VERSION 2.8)

ADD_SUBDIRECTORY(googletest)

set(CMAKE_CXX_STANDARD 11)

add_executable(test unit_tests/test.cpp)
add_executable(rshell        
	src/parse.cpp
        src/main.cpp
        src/manager.cpp
        src/genuse.cpp
        src/shuntingYard.cpp
        src/token.cpp
	)


TARGET_LINK_LIBRARIES(test gtest)
TARGET_COMPILE_DEFINITIONS(test PRIVATE gtest_disable_pthreads=ON)
