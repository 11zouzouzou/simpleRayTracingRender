# simpleRayTracingRender
simpleRayTracingRender


## build
cd ./build
cmake ../
cmake --build .
./SimpleRayTracingRender > image.ppm

## vs code devlop
1. 若你安装了c/c++ clang command adapter or others compile plugins;你需要设置对应的c++版本我们这里使用c++11(-std=c++11)(根据cmakeLists中使用的版本为准)