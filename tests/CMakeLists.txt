# Tests
add_executable(tests LexerTests.cpp SourceTests.cpp)

# Get GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Link
target_link_libraries(tests PRIVATE templater_lib project_options gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

# gtest has issues with these, disable
target_disable_static_analysis(tests)
target_disable_static_analysis(gtest)
target_disable_static_analysis(gtest_main)
