cmake_minimum_required(VERSION 3.21)

project(vimcord)

add_subdirectory(deps/restclient-cpp)
add_subdirectory(deps/simple-logger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(VIMCORD_SOURCE
	platform/platform_linux.cpp
	platform/platform_win32.cpp
	http/http_server.cpp
	main.cpp
	chat_module_frame.cpp
	discord_api_handler.cpp
	filesystem.cpp
	time.cpp
)
list(TRANSFORM VIMCORD_SOURCE PREPEND "src/")

add_executable(vimcord ${VIMCORD_SOURCE})
target_include_directories(vimcord PRIVATE src/ deps/restclient-cpp/include/ deps/json)

find_package(wxWidgets REQUIRED COMPONENTS net core base)

if(wxWidgets_USE_FILE) # not defined in CONFIG mode
	include(${wxWidgets_USE_FILE})
endif()

target_link_libraries(vimcord ${wxWidgets_LIBRARIES} restclient-cpp simple-logger)

if(WIN32)
	target_compile_definitions(vimcord PUBLIC V_ISWIN)
endif()
if(UNIX)
	target_compile_definitions(vimcord PUBLIC V_ISLINUX)

	# Create symlink to assets folder in build path
	if(NOT EXISTS "${CMAKE_BINARY_DIR}/assets")
		message(STATUS "Creating symlink for assets.")
	
		execute_process(
			COMMAND ln -s ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_BINARY_DIR}/assets
		)
	endif()
endif()
