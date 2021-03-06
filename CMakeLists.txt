cmake_minimum_required (VERSION 2.8.11)
project (rht)
add_definitions(-std=c++11)

# Find the FFTW lib
find_library(FFTW_LIBRARY
     NAMES fftw-3.3 fftw3 fftw fftwf-3.3 fftwf3 fftwf)
set(FFTW_LIBRARIES "${FFTW_LIBRARY}")
if(UNIX AND NOT WIN32)
     find_library(FFTW_libm_LIBRARY
         NAMES m)
     list(APPEND FFTW_LIBRARIES "${FFTW_libm_LIBRARY}")
endif()
 
# Find the FFTWF lib
find_library(FFTWF_LIBRARY
     NAMES fftw-3.3f fftw3f fftwf)
list(APPEND FFTW_LIBRARIES "${FFTWF_LIBRARY}")

FIND_PACKAGE(Vigra)
FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(Boost COMPONENTS system)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
set(SOURCES rht/Rht.cpp Epoch.cpp, ImagePaer.cpp main.cpp)
set(HEADERS rht/Rht.h rht/Epoch.h, rht/ImagePaer.h)
add_executable (main main.cpp)
TARGET_INCLUDE_DIRECTORIES(main PUBLIC ${Vigra_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(main vigraimpex ${FFTW_LIBRARIES} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})