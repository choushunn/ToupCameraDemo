cmake_minimum_required(VERSION 3.14)
# 定义子模块名称
set(MODULE_NAME cdevice)
project(${MODULE_NAME} LANGUAGES CXX)

# 自动
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 定义源文件
FILE(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/include/*.h")

# 设置头文件路径变量
set(${MODULE_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 添加头文件路径
include_directories(${${MODULE_NAME}_INCLUDE_DIR})

# 将头文件路径变量传递给父项目
set(${MODULE_NAME}_INCLUDE_DIR ${${MODULE_NAME}_INCLUDE_DIR} PARENT_SCOPE)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# 生成静态库文件
add_library(${MODULE_NAME} STATIC
    ${SRC_FILES}
    ${HEADER_FILES})

# 添加头文件路径
target_include_directories(${MODULE_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(${MODULE_NAME} PRIVATE ${TOUPCAM_INCLUDE_DIR})
target_link_libraries(${MODULE_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${MODULE_NAME} PRIVATE Qt6::WebSockets)
target_link_libraries(${MODULE_NAME} PRIVATE Qt6::SerialPort)
target_link_libraries(${MODULE_NAME} PRIVATE Qt6::Multimedia)
target_link_libraries(${MODULE_NAME} PRIVATE Qt6::Concurrent)
target_link_libraries(${MODULE_NAME} PRIVATE ${TOUPCAM_LIBRARIES})
target_link_libraries(${MODULE_NAME} PRIVATE ${OpenCV_LIBS})

target_compile_definitions(${MODULE_NAME} PRIVATE ${MODULE_NAME}_LIBRARY)

# 指定库文件输出路径
set_target_properties(${MODULE_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# 安装头文件和库文件
install(DIRECTORY include/ DESTINATION include)
install(TARGETS ${MODULE_NAME} DESTINATION lib)
###################################################################
# 生成动态库文件
add_library(${MODULE_NAME}_dynamic SHARED
    ${SRC_FILES}
    ${HEADER_FILES})
set_target_properties(${MODULE_NAME}_dynamic  PROPERTIES OUTPUT_NAME ${MODULE_NAME})
# 添加头文件路径
target_include_directories(${MODULE_NAME}_dynamic PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(${MODULE_NAME}_dynamic PRIVATE ${TOUPCAM_INCLUDE_DIR})
target_link_libraries(${MODULE_NAME}_dynamic PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt6::WebSockets
    Qt6::SerialPort
    Qt6::Multimedia
    Qt6::Concurrent
    ${TOUPCAM_LIBRARIES}
    ${OpenCV_LIBS})


# 链接依赖库
target_link_libraries(${MODULE_NAME}_dynamic PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    ${FastDeploy_LIBS}
    ${ONNXRUNTIME_LIBS}
)

# 指定库文件输出路径
set_target_properties(${MODULE_NAME}_dynamic PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(${MODULE_NAME}_dynamic  PROPERTIES CLEAN_DIRECT_OUTPUT 1)


# 安装头文件和库文件
install(DIRECTORY include/ DESTINATION include)
install(TARGETS ${MODULE_NAME} ${MODULE_NAME}_dynamic DESTINATION bin)
