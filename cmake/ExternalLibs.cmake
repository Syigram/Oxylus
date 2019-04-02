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

find_package (Boost COMPONENTS system filesystem program_options serialization mpi log date_time REQUIRED)

if (Boost_FOUND)
  include_directories (${Boost_INCLUDE_DIR})
endif (Boost_FOUND)

if (OXYLUS_ENABLE_OpenMP)
  find_package(OpenMP)
  if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  endif()
endif (OXYLUS_ENABLE_OpenMP)

find_package(MPI REQUIRED)
if (MPI_FOUND)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MPI_CXX_COMPILE_OPTIONS}")

  ## It seems for older versions MPI_CXX_INCLUDE_PATH is used, while
  ## for newer versions MPI_CXX_HEADER_DIR is used
  if (MPI_CXX_HEADER_DIR)
    include_directories (${MPI_CXX_HEADER_DIR})
  else ()
    include_directories (${MPI_CXX_INCLUDE_PATH})
  endif()
endif (MPI_FOUND)

find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
  include_directories (${OpenCV_INCLUDE_DIR})
endif (OpenCV_FOUND)

include_directories (${CMAKE_SOURCE_DIR}/include)
