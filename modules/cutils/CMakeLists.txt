cmake_minimum_required(VERSION 3.14)
# 定义子模块名称
set(MODULE_NAME connx)
project(${MODULE_NAME})

# C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 自动
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 定义源文件
FILE(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/include/*.h")

# 设置头文件路径变量
set(${MODULE_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 添加头文件路径
include_directories(${${MODULE_NAME}_INCLUDE_DIR})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)


# 生成库文件
add_library(${MODULE_NAME} STATIC
    ${SRC_FILES}
    ${HEADER_FILES})

# 链接依赖库
target_link_libraries(${MODULE_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)

# 添加头文件路径
target_include_directories(${MODULE_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(${MODULE_NAME} PUBLIC ${FASTDEPLOY_INCS})

# 指定库文件输出路径
set_target_properties(${MODULE_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)


# 安装头文件和库文件
install(DIRECTORY include/ DESTINATION include)
install(TARGETS ${MODULE_NAME} DESTINATION lib)
