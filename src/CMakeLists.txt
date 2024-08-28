# 获取 src 目录及其子目录下的所有 .cpp 文件
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"  "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
set(SOURCE ${SRC_FILES})

find_package(concurrentqueue REQUIRED)
add_executable(planck ${SOURCE})
target_include_directories(planck PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(planck pthread concurrentqueue::concurrentqueue)
set_target_properties(planck PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)