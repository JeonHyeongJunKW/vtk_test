cmake_minimum_required(VERSION 3.8)
project(vtk_test)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(VTK REQUIRED)

if($ENV{BUILD_TYPE} MATCHES "Release")
  set(CMAKE_BUILD_TYPE Release)
  message("Build type is Release")
else()
  set(CMAKE_BUILD_TYPE Debug)
  message("Build type is Debug")
endif()

# include_directories(
#   ${PROJECT_SOURCE_DIR}/include
# )

add_executable(
  test_1
  src/test_1/main.cpp)

target_link_libraries(test_1 PRIVATE ${VTK_LIBRARIES})

add_executable(
  test_2
  src/test_2/main.cpp)

target_link_libraries(test_2 PRIVATE ${VTK_LIBRARIES})
