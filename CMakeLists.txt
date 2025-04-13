cmake_minimum_required(VERSION 3.10)
project(own_containers)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CRITICAL: Add BOTH current directory and include directory
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Create executable
add_executable(my_program src/main.cc)


option(BUILD_TESTS "Build tests" ON)  # Add this near top if you want toggle

if(BUILD_TESTS)
       enable_testing()
    
    # Find system-installed GTest
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    
    # Vector tests
    add_executable(vector_tests tests/test_vector.cc)
    target_link_libraries(vector_tests 
        PRIVATE 
        GTest::GTest 
        GTest::Main 
        Threads::Threads
    )
    add_test(NAME vector_tests COMMAND vector_tests)    
endif()
