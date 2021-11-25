set(ENCLAVE_DIR					"${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB TRUSTED_CPP_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

file(GLOB TRUSTED_C_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

set(Trusted_Include_Paths		${ENCLAVE_DIR} ${COMMON_INCLUDE_DIR} ${SGXSDK_INCLUDE_DIRS} ${SGXSDK_INCLUDE_DIRS}/tlibc ${SGX_LIBCXX_PATH})
set(TRUSTED_SRCS				${TRUSTED_CPP_FILES} ${TRUSTED_C_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/Enclave_t.c)

set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        "${ENCLAVE_DIR}/Enclave_t.c"
        "${ENCLAVE_DIR}/Enclave_t.h")


set(COMMON_C_CPP_FLAGS			"${SGX_COMMON_CFLAGS} -nostdinc -fvisibility=hidden -fpic -fpie -fstack-protector -fno-builtin-printf -Wformat -Wformat-security")
set(CMAKE_C_FLAGS 				"${CMAKE_C_FLAGS} ${COMMON_C_CPP_FLAGS} -Wno-implicit-function-declaration -std=c11")
set(CMAKE_CXX_FLAGS 			"${CMAKE_CXX_FLAGS} ${COMMON_C_CPP_FLAGS} -std=c++11 -nostdinc++")

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Enclave_t.c
        MAIN DEPENDS Enclave.edl
        COMMAND ${SGX_EDGER8R} --trusted Enclave.edl
        --search-path ${SGXSDK_INCLUDE_DIRS}
        --search-path ${CMAKE_CURRENT_SOURCE_DIR}
        --search-path ${WAMR_DIR}/core/shared/platform/linux-sgx
        COMMENT "Generating Enclave_t.{c,h}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${ENCLAVE_LIB_NAME} SHARED ${TRUSTED_SRCS})


include_directories(${Trusted_Include_Paths})
target_link_libraries(${ENCLAVE_LIB_NAME} vmlib)

target_link_libraries(${ENCLAVE_LIB_NAME} "${SGX_COMMON_CFLAGS} \
        -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles \
        -L${SGX_LIBRARY_PATH} \
        -Wl,--whole-archive -l${SGX_TRTS_LIB} -Wl,--no-whole-archive \
        -Wl,--start-group -lsgx_tstdc -lsgx_tcxx -lsgx_tcrypto -lsgx_pthread -l${SGX_TSVC_LIB} -Wl,--end-group \
        -Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
        -Wl,-pie,-eenclave_entry -Wl,--export-dynamic \
        -Wl,--defsym,__ImageBase=0 \
        -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/Enclave.lds")

add_custom_command(TARGET ${ENCLAVE_LIB_NAME}
    POST_BUILD
    COMMAND ${SGX_ENCLAVE_SIGNER} sign
    -key ${CMAKE_CURRENT_SOURCE_DIR}/Enclave_private_test.pem
    -config ${CMAKE_CURRENT_SOURCE_DIR}/Enclave.config.xml
    -enclave Enclave/lib${ENCLAVE_LIB_NAME}.so     #ToDo was libTestEnclave.so before. change this later to be also global variable or so
    -out ${SIGNED_ENCLAVE_NAME}
    BYPRODUCTS ${SIGNED_ENCLAVE_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Signing the enclave => ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${SIGNED_ENCLAVE_NAME}")
