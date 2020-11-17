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
             libFungi_interpreter/src/eval.cpp
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
add_executable(interpreter_tests libFungi_interpreter/test/interpreter_test.cpp)
add_executable(varient_tests libFungi_interpreter/test/varient_test.cpp)
target_link_libraries(interpreter_tests GTest::GTest GTest::Main)
target_link_libraries(varient_tests GTest::GTest GTest::Main)

add_test(AllTestsInLichen interpreter_tests)
add_test(AllTestsInLichen varient_tests)

