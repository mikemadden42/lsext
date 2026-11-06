cmake_minimum_required(VERSION 3.10)

project(lsext CXX)

set (CMAKE_CXX_STANDARD 17)

add_executable(lsext lsext.cc)

if (NOT MSVC)
  target_compile_options(lsext PRIVATE -Wall -Wextra -pedantic)
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Darwin" AND NOT CMAKE_OSX_ARCHITECTURES)
    set_target_properties(lsext PROPERTIES
        OSX_ARCHITECTURES "x86_64;arm64"
    )
endif()

install(TARGETS lsext DESTINATION bin)

enable_testing()
add_test(NAME lsext
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_tests.sh $<TARGET_FILE:lsext>
)
