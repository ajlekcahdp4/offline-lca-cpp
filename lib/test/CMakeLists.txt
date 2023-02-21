set (UNIT_TEST_SOURCES
    src/main.cc
)

if (ENABLE_GTEST)
    add_executable(unit_test ${UNIT_TEST_SOURCES})
    target_link_libraries(unit_test ${GTEST_BOTH_LIBRARIES})
    gtest_discover_tests(unit_test)
endif()