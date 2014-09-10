CMAKE_MINIMUM_REQUIRED ( VERSION 2.6 )

PROJECT( distributed-polygon-reducer )

set( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH} ${distributed-polygon-reducer_SOURCE_DIR}/cmake )

add_definitions("-std=c++0x -Wall")

if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Build type. Avilable types are Debug, Release and Native" FORCE)
endif()

set(EXECUTABLE_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}
  CACHE PATH
  "Directory for all executables" FORCE)

enable_testing()

add_subdirectory( src )
add_subdirectory( testing )
