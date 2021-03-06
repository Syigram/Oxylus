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

include(CheckIncludeFiles)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/cmake/OxylusConfig.hpp.in ${CMAKE_SOURCE_DIR}/include/oxylus/OxylusConfig.hpp)

##
file(GLOB OXYLUS_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}/include *.?pp *.h)

set( OXYLUS_LIBRARIES "")

## Create all libraries first
add_subdirectory (configuration)
add_subdirectory (communication)
add_subdirectory (memory)
add_subdirectory (image)
add_subdirectory (training)

set_property(GLOBAL PROPERTY oxylus_libs_property "${OXYLUS_LIBRARIES}")


add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} 
  ${MPI_CXX_LIBRARIES} 
  ${OXYLUS_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${Boost_LIBRARIES})
