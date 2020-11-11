cmake_minimum_required(VERSION 3.16)
project(Lichen)

set(SOURCE_DIR src)
set(TEST_DIR test)
list(APPEND CMAKE_MODULE_PATH cmake_modules)



find_package(SoundIo)

add_library( LibDsp STATIC
             libDSP/dsp.cpp)


if(SOUNDIO_FOUND)
    message(STATUS "SOUNDIO_FOUND=${SOUNDIO_FOUND}")
    include_directories(${SOUNDIO_INCLUDE_DIR})
    add_executable(
        Lichen
        main.cpp
    )
    target_link_libraries(Lichen ${SOUNDIO_LIBRARY})
    target_link_libraries(Lichen LibDsp)
else()
    message("failed to find libraries!")
endif()