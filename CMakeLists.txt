cmake_minimum_required(VERSION 3.14)
project(empyrical_cpp)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address")


include_directories(
        ./include
)

if (DEFINED ${BOOST_INCLUDE_DIR})
    include_directories(
            ${BOOST_INCLUDE_DIR}
    )
endif()

set(EMPYRICAL_HEADERS
        empyrical/utils.hpp
)

set(EMPYRICAL_SOURCES


)

add_executable(
        ${PROJECT_NAME}
        #
        main.cpp
        #
        ${EMPYRICAL_SOURCES}
)

if (DEFINED ${BOOST_LIB_DIR})
    target_link_libraries(
            ${PROJECT_NAME}
            -L${BOOST_LIB_DIR}
    )
endif()

target_link_libraries(
        ${PROJECT_NAME}
        z
        crypto
        ssl
        pthread
)



enable_testing()

add_executable(
        empyrical_test
        tests/test_utils.cpp

)

target_link_libraries(
        empyrical_test
        GTest::gtest_main
)

gtest_discover_tests(empyrical_test)