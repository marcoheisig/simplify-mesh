include_directories( ../src )

find_package(VCG REQUIRED)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
find_package(MPI REQUIRED)

configure_file(config.hpp.in config.hpp)

include_directories(
  SYSTEM ${VCG_INCLUDE_DIR}
  SYSTEM ${Boost_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/testing
  SYSTEM ${MPI_INCLUDE_PATH}
  )

add_executable(mesh_basics
  mesh_basics.cpp
  ../src/Mesh_base.cpp)
add_executable(mesh_io
  mesh_io.cpp
  ../src/Mesh_base.cpp
  ../src/Mesh_io.cpp)
add_executable(mesh_send_recv
  mesh_send_recv.cpp
  ../src/Mesh_base.cpp
  ../src/Mesh_io.cpp)
add_executable(mesh_simplify
  mesh_simplify.cpp
  ../src/Mesh_base.cpp
  ../src/Mesh_io.cpp
  ../src/Mesh_simplify.cpp)

target_compile_options(
  mesh_send_recv
  PUBLIC
  ${MPI_CXX_COMPILE_FLAGS}
  )

target_link_libraries(mesh_basics    ${Boost_LIBRARIES} ${MPI_LIBRARIES})
target_link_libraries(mesh_io        ${Boost_LIBRARIES} ${MPI_LIBRARIES})
target_link_libraries(mesh_send_recv ${Boost_LIBRARIES} ${MPI_LIBRARIES})
target_link_libraries(mesh_simplify  ${Boost_LIBRARIES} ${MPI_LIBRARIES})

add_test(NAME Basics   COMMAND mesh_basics)
add_test(NAME IO       COMMAND mesh_io)
add_test(NAME SendRecv COMMAND mpirun -np 2 ../mesh_send_recv)
add_test(NAME Simp     COMMAND mesh_simplify)

