BUILD_DIR := build
TARGET := boris

# -----------------------------
# Default target
# -----------------------------
all: run

# -----------------------------
# Install dependencies (Conan)
# -----------------------------
deps:
	conan install . \
		--output-folder=$(BUILD_DIR) \
		--build=missing

# -----------------------------
# Configure + build (CMake)
# -----------------------------
build: deps
	cmake -S . -B $(BUILD_DIR) \
		-DCMAKE_TOOLCHAIN_FILE=$(BUILD_DIR)/conan_toolchain.cmake
	cmake --build $(BUILD_DIR)

# -----------------------------
# Run executable
# -----------------------------
run: build
	./$(BUILD_DIR)/$(TARGET)

# -----------------------------
# Clean build artifacts
# -----------------------------
clean:
	rm -rf $(BUILD_DIR)

# -----------------------------
# Rebuild from scratch
# -----------------------------
rebuild: clean all
