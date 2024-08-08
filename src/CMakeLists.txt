set(SOURCE timer_manager.cpp main.cpp)

add_executable(planck ${SOURCE})
target_include_directories(planck PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(planck pthread)
set_target_properties(planck PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)