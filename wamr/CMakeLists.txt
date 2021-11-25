set (WAMR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/runtime)
set (WAMR_EMBEDDING_API_INCLUDE_PATH  ${WAMR_ROOT_DIR}/core/iwasm/include)

set (WAMR_BUILD_PLATFORM "linux-sgx")
set (WAMR_BUILD_TARGET "X86_64")
set (WAMR_BUILD_INTERP 1)
set (WAMR_BUILD_FAST_INTERP 1)
set (WAMR_BUILD_AOT 1)
set (WAMR_BUILD_JIT 0)
set (WAMR_BUILD_LIBC_BUILTIN 1)
set (WAMR_BUILD_LIBC_WASI 1)
set (WAMR_BUILD_LIB_PTHREAD 1)
set (WAMR_BUILD_SIMD 0)
set (WAMR_BUILD_MULTI_MODULE 1)


set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -ffunction-sections -fdata-sections \
                                     -Wall -Wno-unused-parameter -Wno-pedantic \
                                     -nostdinc -fvisibility=hidden -fpie" )

include (${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
add_library(vmlib STATIC ${WAMR_RUNTIME_LIB_SOURCE})

add_custom_command (
             OUTPUT libvmlib_untrusted.a
             COMMAND mkdir -p untrusted && cd untrusted &&
                     ${CMAKE_C_COMPILER} -c ${PLATFORM_SHARED_SOURCE_UNTRUSTED}
             COMMAND ${CMAKE_AR} rc libvmlib_untrusted.a untrusted/*.o
             COMMAND cp libvmlib_untrusted.a ${CMAKE_BINARY_DIR}/libvmlib_untrusted.a)

add_custom_target (vmlib_untrusted ALL DEPENDS libvmlib_untrusted.a)
