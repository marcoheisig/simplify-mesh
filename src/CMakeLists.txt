find_package(MPI REQUIRED)
find_package(VCG REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options system thread)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${VCG_INCLUDE_DIR})

add_executable(
  distributed-polygon-reducer
  main.cpp
  Process.cpp
  Mesh_base.cpp
  Mesh_io.cpp
  Mesh_simplify.cpp
  StaticScheduler.cpp
  )

target_compile_options(
  distributed-polygon-reducer
  PUBLIC
  ${MPI_CXX_COMPILE_FLAGS}
  )

target_link_libraries(
  distributed-polygon-reducer
  ${MPI_LIBRARIES}
  ${Boost_LIBRARIES}
  )

message(STATUS ${Boost_LIBRARIES})
