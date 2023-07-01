# 检查是否已经包含过该文件
if(NOT TARGET cdevice::cdevice)
    # 查找connx库
    find_package(cdevice REQUIRED)
endif()

# 添加链接选项
target_link_libraries(your_target cdevice::cdevice)

# 添加包含路径
target_include_directories(your_target PRIVATE ${CDEVICE_INCLUDE_DIRS})