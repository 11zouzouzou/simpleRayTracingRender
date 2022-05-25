cmake_minimum_required(VERSION 3.10)
project(SimpleRayTracingRender)
# Set to c++11
set ( CMAKE_CXX_STANDARD 11 )

# Source
set(COMMON_ALL 
 src/public/math/vec3.h)

 set(SIMPLE_MAIN
 ${COMMON_ALL}
 src/SimpleMain.cpp
 )

add_executable(SimpleRayTracingRender ${SIMPLE_MAIN})

# 包含math文件 
include_directories(src/public/math)
# 包含simple文件 
include_directories(src/simple)