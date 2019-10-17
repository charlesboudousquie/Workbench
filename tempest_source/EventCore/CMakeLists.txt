cmake_minimum_required(VERSION 3.15.0)

# define target
add_library(EventCore STATIC
    Private/HEADER_ONLY_MODULE.cpp
)

# expose headers
target_include_directories(EventCore
    PUBLIC Public
)

# link to dependencies
#target_link_libraries(EventCore
#    PRIVATE <dependencyModule>
#)

