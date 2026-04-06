cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME Smart_Watch)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# SystemView：FreeRTOSConfig.h 末尾包含 SEGGER_SYSVIEW_FreeRTOS.h，需对 stm32cubemx 可见
target_include_directories(stm32cubemx INTERFACE
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/SEGGER"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/Config"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/Sample/FreeRTOSV10.4"
)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

set(SW_USER_SOURCES
    "${CMAKE_SOURCE_DIR}/User/User_Init.c"
    "${CMAKE_SOURCE_DIR}/User/User_LED/User_LED.c"
    "${CMAKE_SOURCE_DIR}/User/User_IWDG/User_IWDG.c"
    "${CMAKE_SOURCE_DIR}/System/User_Periph_Setup.c"
    "${CMAKE_SOURCE_DIR}/Bsp/Platform/drv_adapter_led/drv_adapter_led.c"
    "${CMAKE_SOURCE_DIR}/Bsp/Platform/drv_adapter_port_led/drv_adapter_port_led.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/shared/src/osal_timer.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/shared/src/osal_task.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/shared/src/osal_sema.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/shared/src/osal_queue.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/shared/src/osal_mutex.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/shared/src/osal_heap.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/FreeRTOS/src/os_impl_timer.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/FreeRTOS/src/os_impl_task.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/FreeRTOS/src/os_impl_sema.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/FreeRTOS/src/os_impl_queue.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/FreeRTOS/src/os_impl_mutex.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/FreeRTOS/src/os_impl_heap.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/Log/Debug/Debug.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/Log/easylogger/src/elog.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/Log/easylogger/src/elog_utils.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/Log/easylogger/src/elog_buf.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/Log/easylogger/port/elog_port.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/Cmbacktrace/cm_backtrace/cm_backtrace.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/Cmbacktrace/cm_backtrace/fault_handler/gcc/cmb_fault.S"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/SEGGER/SEGGER_RTT.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/SEGGER/SEGGER_RTT_ASM_ARMv7M.S"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/SEGGER/SEGGER_RTT_printf.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/SEGGER/SEGGER_SYSVIEW.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/Sample/FreeRTOSV10.4/SEGGER_SYSVIEW_FreeRTOS.c"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/Sample/FreeRTOSV10.4/Config/Cortex-M/SEGGER_SYSVIEW_Config_FreeRTOS.c"
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${SW_USER_SOURCES}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/User"
    "${CMAKE_SOURCE_DIR}/User/User_LED"
    "${CMAKE_SOURCE_DIR}/User/User_IWDG"
    "${CMAKE_SOURCE_DIR}/System"
    "${CMAKE_SOURCE_DIR}/Bsp/Platform/drv_adapter_led"
    "${CMAKE_SOURCE_DIR}/Bsp/Platform/drv_adapter_port_led"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/inc"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/shared/inc"
    "${CMAKE_SOURCE_DIR}/Middlewares/os_adapter/FreeRTOS/inc"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/Log/Debug"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/Log/easylogger/inc"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/Cmbacktrace/cm_backtrace"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/SEGGER"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/Config"
    "${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/SystemView/Sample/FreeRTOSV10.4"
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# 生成 bin
add_custom_command(TARGET Smart_Watch POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary
    $<TARGET_FILE:Smart_Watch>
    ${CMAKE_BINARY_DIR}/Smart_Watch.bin
)

# 生成 hex
add_custom_command(TARGET Smart_Watch POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex
    $<TARGET_FILE:Smart_Watch>
    ${CMAKE_BINARY_DIR}/Smart_Watch.hex
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
