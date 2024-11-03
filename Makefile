CONFIG ?= Debug
BUILD_DIR ?= build/$(CONFIG)
GENERATOR ?= Ninja
# NUM_CORES ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu)
NUM_CORES ?= 4

.PHONY: clear
clear:
	@echo "\033[0;33mCleaning build directory...\033[0m"
	rm -rf $(BUILD_DIR)

.PHONY: clear-all
clear-all:
	@echo "\033[0;33mCleaning build directory...\033[0m"
	rm -rf build

.PHONY: config
config:
	@echo "\033[0;32mConfiguring project...\033[0m"
	cmake -S . -B $(BUILD_DIR) -G $(GENERATOR) -DCMAKE_BUILD_TYPE=$(CONFIG)
	@osascript -e 'display notification "Configuration done!" with title "CMake Build"'
	@afplay /System/Library/Sounds/Submarine.aiff

.PHONY: build
build:
	@echo "\033[0;32mBuilding project...\033[0m"
	@echo "\033[0;32mUsing $(NUM_CORES) cores\033[0m"
	cmake --build $(BUILD_DIR) --parallel $(NUM_CORES) --config $(CONFIG)
	@osascript -e 'display notification "Build done!" with title "CMake Build"'
	@afplay /System/Library/Sounds/Submarine.aiff

.PHONY: rebuild
rebuild: clear config build