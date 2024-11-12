cmake_minimum_required(VERSION 3.31)
project(GAME 
	VERSION 1.0
	DESCRIPTION "a simple game"
	LANGUAGES "CXX"
)


file(COPY res DESTINATION .)

add_executable(game)
target_link_libraries(game raylib)
target_sources(game PUBLIC main.cpp)

