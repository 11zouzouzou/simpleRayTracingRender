cmake_minimum_required(VERSION 3.10)
project(SimpleRayTracingRender 
VERSION 3.0.0
LANGUAGES CXX)
# Set to c++11
set ( CMAKE_CXX_STANDARD 11 )

# Source
set(COMMON_ALL 
 src/public/math/vec3.h
#  src/public/others/external_stb_image.h
#  src/public/others/self_stb_image.h
 )

 set(SIMPLE_MAIN
 ${COMMON_ALL}
 src/SimpleMain.cpp
 )

add_executable(SimpleRayTracingRender ${SIMPLE_MAIN})
add_executable(PI src/pi.cpp ${COMMON_ALL})
add_executable(X_SQ src/intergrate_x_sq.cpp ${COMMON_ALL})
add_executable(SPHERE_IMPORTANCE src/sphere_importance.cpp ${COMMON_ALL})
add_executable(RAYTRACK src/rayTrack/main.cpp ${COMMON_ALL})
add_executable(SPHERE_PLOT src/sphere_plot.cpp ${COMMON_ALL})
add_executable(COS_CUBED src/cos_cubed.cpp ${COMMON_ALL})
add_executable(COS_DENSITY src/cos_density.cpp ${COMMON_ALL})

# 包含math,others文件 
include_directories(src/public/math)
include_directories(src/public/others)
# 包含simple文件 
include_directories(src/simple)