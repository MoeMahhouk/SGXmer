set(UNTRUSTED_DIR		    "${CMAKE_CURRENT_SOURCE_DIR}")
set(APP_INCLUDE_PATHS 		 ${UNTRUSTED_DIR} ${SGXSDK_INCLUDE_DIRS} ${COMMON_INCLUDE_DIR})

set(CMAKE_C_FLAGS 		    "${CMAKE_C_FLAGS} ${SGX_COMMON_CFLAGS} -fPIC -fpie -fstack-protector -Wformat -Wformat-security -Wno-attributes")
set(CMAKE_CXX_FLAGS 		"${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -std=c++11")

set(APP_Link_Flags		    "${SGX_COMMON_CFLAGS} ${VMLIB_UNTRUSTED} -L${SGX_LIBRARY_PATH} -l${SGX_URTS_LIB} -l${SGX_USVC_LIB} -lpthread")



file(GLOB UNTRUSTED_CPP_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

file(GLOB UNTRUSTED_C_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c")


set(APP_SRC_FILES 		     ${UNTRUSTED_CPP_FILES} ${UNTRUSTED_C_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/Enclave_u.c)

set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        "${UNTRUSTED_DIR}/Enclave_u.c"
        "${UNTRUSTED_DIR}/Enclave_u.h"
        )

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Enclave_u.c
        MAIN DEPENDS ../Enclave/Enclave.edl
        COMMAND ${SGX_EDGER8R} --untrusted ../Enclave/Enclave.edl
        --search-path ../Enclave/ 
        --search-path ${SGXSDK_INCLUDE_DIRS} 
        --search-path ${WAMR_DIR}/core/shared/platform/linux-sgx
        COMMENT "Generating TestEnclave_u.{c,h}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${APP_NAME} ${APP_SRC_FILES})

set_target_properties(${APP_NAME}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(${APP_NAME} PUBLIC
        ${APP_INCLUDE_PATHS}
)

target_link_libraries(${APP_NAME}   ${APP_Link_Flags})

add_dependencies(${APP_NAME}        ${ENCLAVE_LIB_NAME})

