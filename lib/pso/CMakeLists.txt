cmake_minimum_required(VERSION 3.5.0)
project(pso VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_library(pso
    "src/particle.cpp"
    "src/pso.cpp"
    "src/swarm.cpp"
)

target_include_directories(pso PUBLIC "include")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
