cmake_minimum_required(VERSION 3.10)
project("N Body Simulations" CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Scegli il tipo di build" FORCE)
endif()
set(TYPE "GALAXY" CACHE STRING "TYPE: GALAXY or EIGHT or CIRCLE")

if(TYPE STREQUAL "GALAXY")
  set(INCLUDE_P "${CMAKE_CURRENT_SOURCE_DIR}/galaxy")
elseif(TYPE STREQUAL "EIGHT")
  set(INCLUDE_P "${CMAKE_CURRENT_SOURCE_DIR}/eight")
elseif(TYPE STREQUAL "CIRCLE")
  set(INCLUDE_P "${CMAKE_CURRENT_SOURCE_DIR}/circle")
elseif(TYPE STREQUAL "ELLYPTIC")
  set(INCLUDE_P "${CMAKE_CURRENT_SOURCE_DIR}/ellypticGalaxy")
endif()

# Cerca SFML sul sistema
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(OpenMP REQUIRED)

set(EIGEN3_INCLUDE_DIR "usr/local/include")


# Crea l'eseguibile
add_executable(anim 
    simulation.cpp 
    ${INCLUDE_P}/graphics.cpp
    state.cpp
)

target_include_directories(anim PRIVATE ${INCLUDE_P})
target_include_directories(anim PRIVATE ${EIGEN3_INCLUDE_DIR})

# Collega le librerie
target_compile_options(anim PRIVATE -march=native)
target_link_libraries(anim PUBLIC sfml-graphics sfml-window sfml-system)
target_link_libraries(anim PUBLIC OpenMP::OpenMP_CXX)
