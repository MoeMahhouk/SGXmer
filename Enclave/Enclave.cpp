/*
 * Copyright (C) 2011-2021 Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "Enclave.h"
#include "Enclave_t.h" /* print_string */
#include <cstdlib>
#include <stdarg.h>
#include <stdio.h> /* vsnprintf */
#include <string.h>
#include <inttypes.h>
#include "wasm_export.h"
#include "bh_platform.h"


/* 
 * printf: 
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */
int printf(const char* fmt, ...)
{
    char buf[BUFSIZ] = { '\0' };
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print(buf);
    return (int)strnlen(buf, BUFSIZ - 1) + 1;
}


static void module_destroyer_cb(uint8_t *buffer, uint32_t size)
{
    sgx_status_t ret = SGX_SUCCESS;
    ret = ocall_free((uint64_t)buffer);
    if(ret != SGX_SUCCESS)
    {
        printf("Failed to free the module's buffer with error code: %d\n", ret);
    }
}

static bool module_reader_cb(const char *module_name, uint8_t **p_buffer, uint32_t *p_size)
{   
    sgx_status_t ret = SGX_SUCCESS;
    ret = ocall_read_file_to_buffer(p_buffer,module_name, p_size);
    if(ret != SGX_SUCCESS)
    {
        printf("Failed to read file from buffer with error code: %d\n", ret);
        return false;
    }
    if(p_buffer == NULL)
    {
        printf("failed to load the module in the buffer from the ocall\n");
        return false;
    }

    return true;
}


static void register_native_symbols()
{
    static NativeSymbol native_symbols[] = 
    {
        //{
        //    "my_printf", 		        // the name of WASM function name
        //    (void *)my_printf,          // the native function pointer
        //    "($)",		                // the function prototype signature
        //    NULL                        // Attribute
        //}
    };

    // Registering the native
    int n_native_symbols = sizeof(native_symbols) / sizeof(NativeSymbol);
    if (!wasm_runtime_register_natives("env", native_symbols, n_native_symbols))
    {   
        abort();
    } else {
        LOG_VERBOSE("native symbols have been registered successfully!\n");
    }

}

void ecall_set_log_level(int log_level) 
{
    #if WASM_ENABLE_LOG != 0
    LOG_VERBOSE("Set log verbose level to %d.\n", log_level);
    bh_log_set_verbose_level(log_level);
    #endif
}


void enclave_print(const char *message)
{
    ocall_print(message);
}


void ecall_init_runtime() 
{
    os_set_print_function(enclave_print);
    if(!wasm_runtime_init())
    {
        printf("Error: failed to initialise the wasm runtime!\n");
        abort();
    }
    LOG_VERBOSE("Init runtime environment success.\n");
    wasm_runtime_set_module_reader(module_reader_cb, module_destroyer_cb);
    LOG_VERBOSE("Set module reader and destroyer success .\n");

}

void ecall_destroy_runtime()
{
    wasm_runtime_destroy();
    LOG_VERBOSE("Destroy runtime success.\n");
}
