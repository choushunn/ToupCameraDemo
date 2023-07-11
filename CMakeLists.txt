cmake_minimum_required(VERSION 3.5)

# 设置项目名称和版本
project(ToupCameraDemo VERSION 0.1 LANGUAGES CXX)

# 输出项目名称和当前平台/架构信息
message(STATUS "Project Name: " ${PROJECT_NAME})
message(STATUS "current platform/architecture: ${CMAKE_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR}")

# 自动设置UI相关选项
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX install)

# 配置库文件路径
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(THIRD_PARTY_LIB_PATH "F:/share/runtime/win")
#    set(THIRD_PARTY_LIB_PATH "D:\\Projects\\runtime")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(THIRD_PARTY_LIB_PATH "/root/runtime")
else()
    message(FATAL_ERROR "Unsupported current platform: ${CMAKE_SYSTEM_NAME}")
endif()


# 依赖库路径设置
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # ToupCam
    set(ToupCam_DIR "${THIRD_PARTY_LIB_PATH}/toupcam")
    # FastDeploy
    set(FastDeploy_DIR "${THIRD_PARTY_LIB_PATH}/fastdeploy-win-x64-gpu")
    # ncnn
    set(ncnn_DIR "${THIRD_PARTY_LIB_PATH}/ncnn/x64/lib/cmake/ncnn")
    # onnxruntime
    set(OnnxRuntime_DIR "${FastDeploy_DIR}/third_libs/install/onnxruntime")
    # OpenCV
    set(OpenCV_DIR "${FastDeploy_DIR}/third_libs/install/opencv/build")
    # CUDA
    set(CUDA_DIRECTORY "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.8")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
        # x86_64版本
        set(LIB_ARCH x86_64)
    elseif(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64")
        # arm64版本
        set(LIB_ARCH aarch64)
        # ToupCam 系统内置，不用打开
        # set(ToupCam_DIR "${THIRD_PARTY_LIB_PATH}/toupcam/lib/cmake/toupcam")

        # FastDeploy
        set(FastDeploy_DIR "${THIRD_PARTY_LIB_PATH}/fastdeploy-install")
        # ncnn
        set(ncnn_DIR "${THIRD_PARTY_LIB_PATH}/ncnn/x64/lib/cmake/ncnn")
        # onnxruntime
        set(OnnxRuntime_DIR "${FastDeploy_DIR}/third_libs/install/onnxruntime")
        # OpenCV
        set(OpenCV_DIR "${FastDeploy_DIR}/third_libs/install/opencv/share/OpenCV")
    else()
        # 不支持的处理器架构
        message(FATAL_ERROR "Unsupported processor architecture: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
    endif()
endif()

# 导入第三方库
find_package(OpenCV REQUIRED)
find_package(ToupCam REQUIRED)
find_package(ncnn REQUIRED)
find_package(OnnxRuntime REQUIRED)
find_package(FastDeploy REQUIRED)

# 查找Qt库
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS WebSockets)

# 添加子目录
add_subdirectory(modules/connx)
add_subdirectory(modules/cdevice)

# 设置头文件目录为include文件夹
include_directories(include)
include_directories(modules/connx/include)
include_directories(modules/cdevice/include)
include_directories(${TOUPCAM_INCLUDE_DIR})
# 添加FastDeploy依赖头文件
include_directories(${FASTDEPLOY_INCS})

# 设置自动生成的ui头文件的输出路径为include目录
set(CMAKE_AUTOUIC_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)
# 设置ui文件搜索路径
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

# 匹配寻找头文件、源文件、UI文件、翻译文件
FILE(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/include/*.h")
FILE(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB_RECURSE UI_FILES "${PROJECT_SOURCE_DIR}/ui/*.ui")
set(TS_FILES "translations/${PROJECT_NAME}_zh_CN.ts")

set(PROJECT_SOURCES
    ${HEADER_FILES}
    ${SRC_FILES}
    ${TS_FILES}
    ${UI_FILES})

# 如果使用Qt6，则使用qt_add_executable生成可执行文件和qm文件
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION # 手动处理清理操作
        ${PROJECT_SOURCES} # 项目源码
    )

    # 生成qm文件
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    # 如果使用Qt5或以下版本，则使用add_executable和qt5_create_translation生成可执行文件和qm文件
    if(ANDROID)
        # 如果是在Android平台上，则使用add_library生成共享库
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )

    else()
        # 否则使用add_executable生成可执行文件
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()

    # 生成qm文件
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# 复制models文件夹到build目录下
file(COPY ${CMAKE_SOURCE_DIR}/models DESTINATION ${CMAKE_BINARY_DIR})

# 链接Qt库
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::SerialPort)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::WebSockets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Concurrent)

# 链接第三方库
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${TOUPCAM_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ncnn)
target_link_libraries(${PROJECT_NAME} PRIVATE ${ONNXRUNTIME_LIBS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FastDeploy_LIBS}) # 添加FastDeploy库依赖
target_link_libraries(${PROJECT_NAME} PRIVATE connx)
target_link_libraries(${PROJECT_NAME} PRIVATE cdevice)
# 链接cdevice库文件

# 添加预处理定义，防止冲突
add_compile_definitions(NOMINMAX)

# 设置Mac OS X的Bundle属性，包括GUI标识符、版本号和短版本号
set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 安装目标可执行文件和库
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

# 复制models文件夹到安装目录下
install(DIRECTORY ${CMAKE_SOURCE_DIR}/models DESTINATION ${CMAKE_INSTALL_PREFIX})

# 如果使用Qt6，则使用qt_finalize_executable进行清理操作
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
