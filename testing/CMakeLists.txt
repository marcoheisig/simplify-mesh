include_directories( ../src )

find_package(VCG REQUIRED)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

include_directories(
  SYSTEM ${VCG_INCLUDE_DIR}
  SYSTEM ${Boost_INCLUDE_DIRS}
  )

add_executable(meshtest meshtest.cpp)

target_link_libraries(
  meshtest
  ${Boost_LIBRARIES}
  )

add_test(NAME Mesh COMMAND meshtest)
