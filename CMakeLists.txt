cmake_minimum_required(VERSION 3.10)

project(skale_gas_miner)

# =============================
# NDK path and cpu-features.h
# =============================
if(NOT DEFINED ANDROID_NDK)
    if(DEFINED ENV{ANDROID_NDK})
        set(ANDROID_NDK $ENV{ANDROID_NDK})
    else()
        message(FATAL_ERROR "ANDROID_NDK must be set!")
    endif()
endif()

# =============================
# Include directories
# =============================
include_directories(
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/helpers"
    "${CMAKE_SOURCE_DIR}/lib/cryptopp"
    "${ANDROID_NDK}/sources/android/cpufeatures"
)

# =============================
# Collect Crypto++ source files
# =============================
file(GLOB CRYPTOPP_SOURCES
    "${CMAKE_SOURCE_DIR}/lib/cryptopp/*.cpp"
)

# Remove SIMD and hardware-accelerated files that require special CPU features
list(REMOVE_ITEM CRYPTOPP_SOURCES
    "${CMAKE_SOURCE_DIR}/lib/cryptopp/gcm_simd.cpp"
    "${CMAKE_SOURCE_DIR}/lib/cryptopp/gf2n_simd.cpp"
    "${CMAKE_SOURCE_DIR}/lib/cryptopp/crc_simd.cpp"
    "${CMAKE_SOURCE_DIR}/lib/cryptopp/rijndael_simd.cpp"
    "${CMAKE_SOURCE_DIR}/lib/cryptopp/sha_simd.cpp"
)

add_library(cryptopp STATIC ${CRYPTOPP_SOURCES})

target_compile_definitions(cryptopp PRIVATE
    CRYPTOPP_DISABLE_ASM
    CRYPTOPP_DISABLE_ARM_NEON
    CRYPTOPP_DISABLE_ARM_CRC32
    CRYPTOPP_DISABLE_ARM_PMULL
    CRYPTOPP_DISABLE_ARM_SHA
    CRYPTOPP_DISABLE_ARM_AES
)

# =============================
# Collect all helper .cpp files
# =============================
file(GLOB HELPER_SOURCES
    "${CMAKE_SOURCE_DIR}/helpers/*.cpp"
)

add_library(skale_gas_miner SHARED
    miner.cpp
    dllmain.cpp
    ${HELPER_SOURCES}
)

# =============================
# CPU features for runtime detection
# =============================
add_library(cpufeatures STATIC
    "${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c"
)

# =============================
# Android system log lib
# =============================
find_library(log-lib log)

# =============================
# Link all libraries together
# =============================
target_link_libraries(skale_gas_miner
    cryptopp
    cpufeatures
    ${log-lib}
)

# =============================
# Ensure skale_gas_miner sees cryptopp headers
# =============================
target_include_directories(skale_gas_miner PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/lib/cryptopp
)

set_target_properties(skale_gas_miner PROPERTIES OUTPUT_NAME "Cryptopia.SkaleMiner")
