FILE(GLOB_RECURSE BENCHMARK_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp") 
SET(SOURCE_FILE ${BENCHMARK_FILES})

find_package(benchmark REQUIRED)

ADD_EXECUTABLE(planck_benchmark ${SOURCE_FILE})
# TARGET_INCLUDE_DIRECTORIES(planck_benchmark PUBLIC ${GTEST_INCLUDE_DIRS} )
TARGET_LINK_LIBRARIES(planck_benchmark benchmark::benchmark_main)
SET_TARGET_PROPERTIES(planck_benchmark PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)