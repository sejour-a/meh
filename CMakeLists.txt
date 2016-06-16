cmake_minimum_required(VERSION 2.4)
project(meh)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -o2"                 )
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}  -std=c++11 -o2" )

set(MEH_SOURCE_FILES
        src/meh.cpp     )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(meh ${MEH_SOURCE_FILES})
