/**
 * @file logs.h
 * @author Ignas207 (https://github.com/Ignas207/simple-c-logging)
 * @brief Header only implementation of logging functions.
 * @version 1.0
 * @date 2024-01-12
 * 
 * @copyright Copyright (c) 2024 MIT License
 * 
 */

#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wstring-plus-int"
#endif

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define LOGS_MAX_LENGHT 150U

/*
    Gets the path RELATIVE to CMakeLists!
    For this to work need to add the following to CMakeLists.txt!
    -------------------------------------------------------------
    string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
    add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")
    -------------------------------------------------------------
*/
#ifndef SOURCE_PATH_SIZE
#define SOURCE_PATH_SIZE 0U
#endif

/**
 * @brief We are using this to get rid of the absolute path of our project.
 *
 */
#define FILENAME__ ((__FILE__) + SOURCE_PATH_SIZE)

#define __LOG_WRITE(type, fileName, lineNumber, fmt, ...)                         \
  do {                                                                            \
                                                                                  \
    int32_t retSize;                                                              \
    char logStr[LOGS_MAX_LENGHT] = {0};                                           \
                                                                                  \
    /* Writing the starting portion */                                            \
    retSize = snprintf(logStr, LOGS_MAX_LENGHT, "%s (%s:%d): ", type,             \
                       fileName, lineNumber);                                     \
                                                                                  \
    /* Writing the VA_ARGS */                                                     \
    snprintf(logStr + retSize, LOGS_MAX_LENGHT - retSize -1, fmt, ##__VA_ARGS__); \
    puts(logStr);                                                                 \
  } while (0)

#ifdef LOG_ENABLE_EVENT_MSG
/**
 * @brief Writes an 'Event' message to the terminal.
 *
 */
#define LOG_EVENT(fmt, ...)                                                    \
  __LOG_WRITE("EVENT", FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_EVENT(fmt, ...)                                                    \
  do {                                                                         \
    (void)fmt;                                                                 \
  } while (0)
#endif

#ifdef LOG_ENABLE_INFO_MSG
/**
 * @brief Writes a 'Info' message to the terminal.
 *
 */
#define LOG_INFO(fmt, ...)                                                     \
  __LOG_WRITE("INFO", FILENAME__, __LINE__, fmt, ##__VA_ARGS__)               
#else
#define LOG_INFO(fmt, ...)                                                     \
  do {                                                                         \
    (void)fmt;                                                                 \
  } while (0)
#endif

#ifdef LOG_ENABLE_ERROR_MSG
/**
 * @brief Writes an 'Error' message to the terminal.
 *
 */
#define LOG_ERROR(fmt, ...)                                                    \
  __LOG_WRITE("ERROR", FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...)                                                    \
  do {                                                                         \
    (void)fmt;                                                                 \
  } while (0)
#endif

#ifdef __cplusplus
}
#endif

#endif