cmake_minimum_required(VERSION 3.2)

project(learn_octomap)

find_package(octomap REQUIRED)
include_directories(${OCTOMAP_INCLUDE_DIRS})

find_package(fcl REQUIRED)
include_directories(${fcl_INCLUDE_DIRS})


add_executable (createOctomap src/createOctomap.cpp)
target_link_libraries(createOctomap octomap)

add_executable(readOctomap src/readOctomapForCollisionChecking.cpp)
target_link_libraries(readOctomap fcl octomap)


