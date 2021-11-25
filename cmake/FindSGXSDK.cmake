
if(EXISTS SGX_DIR)
set(SGX_SDK ${SGX_DIR})
elseif(EXISTS SGX_ROOT)
set(SGX_SDK ${SGX_ROOT})
elseif(EXISTS $ENV{SGX_SDK})
set(SGX_SDK $ENV{SGX_SDK})
elseif(EXISTS $ENV{SGX_DIR})
set(SGX_SDK $ENV{SGX_DIR})
elseif(EXISTS $ENV{SGX_ROOT})
set(SGX_SDK $ENV{SGX_ROOT})
else()
set(SGX_SDK "/opt/intel/sgxsdk")
endif()

FIND_PATH(SGXSDK_INCLUDE_DIRS sgx.h "${SGX_SDK}/include")
FIND_LIBRARY(SGXSDK_LIBRARIES libsgx_urts.so "${SGX_SDK}/lib64")

# handle the QUIETLY and REQUIRED arguments and set LibODB_FOUND to TRUE
# if all listed variables are TRUE, hide their existence from configuration view
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SGXSDK DEFAULT_MSG
    SGXSDK_INCLUDE_DIRS SGXSDK_LIBRARIES)

MARK_AS_ADVANCED(SGXSDK_LIBRARIES SGXSDK_INCLUDE_DIRS)
