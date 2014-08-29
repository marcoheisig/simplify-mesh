include_directories( ../src )

find_package(VCG REQUIRED)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

configure_file(config.hpp.in config.hpp)

include_directories(
  SYSTEM ${VCG_INCLUDE_DIR}
  SYSTEM ${Boost_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/testing
  )

add_executable(mesh_basics mesh_basics.cpp ../src/Mesh.cpp)
add_executable(mesh_io mesh_io.cpp ../src/Mesh.cpp)
add_executable(mesh_simplify mesh_simplify.cpp ../src/Mesh.cpp)

target_link_libraries(mesh_basics ${Boost_LIBRARIES})
target_link_libraries(mesh_io     ${Boost_LIBRARIES})
target_link_libraries(mesh_simplify     ${Boost_LIBRARIES})

add_test(NAME Basics COMMAND mesh_basics)
add_test(NAME IO     COMMAND mesh_io)
add_test(NAME Simp   COMMAND mesh_simplify)

