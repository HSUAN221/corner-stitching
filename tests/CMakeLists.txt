set(test_name demo_test)
add_gtest_with_libraries(${PROJECT_NAME}_${test_name} ${test_name}.cpp ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_${test_name} PUBLIC ${PROJECT_NAME})
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME}_${test_name} PUBLIC OpenMP::OpenMP_CXX)
endif()

unset(test_name)


