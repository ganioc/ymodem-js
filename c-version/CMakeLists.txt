
CMAKE_MINIMUM_REQUIRED(VERSION 3.5 FATAL_ERROR)

project(ymodem_tx LANGUAGES C)

#target_include_directories()

##############设置目标文件生成位置#####################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

set (RUNTIME_INSTALL_DIR bin)
set (LIBRARY_INSTALL_DIR lib)
set (INCLUDE_INSTALL_DIR include)

#添加头文件包含路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}/inc")
#添加src子目录
#add_subdirectory(src)
#添加根目录的源文件
aux_source_directory(. _SOURCES)
#添加src子目录所有源文件
aux_source_directory(src _SUB_SOURCES)

add_executable(tx ${_SOURCES} ${_SUB_SOURCES})

