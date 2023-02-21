set (OFFLINE_LCA_LIB_SOURCES
    src/tree-node.cc
)

add_library(offline-lca ${OFFLINE_LCA_LIB_SOURCES})
target_include_directories(offline-lca PUBLIC include)


add_subdirectory(test)