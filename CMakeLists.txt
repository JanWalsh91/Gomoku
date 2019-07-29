cmake_minimum_required(VERSION 3.5)
project(gomoku)
include(ExternalProject)

if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -std=c++1z")
endif()

if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/libs/)
	execute_process(COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/libs/)
endif()


if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/libs/SFML)
	message(STATUS "Downloading SFML...")
	file(DOWNLOAD https://github.com/SFML/SFML/releases/download/2.5.1/SFML-2.5.1-sources.zip ${CMAKE_CURRENT_BINARY_DIR}/libs/SFML.zip)
	execute_process(COMMAND unzip -q ${CMAKE_CURRENT_BINARY_DIR}/libs/SFML.zip -d ${CMAKE_CURRENT_BINARY_DIR}/libs/)
	execute_process(COMMAND mv ${CMAKE_CURRENT_BINARY_DIR}/libs/SFML-2.5.1 ${CMAKE_CURRENT_BINARY_DIR}/libs/SFML)
	execute_process(COMMAND rm ${CMAKE_CURRENT_BINARY_DIR}/libs/SFML.zip)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/libs/SFML/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/interface/board)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/interface/gui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/controller)

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/libs/SFML)


set(
	SOURCE_FILES
	src/main.cpp
	src/interface/SFMLWindow.cpp
	src/interface/board/Background.cpp
	src/interface/board/Grid.cpp
	src/controller/Player.cpp
	src/interface/gui/Button.cpp
	src/interface/gui/TextBox.cpp
	src/interface/gui/Colors.cpp	
)

set(
	HEADER_FILES
	src/interface/SFMLWindow.hpp
	src/interface/board/IRenderable.hpp
	src/interface/board/Background.hpp
	src/interface/board/Grid.hpp
	src/controller/Player.hpp
	src/interface/gui/Button.hpp
	src/interface/gui/TextBox.hpp
	src/interface/gui/Colors.hpp
)

add_executable(gomoku ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories (gomoku PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gomoku sfml-graphics)