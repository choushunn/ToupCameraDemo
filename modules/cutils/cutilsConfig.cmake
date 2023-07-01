# 检查是否已经包含过该文件
if(NOT TARGET connx::connx)
    # 查找connx库
    find_package(connx REQUIRED)
endif()

# 添加链接选项
target_link_libraries(your_target connx::connx)

# 添加包含路径
target_include_directories(your_target PRIVATE ${CONNX_INCLUDE_DIRS})