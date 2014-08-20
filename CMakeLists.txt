CMAKE_MINIMUM_REQUIRED ( VERSION 2.6 )

PROJECT ( distributed-polygon-reducer )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Build type. Avilable types are Debug, Release and Native" FORCE)
endif()

add_subdirectory( src )
