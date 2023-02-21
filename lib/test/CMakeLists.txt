set (UNIT_TEST_SOURCES
    src/main.cc
    src/cartesian-tree.cc
)

if (ENABLE_GTEST)
    add_executable(unit-test ${UNIT_TEST_SOURCES})
    target_include_directories(unit-test PUBLIC ${OFFLINE_LCA_INCLUDE_DIR})
    target_link_libraries(unit-test ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(unit-test offline-lca)
    gtest_discover_tests(unit-test)
endif()