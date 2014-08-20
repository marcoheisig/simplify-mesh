find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(
  distributed-polygon-reducer
  main.cpp
  App.cpp
  local_coarsening.cpp
  merging.cpp
  Mesh.cpp
  )

target_compile_options(
  distributed-polygon-reducer
  PUBLIC
  ${MPI_CXX_COMPILE_FLAGS}
  )

target_link_libraries(
  distributed-polygon-reducer
  ${MPI_LIBRARIES}
  )
