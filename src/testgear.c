/*
 * Copyright (c) 2012-2014, Martin Lund
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Test Gear client API functions
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "testgear/config.h"
#include "testgear/debug.h"
#include "testgear/message.h"
#include "testgear/tcp.h"
#include "testgear/session.h"

#define SERVER_PORT 8000  // Default TCP server port

char *tg_error;

void init(void)
{
    int i;

    // Initialize session structures
    for (i=0; i<MAX_SESSIONS; i++)
    {
        session[i].allocated = false;
        session[i].read = NULL;
        session[i].write = NULL;
        session[i].close = NULL;
    }
}

// Connection management functions
int tg_connect(char *name)
{
    bool session_available = false;
    char *connection_string;
    char *ip, *port;
    int tcp_port;
    int i;

    pthread_mutex_lock(&session_mutex);

    // Find a free session entry (i)
    for (i=0; i<MAX_SESSIONS; i++)
    {
        if (session[i].allocated == false)
        {
            session_available=true;
            break;
        }
    }

    // Return error if no session can be allocated
    if (session_available == false)
    {
        printf("Error: Too many active sessions!\n");
        goto error;
    }

    connection_string = strdup(name);

    debug_printf("libtestgear v%s\n", VERSION);
    debug_printf("\n");
    debug_printf("Connecting to %s\n", name);

    // Decode connection string
    if (strncmp(connection_string, "tcp://", 6) == 0)
    {
        // TCP connection

        // Decode IP address
        ip = strtok(&connection_string[6], ":");
        if (ip == NULL)
        {
            tg_error = "Invalid IP address";
            goto error;
        }

        // Decode port
        port = strtok(NULL, ":");
        if (port != NULL)
            tcp_port = atoi(port);
        else
            tcp_port = SERVER_PORT;

        // Register tcp I/O functions
        session[i].write = &tcp_write;
        session[i].read = &tcp_read;
        session[i].close = &tcp_close;
        if (tcp_connect(i, ip, tcp_port) < 0)
            goto error;
    } else if (strncmp(connection_string, "serial://", 9) == 0)
    {
        // Serial connection

        // Decode device name
        // Register serial I/O function
    } else if (strncmp(connection_string, "usb://", 6) == 0)
    {
        // USB connection

        // Decode vendor ID
        // Decode product ID
        // Register USB I/O functions
    } else
    {
        tg_error = "Invalid connection string";
        return -1;
    }

    free(connection_string);

    // Claim session
    session[i].allocated = true;

    pthread_mutex_unlock(&session_mutex);

    // Return session handle
    return i;

error:
    pthread_mutex_unlock(&session_mutex);
    return -1;
}

int tg_disconnect(int handle)
{
    pthread_mutex_lock(&session_mutex);
    session[handle].allocated = false;
    pthread_mutex_unlock(&session_mutex);

    return session[handle].close(handle);
}

// Plugin managment functions
int tg_list_plugins(int handle, char *list)
{
    return submit_message(handle, LIST_PLUGINS, NULL, (void *) list, NULL, 0, TIMEOUT);
}

int tg_plugin_load(int handle, const char *name)
{
    return submit_message(handle, PLUGIN_LOAD, name, NULL, NULL, 0, TIMEOUT);
}

int tg_plugin_unload(int handle, const char *name)
{
    return submit_message(handle, PLUGIN_UNLOAD, name, NULL, NULL, 0, TIMEOUT);
}

int tg_plugin_list_properties(int handle, const char *name, char *properties)
{
    return submit_message(handle, PLUGIN_LIST_PROPERTIES, name, (void *) properties, NULL, 0, TIMEOUT);
}

// Variable management functions
int tg_get_char(int handle, const char *name, char *value)
{
    return submit_message(handle, GET_CHAR, name, (void *) value, NULL, 0, TIMEOUT);
}

int tg_get_short(int handle, const char *name, short *value)
{
    return submit_message(handle, GET_SHORT, name, (void *) value, NULL, 0, TIMEOUT);
}

int tg_get_int(int handle, const char *name, int *value)
{
    return submit_message(handle, GET_INT, name, (void *) value, NULL, 0, TIMEOUT);
}

int tg_get_long(int handle, const char *name, long *value)
{
    return submit_message(handle, GET_LONG, name, (void *) value, NULL, 0, TIMEOUT);
}

int tg_get_float(int handle, const char *name, float *value)
{
    return submit_message(handle, GET_FLOAT, name, (void *) value, NULL, 0, TIMEOUT);
}

int tg_get_double(int handle, const char *name, double *value)
{
    return submit_message(handle, GET_DOUBLE, name, (void *) value, NULL, 0, TIMEOUT);
}

int tg_get_string(int handle, const char *name, char *string)
{
    return submit_message(handle, GET_STRING, name, (void *) string, NULL, 0, TIMEOUT);
}

int tg_set_char(int handle, const char *name, char value)
{
    return submit_message(handle, SET_CHAR, name, NULL, (void *) &value, sizeof(char), TIMEOUT);
}

int tg_set_short(int handle, const char *name, short value)
{
    return submit_message(handle, SET_SHORT, name, NULL, (void *) &value, sizeof(short), TIMEOUT);
}

int tg_set_int(int handle, const char *name, int value)
{
    return submit_message(handle, SET_INT, name, NULL, (void *) &value, sizeof(int), TIMEOUT);
}

int tg_set_long(int handle, const char *name, long value)
{
    return submit_message(handle, SET_LONG, name, NULL, (void *) &value, sizeof(long), TIMEOUT);
}

int tg_set_float(int handle, const char *name, float value)
{
    return submit_message(handle, SET_FLOAT, name, NULL, (void *) &value, sizeof(float), TIMEOUT);
}

int tg_set_double(int handle, const char *name, double value)
{
    return submit_message(handle, SET_DOUBLE, name, NULL, (void *) &value, sizeof(double), TIMEOUT);
}

int tg_set_string(int handle, const char *name, char *string)
{
    return submit_message(handle, SET_STRING, name, NULL, (void *) string, strlen(string)+1, TIMEOUT);
}

// Command management function
int tg_run(int handle, const char *name, int *return_value)
{
    return submit_message(handle, RUN, name, (void *) return_value, NULL, 0, TIMEOUT);
}

// Description function
int tg_describe(int handle, const char *name, char *string)
{
    return submit_message(handle, DESCRIBE, name, (void *) string, NULL, 0, TIMEOUT);
}
