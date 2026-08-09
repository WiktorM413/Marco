include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.15.2
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB MARCO_TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*_test.cpp")

add_executable(marco_tests ${MARCO_TEST_SOURCES})

set_target_properties(marco_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

target_link_libraries(marco_tests
	PRIVATE
	marco
	GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(marco_tests)