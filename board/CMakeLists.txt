cmake_minimum_required(VERSION 3.6)

set(ARM_NONE_EABI_TOOLCHAIN_PATH "/usr")
set(NRF_TARGET "nrf52")

set(NRF5_SDK_PATH "/media/pieter/data/iot/DeviceDownload/nRF5SDK160098a08e2")
set(NRFJPROG "/usr/bin/nrfjprog")

set(SOFTDEVICE_PATH "${NRF5_SDK_PATH}/components/softdevice/s132/hex/s132_nrf52_6.3.1_softdevice.hex")
include("cmake-nRF5x/CMake_nRF5x.cmake")

# must be called before first project call or add_subdirectory unless passing on command line
nRF5x_toolchainSetup()

project(IoTProject C ASM)

nRF5x_setup()

nRF5x_addAppButton()
nRF5x_addAppScheduler()
nRF5x_addAppTimer()
nRF5x_addBLEGATT()
nRF5x_addBSP(TRUE FALSE FALSE)

add_definitions(-DDEVICE_NAME="${DEVICE_NAME}")
add_definitions(-DCONFIG_GPIO_AS_PINRESET)

FILE(GLOB SOURCES_SRC "src/*.c")

include_directories("./include")
list(APPEND SOURCE_FILES "main.c" "${SOURCES_SRC}")

nRF5x_addExecutable(${PROJECT_NAME} "${SOURCE_FILES}")