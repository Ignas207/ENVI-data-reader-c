cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(envi-test)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
enable_language(C CXX)

string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

add_definitions("-DLOG_ENABLE_INFO_MSG")
add_definitions("-DLOG_ENABLE_EVENT_MSG")
add_definitions("-DLOG_ENABLE_ERROR_MSG")

set(OPENCV_MAIN_DIR "C:/opencv")

if(NOT WIN32)
    find_package(OpenCV REQUIRED)
    message(STATUS "Found OpenCV in system!")
elseif(WIN32)
    message(STATUS "Trying to add OpenCV in \"${OPENCV_MAIN_DIR}\"")

    set(OpenCV_DIR ${OPENCV_MAIN_DIR}/build/x64/vc16/lib)
    FIND_PACKAGE(OpenCV REQUIRED)

    set(OpenCV_LIB_VERSION ${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH})
endif()

add_executable(${CMAKE_PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/src/envi-data-handling.c
    ${CMAKE_SOURCE_DIR}/src/envi-data-reading.c
    ${CMAKE_SOURCE_DIR}/src/envi-hdr-parsing.c
    ${CMAKE_SOURCE_DIR}/main.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/inc
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${OpenCV_LIBS}
)

if(WIN32)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${OPENCV_MAIN_DIR}/build/x64/vc16/bin/opencv_world${OpenCV_LIB_VERSION}.dll
        ${CMAKE_SOURCE_DIR}/build/opencv_world${OpenCV_LIB_VERSION}.dll
    )

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${OPENCV_MAIN_DIR}/build/x64/vc16/bin/opencv_world${OpenCV_LIB_VERSION}d.dll
        ${CMAKE_SOURCE_DIR}/build/opencv_world${OpenCV_LIB_VERSION}d.dll
    )
endif()

