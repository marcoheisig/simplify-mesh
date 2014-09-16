find_package(MPI REQUIRED)
find_package(VCG REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options system thread)

configure_file(Version.hpp.in Version.hpp)
include_directories(SYSTEM ${CMAKE_BINARY_DIR}/src)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${VCG_INCLUDE_DIR})

add_executable(
  simplify-mesh
  main.cpp
  Process.cpp
  Mesh_base.cpp
  Mesh_io.cpp
  Mesh_simplify.cpp
  StaticScheduler.cpp
  )

target_compile_options(
  simplify-mesh
  PUBLIC
  ${MPI_CXX_COMPILE_FLAGS}
  )

target_link_libraries(
  simplify-mesh
  ${MPI_LIBRARIES}
  ${Boost_LIBRARIES}
  )

message(STATUS ${Boost_LIBRARIES})
