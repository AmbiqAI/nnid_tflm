
##### Toolchain Defaults #####
TOOLCHAIN ?= arm-none-eabi
ifeq ($(TOOLCHAIN),arm-none-eabi)
COMPILERNAME := gcc
else ifeq ($(TOOLCHAIN),arm)
COMPILERNAME := clang
endif

BINDIR := build
NESTDIR := nest
SHELL  :=/bin/bash

##### Target Hardware Defaults #####
ifndef BOARD
BOARD  :=apollo4p
endif
BRD   :=$(BOARD)
BOARDROOT = $(firstword $(subst _, ,$(BRD)))
ifndef EVB
EVB    :=evb
endif
PART   = $(BOARDROOT)
CPU    = cortex-m4
FPU    = fpv4-sp-d16
FABI     = hard



##### Extern Library Defaults #####
ifndef AS_VERSION
AS_VERSION := R4.4.1
endif
ifndef TF_VERSION
TF_VERSION := 0264234_Nov_15_2023
endif

# application stack and heap size
ifndef STACK_SIZE_IN_32B_WORDS
STACK_SIZE_IN_32B_WORDS ?= 4096
endif

# If LEGACY MALLOC is 1, a heap of NS_MALLOC_HEAP_SIZE_IN_K
# will be allocated for all NS examples. If LEGACY MALLOC is 0,
# the heap must be allocated by the example, but can differ
# per each example

ifeq ($(LEGACY_MALLOC),1)
	DEFINES+= configAPPLICATION_ALLOCATED_HEAP=0
else
	DEFINES+= configAPPLICATION_ALLOCATED_HEAP=1
endif

ifndef NS_MALLOC_HEAP_SIZE_IN_K
NS_MALLOC_HEAP_SIZE_IN_K ?= 32
endif

##### TinyUSB Default Config #####
DEFINES+= CFG_TUSB_MCU=OPT_MCU_APOLLO4
# DEFINES+= BOARD_DEVICE_RHPORT_SPEED=OPT_MODE_HIGH_SPEED

##### BLE Defines
## BLE is only supported by neuralSPOT for AmbiqSuite R4.3.0 and later
ifeq ($(AS_VERSION),R4.3.0)
	BLE_SUPPORTED := $(BLE_PRESENT)
	ifeq ($(BLE_SUPPORTED),1)
		DEFINES+= NS_BLE_SUPPORTED
	endif
else ifeq ($(AS_VERSION),R4.4.1)
	BLE_SUPPORTED := $(BLE_PRESENT)
	ifeq ($(BLE_SUPPORTED),1)
		DEFINES+= NS_BLE_SUPPORTED
	endif
else
	BLE_SUPPORTED := 0
endif


##### Common AI Precompiler Directives #####
# 1 = load TF library with debug info, turn on TF debug statements
MLDEBUG ?= 0

ifeq ($(TF_VERSION),d5f819d_Aug_10_2023)
	DEFINES+= NS_TFSTRUCTURE_RECENT
endif
ifeq ($(TF_VERSION),0264234_Nov_15_2023)
	DEFINES+= NS_TFSTRUCTURE_RECENT
endif
ifeq ($(TF_VERSION),fecdd5d)
	DEFINES+= NS_TFSTRUCTURE_RECENT
endif

