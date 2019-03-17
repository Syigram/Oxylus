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

find_package (Boost COMPONENTS system filesystem serialization unit_test_framework REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

#include( ${CMAKE_SOURCE_DIR}/cmake/CompilerFlags.cmake )
#include( ${CMAKE_SOURCE_DIR}/cmake/ExternalLibs.cmake )

file(GLOB OXYLUS_TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.hpp)

#Import the oxylus libraries
get_property(OXYLUS_LIBRARIES GLOBAL PROPERTY oxylus_libs_property)
#message("OXYLUS_LIBRARIES=${OXYLUS_LIBRARIES}")


add_executable (${CMAKE_PROJECT_NAME}_tests ${OXYLUS_TEST_SRCS})
target_link_libraries (${CMAKE_PROJECT_NAME}_tests
                       ${OXYLUS_LIBRARIES}
                       ${Boost_LIBRARIES})

add_test(NAME ${CMAKE_PROJECT_NAME}_tests COMMAND ${CMAKE_PROJECT_NAME}_tests)
