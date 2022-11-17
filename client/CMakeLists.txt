cmake_minimum_required(VERSION 2.8)

if(CMAKE_BUILD_TYPE MATCHES Release)
	message("Release Build")
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/release)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/release)
else(CMAKE_BUILD_TYPE MATCHES Release)
	message("Debug Build")
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/debug)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/debug)
endif(CMAKE_BUILD_TYPE MATCHES Release)

set(PROJECT_NAME
    ProjetClient
    )

set(SRCS
    src/main.cpp
    src/TCPClient.cpp
    src/CodeReader.cpp
    )

project(${PROJECT_NAME} CXX C)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
	include_directories(${OpenCV_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} ${ZBAR_LIBRARIES} zbar)
else(OpenCV_FOUND)
	message(FATAL ERROR "Librairie OpenCV introuvable!")
endif(OpenCV_FOUND)


