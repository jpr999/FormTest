project(StringGenTest)

set (This ${PROJECT_NAME})

include(FetchContent)
FetchContent_Declare(google_test
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG HEAD
)
FetchContent_MakeAvailable(google_test)

include(GoogleTest)

include(CTest) 

include_directories(${EXTERNAL_INSTALL_LOCATION}/include ../StringGenLib)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib64)

add_executable(${This} src/StringGenTest.cpp)
target_link_libraries(${This} PUBLIC gtest gtest_main pthread StringGenLib)
gtest_discover_tests(${This})
