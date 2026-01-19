cmake_minimum_required(VERSION 3.16)

# Enable testing (already called in top-level CMakeLists) Add GoogleTest via
# vcpkg (gtest is listed in vcpkg.json)
find_package(GTest CONFIG REQUIRED)

add_executable(unit_tests example_test.cpp)

target_link_libraries(unit_tests PRIVATE GTest::gtest GTest::gtest_main
                                         Qt6::Widgets SHantilly-ui)

enable_testing()
add_test(NAME unit_tests COMMAND unit_tests)
