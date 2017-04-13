/*****************************************************************************
*
*  exosite_pal_async.h - Common header for Exosite hardware adapation layer
*  Copyright (C) 2012-2017 Exosite LLC
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Exosite LLC nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/
#ifndef __EXOSITE_PAL_ASYNC__
#define __EXOSITE_PAL_ASYNC__
#include <sys/types.h>
#include <stdint.h>

// Utility PAL
uint8_t exoPal_itoa(int value, char* buf, uint8_t bufSize);
int32_t exoPal_atoi(char* val);
size_t exoPal_strlen(const char *s);
char* exoPal_strstr(const char *str, const char *target);
void * exoPal_memmove(void* dst, const void * src, size_t length);
void * exoPal_memset(void* dst, int c, uint16_t length);
size_t exoPal_strlcpy(char* dst, const char* src, size_t len);
size_t exoPal_strlcat(char* dst, const char* src, size_t len);

#ifndef MIN
/** Macro for the least of the two */
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif /*MIN*/

// Memory/NVRAM/Flash PAL
uint8_t exoPal_setCik(const char * read_buffer);
uint8_t exoPal_getCik(char * read_buffer);
uint8_t exoPal_getVendor(char * read_buffer);
uint8_t exoPal_getModel(char * read_buffer);
uint8_t exoPal_getUuid(char * read_buffer);


// Async Sockets PAL.
typedef struct exoPal_state_s exoPal_state_t;
typedef struct exoPal_ops_s exoPal_ops_t;

/** \brief Status callback
 * \param[in,out] state The PAL state.
 * \param[in] status THe status of the operation
 */
typedef int (*exoPal_status_cb) (exoPal_state_t *state, int status);

/** \brief Data Received callback
 * \param[in,out] state The PAL state.
 * \param[in] data The data just read
 * \param[in] len How much data
 */
typedef int (*exoPal_data_cb) (exoPal_state_t *state, const char *data, size_t len);

struct exoPal_ops_s {
    exoPal_status_cb on_start_complete;
    exoPal_status_cb on_connected; // on_socket_opened.
    exoPal_status_cb on_send_complete;
    exoPal_data_cb   on_recv;
    exoPal_status_cb on_socket_closed;
};

enum exoPal_state_e {
    exoPal_state_uninitalized = 0,
    exoPal_state_initalized,
};
struct exoPal_state_s {
    enum exoPal_state_e state; //!< State that this PAL is in
    exoPal_ops_t ops; //!< Table of callback functions.
    void *context; //!< This is used to store Exosite_state_t

    // Contents here are specific to each PAL.
};


void exoPal_init(exoPal_state_t *state);
int exoPal_start(exoPal_state_t *state, const char *host);
int exoPal_stop(exoPal_state_t *state);

int exoPal_tcpSocketOpen(exoPal_state_t *state);
int exoPal_tcpSocketClose(exoPal_state_t *state);
int exoPal_socketWrite(exoPal_state_t *state, const char * buffer, uint16_t len);
int exoPal_socketRead(exoPal_state_t *state, char *buffer, uint16_t bufSize);
int exoPal_sendingComplete(exoPal_state_t *state);

#endif /*__EXOSITE_PAL_ASYNC__*/

/* vim: set ai cin et sw=4 ts=4 : */
