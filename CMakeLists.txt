cmake_minimum_required (VERSION 2.8...3.16)
project(SGXmer "C" "CXX")

add_subdirectory(wamr)
set (WAMR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wamr/runtime)
set (VMLIB_UNTRUSTED         "${CMAKE_BINARY_DIR}/libvmlib_untrusted.a")

set (WASI_VERSION 12)
set (WASI_VERSION_FULL ${WASI_VERSION}.0)
set (WASI_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wasi-sdk)
set (WASI_CLANG ${WASI_SDK_DIR}/bin/clang)
set (WASI_SYSROOT ${WASI_SDK_DIR}/share/wasi-sysroot)
if (NOT EXISTS ${WASI_SDK_DIR})    
    execute_process(
        # Downloading the wasi sdk files
        COMMAND wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${WASI_VERSION}/wasi-sdk-${WASI_VERSION_FULL}-linux.tar.gz
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_QUIET
        ERROR_QUIET
    )
    execute_process(
         # Creating directory for wasi sdk and extracting the downloaded files
         COMMAND mkdir -p ${WASI_SDK_DIR}
         COMMAND tar -zxvf wasi-sdk-${WASI_VERSION_FULL}-linux.tar.gz -C ${WASI_SDK_DIR} --strip 1
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
         OUTPUT_QUIET
         ERROR_QUIET
     )
     execute_process(
        # Cleaning up after installation
         COMMAND rm wasi-sdk-${WASI_VERSION_FULL}-linux.tar.gz
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
         OUTPUT_QUIET
         ERROR_QUIET
     )
endif()



include(cmake/FindSGXSDK.cmake)
include(cmake/ConfigSGX.cmake)

set(COMMON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Include")

set(ENCLAVE_LIB_NAME            SimpleEnclave)
set(SIGNED_ENCLAVE_NAME         SimpleEnclave.signed.so)
set(APP_NAME                    SimpleSGXApp)
#set(Security_Link_Flags			    "-Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -pie")


add_definitions(-DENCLAVE_FILENAME="${SIGNED_ENCLAVE_NAME}")



add_subdirectory(Enclave)
add_subdirectory(App)