cmake_minimum_required(VERSION 3.16)

project(browser-sdk-cpp-demo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif()

add_definitions(-DUNICODE -D_UNICODE)

set(DEMO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk_wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk_wrapper.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/brosdk/brosdk.h)

if(WIN32)
  list(APPEND DEMO_SOURCES
       ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/dlfcn-win32/src/dlfcn.c)
endif()

add_executable(${PROJECT_NAME} ${DEMO_SOURCES})

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
          ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
          ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rapidjson/include)

if(MSVC)
  target_compile_options(
    ${PROJECT_NAME}
    PRIVATE /std:c++latest
            /Zc:preprocessor
            /utf-8
            /W3
            /wd4244
            /wd4018
            /wd4996
            /wd4267)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -std=c++20 -finput-charset=UTF-8)
endif()

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE version.lib Dbghelp.lib)
elseif(APPLE)
  target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE "-framework CoreFoundation"
            "-framework ApplicationServices"
            "-liconv")
elseif(UNIX)
  target_link_libraries(${PROJECT_NAME} PRIVATE dl pthread)
endif()

if(CMAKE_CONFIGURATION_TYPES)
  set(_demo_output_dir "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
else()
  set(_demo_output_dir "${CMAKE_CURRENT_BINARY_DIR}")
endif()

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${_demo_output_dir}"
             LIBRARY_OUTPUT_DIRECTORY "${_demo_output_dir}"
             ARCHIVE_OUTPUT_DIRECTORY "${_demo_output_dir}")

if(WIN32)
  set(BROSDK_RUNTIME_NAME "brosdk.dll")
elseif(APPLE)
  set(BROSDK_RUNTIME_NAME "brosdk.dylib")
else()
  set(BROSDK_RUNTIME_NAME "brosdk.so")
endif()

set(BROSDK_RUNTIME_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/brosdk/${BROSDK_RUNTIME_NAME}")

if(EXISTS "${BROSDK_RUNTIME_SOURCE}")
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${BROSDK_RUNTIME_SOURCE}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${BROSDK_RUNTIME_NAME}"
    COMMENT "Copying ${BROSDK_RUNTIME_NAME} to the demo output directory")
endif()
