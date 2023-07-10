#!/bin/bash

# 判断build文件夹是否存在，不存在则创建
if [ ! -d "build" ]; then
  mkdir build
fi

# 进入build文件夹
cd build
LIB_PATH=/root/runtime/fastdeploy-linux-aarch64/lib
LIB_NAME=fastdeploy

# 执行cmake和make命令
cmake ..
make -j$(nproc) LDFLAGS="-L${LIB_PATH}" LDLIBS="-l${LIB_NAME}"
