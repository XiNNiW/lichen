cmake_minimum_required(VERSION 3.16)
project(Lichen)

set(SOURCE_DIR src)
set(TEST_DIR test)
list(APPEND CMAKE_MODULE_PATH cmake_modules)

find_package(SoundIo REQUIRED)
include_directories(${SOUNDIO_INCLUDE_DIR})

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
FLEX_TARGET(MyScanner libFungi_interpreter/src/lang/sporelang_flex.l  ${CMAKE_CURRENT_BINARY_DIR}/sporelang_flex.cpp)
BISON_TARGET(MyParser libFungi_interpreter/src/lang/sporelang_bison.y ${CMAKE_CURRENT_BINARY_DIR}/sporelang_bison.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

#Algae (DSP)
add_library( LibAlgae_dsp STATIC
             libAlgae_dsp/dsp.cpp)

#Fungus (Interpreter)
add_library( LibFungi_interpreter STATIC
             libFungi_interpreter/src/eval.cpp)


add_executable(
    Lichen
    main.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)
target_link_libraries(LibFungi_interpreter ${SOUNDIO_LIBRARY})
target_link_libraries(LibFungi_interpreter ${FLEX_LIBRARIES})
target_link_libraries(Lichen LibAlgae_dsp)
target_link_libraries(Lichen LibFungi_interpreter)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)