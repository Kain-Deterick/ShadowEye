cmake_minimum_required(VERSION 3.5)

project (DB_API)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Source/main.cpp")
set(HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Headers/CheckProcess.h" 
			"${CMAKE_CURRENT_SOURCE_DIR}/Headers/SmartHandle.h")

add_subdirectory(MySQL_API)

include_directories(MySQL_API)
add_executable(DB_API WIN32 ${SOURCE} ${HEADERS})

target_link_libraries(DB_API MySQL_API)
