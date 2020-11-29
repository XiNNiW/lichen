cmake_minimum_required(VERSION 3.16)
# set (CMAKE_CXX_STANDARD 14)

# project(Lichen LANGUAGES CXX VERSION 14)
project(Lichen)


set(SOURCE_DIR src)
set(TEST_DIR test)
list(APPEND CMAKE_MODULE_PATH cmake_modules)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

find_package(SoundIo REQUIRED)
include_directories(${SOUNDIO_INCLUDE_DIR})

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
FLEX_TARGET(MyScanner libFungi_interpreter/src/lang/sporelang_flex.l  ${CMAKE_CURRENT_BINARY_DIR}/lib/sporelang_flex.cpp)
BISON_TARGET(MyParser libFungi_interpreter/src/lang/sporelang_bison.y ${CMAKE_CURRENT_BINARY_DIR}/lib/sporelang_bison.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

#Algae (DSP)
add_library( LibAlgae_dsp STATIC
             libAlgae_dsp/dsp.cpp)

#Fungi (Interpreter)
add_library( LibFungi_interpreter STATIC
             libFungi_interpreter/src/eval.h
             ${BISON_MyParser_OUTPUTS}
             ${FLEX_MyScanner_OUTPUTS}
)
target_link_libraries(LibFungi_interpreter ${FLEX_LIBRARIES})


#Production Build
add_executable(
    Lichen
    main.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)
target_link_libraries(Lichen LibAlgae_dsp)
target_link_libraries(Lichen ${SOUNDIO_LIBRARY})
target_link_libraries(Lichen LibFungi_interpreter)

#Tests
add_executable( LichenTests 
    libFungi_interpreter/test/interpreter_test.cpp
    libFungi_interpreter/test/semantic_analyzer_test.cpp
    libFungi_interpreter/test/data_variant_test.cpp
    libFungi_interpreter/test/symbol_variant_test.cpp
    libFungi_interpreter/test/scoped_symbol_table_test.cpp
    libFungi_interpreter/test/activation_record_test.cpp
    libFungi_interpreter/test/maybe_test.cpp
    libFungi_interpreter/test/either_test.cpp
    libFungi_interpreter/test/signal_wrapper_test.cpp
    libFungi_interpreter/test/eval_test.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)
target_link_libraries(LichenTests GTest::GTest GTest::Main)
gtest_discover_tests(LichenTests)

# add_executable(interpreter_tests libFungi_interpreter/test/interpreter_test.cpp)
# target_link_libraries(interpreter_tests GTest::GTest GTest::Main)
# gtest_discover_tests(interpreter_tests)

# add_executable(varient_tests libFungi_interpreter/test/varient_test.cpp)
# target_link_libraries(varient_tests GTest::GTest GTest::Main)
# gtest_discover_tests(varient_tests)

# add_executable(symbol_tests libFungi_interpreter/test/symbol_test.cpp)
# target_link_libraries(symbol_tests GTest::GTest GTest::Main)
# gtest_discover_tests(symbol_tests)

# add_executable(scoped_symbol_table_tests libFungi_interpreter/test/scoped_symbol_table_test.cpp)
# target_link_libraries(scoped_symbol_table_tests GTest::GTest GTest::Main)
# gtest_discover_tests(scoped_symbol_table_tests)

# add_executable(activation_record_tests libFungi_interpreter/test/activation_record_test.cpp)
# target_link_libraries(activation_record_tests GTest::GTest GTest::Main)
# gtest_discover_tests(activation_record_tests)


