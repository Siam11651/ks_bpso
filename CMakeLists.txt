cmake_minimum_required(VERSION 3.5.0)
project(ks_bpso VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_subdirectory("lib/pso")

add_executable(ks_bpso 
    "src/knapsack.cpp"
    "src/main.cpp"
)

target_include_directories(ks_bpso PRIVATE "include")
target_link_libraries(ks_bpso
    pso
)

add_custom_command(TARGET ks_bpso POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/datasets"
    "${CMAKE_BINARY_DIR}/datasets")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
