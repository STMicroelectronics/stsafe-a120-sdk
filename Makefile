# ******************************************************************************
# STSAFE-A120 Examples - Linux / STM32MP1 Build System
# ******************************************************************************
#
# This Makefile builds all STSAFE-A120 example applications for Linux,
# targeting the STM32MP1 platform (Cortex-A7 running OpenSTLinux).
#
# Usage:
#   make                          - Build all examples (native)
#   make EXAMPLE=01_Echo_loop     - Build a specific example
#   make CROSS_COMPILE=arm-linux-gnueabihf-  - Cross-compile for STM32MP1
#   make clean                    - Remove all build artifacts
#
# Prerequisites:
#   - STSELib submodule must be initialized:
#       git submodule update --init Middleware/STSELib
#   - OpenSSL development libraries must be installed:
#       (native)  sudo apt-get install libssl-dev
#       (Yocto)   Included in OpenSTLinux SDK
#
# ******************************************************************************

REPO_ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# Ensure the default goal is 'all', regardless of rule ordering below.
.DEFAULT_GOAL := all

# ---------------------------------------------------------------------------
# Toolchain
# ---------------------------------------------------------------------------
# Two supported workflows:
#
#   1. OpenSTLinux SDK (recommended for STM32MP1):
#        source /opt/st/stm32mp1/<ver>/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
#        make
#      The SDK environment-setup script exports CC (with --sysroot, -march, etc.),
#      CFLAGS, LDFLAGS, etc.  Do NOT pass CROSS_COMPILE in this case — the SDK
#      has already set CC correctly.
#
#   2. Generic cross-toolchain (e.g. Linaro, Debian):
#        make CROSS_COMPILE=arm-linux-gnueabihf-
#
# If CC is already set in the environment (origin != "default"), honour it
# unchanged so that the SDK sysroot flags are preserved.
ifeq ($(origin CC), default)
CC := $(CROSS_COMPILE)gcc
endif
ifeq ($(origin AR), default)
AR := $(CROSS_COMPILE)ar
endif
ifeq ($(origin STRIP), default)
STRIP := $(CROSS_COMPILE)strip
endif

# ---------------------------------------------------------------------------
# Directories
# ---------------------------------------------------------------------------
PLATFORM_DIR   := $(REPO_ROOT)/Platform
STSELIB_DIR    := $(REPO_ROOT)/Middleware/STSELib
APPS_UTILS_DIR := $(REPO_ROOT)/Applications/Apps_utils
PROJECTS_DIR   := $(REPO_ROOT)/Applications/Projects
BUILD_DIR      := build

# ---------------------------------------------------------------------------
# List of all example projects (excluding template)
# ---------------------------------------------------------------------------
ALL_EXAMPLES := \
    01_Echo_loop \
    01_Random_number \
    01_Hash \
    01_Device_authentication \
    01_Device_authentication_multi_steps \
    01_Key_pair_generation_NIST_P256 \
    01_Key_pair_generation_NIST_P521 \
    01_Key_pair_generation_BRAINPOOL_P512 \
    01_Key_pair_generation_EDWARDS_25519 \
    01_Secure_data_storage_zone_access \
    01_Secure_data_storage_counter_access \
    02_Command_AC_provisioning \
    02_Host_key_provisioning \
    02_Host_key_provisioning_wrapped \
    03_ECDH \
    03_Key_wrapping \
    04_Symmetric_key_provisioning_control_fields \
    05_Symmetric_key_establishment_compute_AES-128_CMAC \
    05_Symmetric_key_establishment_encrypt_AES-256_CCM \
    05_Symmetric_key_provisioning_wrapped_compute_AES-128_CMAC \
    05_Symmetric_key_provisioning_wrapped_encrypt_AES-256_CCM

# If EXAMPLE is specified on the command line, build only that one
ifdef EXAMPLE
TARGETS := $(EXAMPLE)
else
TARGETS := $(ALL_EXAMPLES)
endif

# ---------------------------------------------------------------------------
# Include paths
# ---------------------------------------------------------------------------
INCLUDES := \
    -I$(PLATFORM_DIR) \
    -I$(PLATFORM_DIR)/STSELib \
    -I$(STSELIB_DIR) \
    -I$(APPS_UTILS_DIR)

# ---------------------------------------------------------------------------
# Preprocessor definitions
#
# Enable all supported ECC curves and hash algorithms so that each example
# can select what it needs via its own stse_conf.h.
# The crypto feature flags below match what is needed across the full example
# set; individual examples that do not use a feature simply leave it uncalled.
# ---------------------------------------------------------------------------
# DEFINES := \
#     -DSTSE_CONF_STSAFE_A_SUPPORT \
#     -DSTSE_CONF_ECC_NIST_P_256 \
#     -DSTSE_CONF_ECC_NIST_P_384 \
#     -DSTSE_CONF_ECC_NIST_P_521 \
#     -DSTSE_CONF_ECC_BRAINPOOL_P_256 \
#     -DSTSE_CONF_ECC_BRAINPOOL_P_384 \
#     -DSTSE_CONF_ECC_BRAINPOOL_P_512 \
#     -DSTSE_CONF_ECC_CURVE_25519 \
#     -DSTSE_CONF_ECC_EDWARD_25519 \
#     -DSTSE_CONF_HASH_SHA_1 \
#     -DSTSE_CONF_HASH_SHA_224 \
#     -DSTSE_CONF_HASH_SHA_256 \
#     -DSTSE_CONF_HASH_SHA_384 \
#     -DSTSE_CONF_HASH_SHA_512 \
#     -DSTSE_CONF_USE_HOST_KEY_ESTABLISHMENT \
#     -DSTSE_CONF_USE_HOST_SESSION \
#     -DSTSE_CONF_USE_SYMMETRIC_KEY_ESTABLISHMENT \
#     -DSTSE_CONF_USE_HOST_KEY_PROVISIONING_WRAPPED \
#     -DSTSE_CONF_USE_SYMMETRIC_KEY_PROVISIONING_WRAPPED

