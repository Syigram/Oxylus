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
file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS *.cpp)

add_library(oxylus_configuration STATIC ${SRCS})
set(OXYLUS_LIBRARIES ${OXYLUS_LIBRARIES} oxylus_configuration PARENT_SCOPE)
