
# Description
Ready to go VSCode friendly project for Playdate. Project made for development in C programming language but suitable for lua also.

To build and run on Playdate emulator having CMake and Visual Studio build tools is enough.
If you want to build and deploy on device you have to install [ninja](https://ninja-build.org/) build system and [arm compiler](https://developer.arm.com/downloads/-/gnu-rm).
## Installation
- Install [Playdate SDK](https://play.date/dev/)
- Just clone repo and move *.json files from .vscode/examples to .vscode/examples/..
- In CMakePresets.json change variables PLAYDATE_SDK_PATH.
## Usage
### To run on emulator:
- configure and build Win_Dev_Visual_Studio preset 
- hit F5 to run debug build
### To run on device:
- configure and build Arm_Dev_Ninja preset
- run task "Run on Playdate" with device connected.