# ---------------------------------------------------------------------------
# Compiler flags
# ---------------------------------------------------------------------------
# OPENSSL_API_COMPAT: Suppress deprecation warnings for APIs available in
# OpenSSL 1.1.1, which remain functional in OpenSSL 3.x. This ensures the
# same source compiles cleanly on both OpenSSL 1.1.x (OpenSTLinux SDK) and
# OpenSSL 3.x (recent host distributions).
#
# Using += so that any --sysroot, -march, -mfpu, -mfloat-abi flags injected
# by the OpenSTLinux SDK environment-setup script are preserved.
CFLAGS += \
    -Wall \
    -Wextra \
    -Wno-unused-parameter \
    -O2 \
    -g \
    -DOPENSSL_API_COMPAT=0x10101000L \
    $(INCLUDES) \
    $(DEFINES)

# ---------------------------------------------------------------------------
# Linker flags
# ---------------------------------------------------------------------------
# Using += to preserve --sysroot and rpath flags from the SDK environment.
LDFLAGS += -lssl -lcrypto -lm

# ---------------------------------------------------------------------------
# Platform source files (shared across all examples)
# ---------------------------------------------------------------------------
PLATFORM_SRCS := \
    $(PLATFORM_DIR)/STSELib/stse_platform_i2c.c \
    $(PLATFORM_DIR)/STSELib/stse_platform_delay.c \
    $(PLATFORM_DIR)/STSELib/stse_platform_aes.c \
    $(PLATFORM_DIR)/STSELib/stse_platform_ecc.c \
    $(PLATFORM_DIR)/STSELib/stse_platform_hash.c \
    $(PLATFORM_DIR)/STSELib/stse_platform_random.c \
    $(PLATFORM_DIR)/STSELib/stse_platform_crc.c \
    $(PLATFORM_DIR)/STSELib/stse_platform_crypto_init.c \
    $(PLATFORM_DIR)/STSELib/stse_platform_power.c \
    $(APPS_UTILS_DIR)/Apps_utils.c

# ---------------------------------------------------------------------------
# STSELib source files (from submodule)
# Discover all .c files in the library directory tree.
# ---------------------------------------------------------------------------
STSELIB_SRCS := $(shell find $(STSELIB_DIR) -name '*.c' 2>/dev/null)

# ---------------------------------------------------------------------------
# Sanity check: verify STSELib submodule is initialized
# ---------------------------------------------------------------------------
.PHONY: check_stselib
check_stselib:
	@if [ -z "$(STSELIB_SRCS)" ]; then \
		echo ""; \
		echo "ERROR: STSELib submodule is not initialized!"; \
		echo "       Please run: git submodule update --init Middleware/STSELib"; \
		echo ""; \
		exit 1; \
	fi

# ---------------------------------------------------------------------------
# Build targets
# ---------------------------------------------------------------------------
.PHONY: all clean $(ALL_EXAMPLES)

all: check_stselib $(addprefix $(BUILD_DIR)/,$(TARGETS))

# Rule to build a single example binary
$(BUILD_DIR)/%: check_stselib
	@mkdir -p $(BUILD_DIR)
	@if [ ! -f "$(PROJECTS_DIR)/$*/main.c" ]; then \
		echo "ERROR: Project '$*' not found at $(PROJECTS_DIR)/$*/main.c"; \
		exit 1; \
	fi
	@echo "Building $* ..."
	$(CC) $(CFLAGS) \
		-I$(PROJECTS_DIR)/$* \
		$(PROJECTS_DIR)/$*/main.c \
		$(PLATFORM_SRCS) \
		$(STSELIB_SRCS) \
		-o $@ \
		$(LDFLAGS)
	@echo "  -> $@ built successfully"

# Convenience target: build a single example via EXAMPLE=<name>
.PHONY: example
example: check_stselib $(BUILD_DIR)/$(EXAMPLE)

clean:
	rm -rf $(BUILD_DIR)

# ---------------------------------------------------------------------------
# Help target
# ---------------------------------------------------------------------------
.PHONY: help
help:
	@echo ""
	@echo "STSAFE-A120 Examples - Linux/STM32MP1 Build System"
	@echo "===================================================="
	@echo ""
	@echo "Targets:"
	@echo "  all              Build all examples (default)"
	@echo "  EXAMPLE=<name>   Build a specific example"
	@echo "  clean            Remove all build artifacts"
	@echo "  help             Show this help message"
	@echo ""
	@echo "Workflows:"
	@echo ""
	@echo "  OpenSTLinux SDK (recommended for STM32MP1):"
	@echo "    source /opt/st/stm32mp1/<ver>/environment-setup-cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi"
	@echo "    make"
	@echo "    -- The SDK sets CC with --sysroot automatically. Do NOT pass CROSS_COMPILE."
	@echo ""
	@echo "  Generic cross-toolchain:"
	@echo "    make CROSS_COMPILE=arm-linux-gnueabihf-"
	@echo ""
	@echo "Variables:"
	@echo "  CROSS_COMPILE    Toolchain prefix for generic toolchains (NOT needed with OpenSTLinux SDK)"
	@echo "  EXAMPLE          Build only the specified example (e.g. 01_Echo_loop)"
	@echo ""
	@echo "Available examples:"
	@$(foreach ex,$(ALL_EXAMPLES),echo "  $(ex)";)
	@echo ""
