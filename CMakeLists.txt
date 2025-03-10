cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_STANDARD 11)
set(PLAYDATE_SDK_PATH ../../PlaydateSDK CACHE PATH "Path to PlaydateSDK")
set(ENVSDK ${PLAYDATE_SDK_PATH})
if (NOT ${ENVSDK} STREQUAL "")
	# Convert path from Windows
	file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
	execute_process(
			COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
			COMMAND head -n 1
			COMMAND cut -c9-
			OUTPUT_VARIABLE SDK
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif()

if (NOT EXISTS ${SDK})
	message(FATAL_ERROR "SDK Path not found; set CACHE value PLAYDATE_SDK_PATH")
	return()
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Game Name Customization
set(PLAYDATE_GAME_NAME TemplateGame CACHE STRING "Game name")
set(PLAYDATE_GAME_DEVICE TemplateGame_DEVICE CACHE STRING "Game name for device")

set(VSCODE_ACTIONS OFF CACHE BOOL "VS Code spevific actions")

if (VSCODE_ACTIONS)
	message("Run actions for VS Code")
	# Setting VS Code launch vars
	set (VSCODE_SETTINGS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.vscode/settings.json")
	if (EXISTS "${VSCODE_SETTINGS_PATH}")
		file (READ "${VSCODE_SETTINGS_PATH}" VSCODE_SETTINGS_JSON)
	else()
		set (VSCODE_SETTINGS_JSON "{}")
	endif()
	string (JSON VSCODE_SETTINGS_JSON_NEW SET ${VSCODE_SETTINGS_JSON} "PLAYDATE_SDK_PATH" "\"${SDK}\"")
	string (JSON VSCODE_SETTINGS_JSON_NEW SET ${VSCODE_SETTINGS_JSON_NEW} "PLAYDATE_GAME_NAME" "\"${PLAYDATE_GAME_NAME}\"")
	if (NOT "${VSCODE_SETTINGS_JSON_NEW}" STREQUAL "${VSCODE_SETTINGS_JSON}")
	file (WRITE "${VSCODE_SETTINGS_PATH}" "${VSCODE_SETTINGS_JSON_NEW}")
	endif()
endif()

project(${PLAYDATE_GAME_NAME} C ASM)

file(GLOB IMAGES
	"Source/images/*"
)

file(GLOB CODE
	"Code/*"
)

if (TOOLCHAIN STREQUAL "armgcc")
	message("Tool chain armgcc.")
	add_executable(${PLAYDATE_GAME_DEVICE} main.c ${CODE})
else()
	add_library(${PLAYDATE_GAME_NAME} SHARED main.c ${CODE} ${IMAGES})
endif()

include(${SDK}/C_API/buildsupport/playdate_game.cmake)
