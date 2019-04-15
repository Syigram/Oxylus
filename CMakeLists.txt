########################################################
## Copyright (C) 2018                                 ##
## SIPLab Tecnológico de Costa Rica                   ##
##                                                    ##
## Project: Oxylus Framework                          ##
##                                                    ##
## Check the LICENCE.txt file for further information ##
##                                                    ##
## author: Pablo Alvarado                             ##
## date:   2018-08-31                                 ##
########################################################

cmake_minimum_required(VERSION 3.5)

project(oxylus VERSION 0.0.1 LANGUAGES CXX)

## Options
option(OXYLUS_ENABLE_OpenMP "Enable use of OpenMP" on)

## Ensure that all binaries are stored in the same directory (usually build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

## All external libraries should be checked only once
include(ExternalLibs)

## All compiler options
include(CompilerFlags)

## Create libraries and executables
add_subdirectory (src)

## Also build unit tests
# enable_testing ()
# add_subdirectory (test)
IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()
