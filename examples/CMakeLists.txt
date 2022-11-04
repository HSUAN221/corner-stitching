include_directories(${CMAKE_SOURCE_DIR}/examples)


set(example_name demo_example)
add_executable(${PROJECT_NAME}_${example_name} ${example_name}.cpp)
target_link_libraries(${PROJECT_NAME}_${example_name} PUBLIC ${PROJECT_NAME})
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME}_${example_name} PUBLIC OpenMP::OpenMP_CXX)
endif()


unset(example_name)
