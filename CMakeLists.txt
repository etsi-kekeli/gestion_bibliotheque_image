cmake_minimum_required(VERSION 3.10)
project(gestion_bibliotheque_image)

#Set the C++ standard to 17 
set(CMAKE_CXX_STANDARD 17)

#trouver package OpenCV
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV se trouve dans: ${OpenCV_DIR}")

#inclure les repertoires de OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Cr�er un executable
add_executable(gestion_bibliotheque_image main.cpp Image.cpp)

#link Libraries OpenCV
target_link_libraries(gestion_bibliotheque_image ${OpenCV_LIBS})
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")

