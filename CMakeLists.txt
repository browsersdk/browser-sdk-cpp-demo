cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  # CMAKE_MSVC_DEBUG_INFORMATION_FORMAT is managed exclusively in
  # build/cmake/build_type.cmake to avoid conflicts between EditAndContinue
  # (/ZI) and ProgramDatabase (/Zi).
endif()

add_definitions(-DUNICODE -D_UNICODE)
set(CMAKE_CONFIGURATION_TYPES
    "Debug;Release;"
    CACHE STRING "info" FORCE)

project(browser-sdk-demo)
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/main.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk_wrapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk_wrapper.cc
				${CMAKE_CURRENT_SOURCE_DIR}/autotest.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cc)
if(WIN32)   
list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/dlfcn-win32/src/dlfcn.c)
endif()
add_executable(${PROJECT_NAME} ${SOURCES})

if(MSVC)
  if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT
                                                        ${PROJECT_NAME})
    target_compile_options(
      ${PROJECT_NAME}
      PRIVATE $<$<C_COMPILER_ID:MSVC>:/W3> $<$<C_COMPILER_ID:MSVC>:/wd4244>
              $<$<C_COMPILER_ID:MSVC>:/wd4018> $<$<C_COMPILER_ID:MSVC>:/wd4996>
              $<$<C_COMPILER_ID:MSVC>:/wd4267>)
  endif()
  # /Zi (ProgramDatabase) is set per-config in build/cmake/build_type.cmake via
  # CMAKE_MSVC_DEBUG_INFORMATION_FORMAT to guarantee consistent PDB generation.
  # Do NOT add /Zi or /ZI here to avoid symbol offset mismatches (breakpoint
  # drift). Use literal MSVC flags directly (no generator expressions) to
  # prevent GCC/Clang flags like -g/-O0 from leaking into cl.exe invocations
  # (D9002 warning).
  add_compile_options(/std:c++latest /Zc:preprocessor)

  add_compile_options("/utf-8")
else()
  # GCC / Clang: -g is only needed here; debug info flags are in
  # CMAKE_CXX_FLAGS_DEBUG
  add_compile_options(-std=c++20)
  add_compile_options(-finput-charset=UTF-8)
endif()

if(WIN32)
  target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/node-addon-api)

  target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  #3rdparty/dlfcn-win32/src/
  target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE version.lib
    PRIVATE Dbghelp.lib)
elseif(APPLE)
  target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${xsROOT_DIR}
            ${xsINCLUDE_DIR}
            ${xs3RDPARTY_DIR}/utfcpp/source/
            ${xs3RDPARTY_DIR}/cpp-httplib/
            ${xsVCPKG_INCLUDE_DIR})

  set(_demo_link_dirs)
  if(EXISTS "${xsLIBS_DIR}")
    list(APPEND _demo_link_dirs "${xsLIBS_DIR}")
  endif()
  if(EXISTS "${xsVCPKG_LIB_DIR}")
    list(APPEND _demo_link_dirs "${xsVCPKG_LIB_DIR}")
  endif()
  if(_demo_link_dirs)
    target_link_directories(${PROJECT_NAME} PRIVATE ${_demo_link_dirs})
  endif()

  target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE "-framework CoreFoundation"
    PRIVATE "-framework Cocoa"
    PRIVATE "-framework CoreGraphics"
    PRIVATE "-liconv"
    PRIVATE ${xsOPENSSL_LIBRARY}
    PRIVATE ${xsFMT_LIBRARY})
endif()

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${xsBIN_DIR}/$<CONFIG>"
             LIBRARY_OUTPUT_DIRECTORY "${xsBIN_DIR}/$<CONFIG>"
             ARCHIVE_OUTPUT_DIRECTORY "${xsBIN_DIR}/$<CONFIG>")
