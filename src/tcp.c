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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include "testgear/config.h"
#include "testgear/tcp.h"
#include "testgear/debug.h"

int server_socket;

int tcp_connect(const char *hostname, int port)
{
    struct sockaddr_in server_address;
    struct hostent *host;

    // Create a reliable stream socket using TCP/IP
    if ((server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        perror("Error: socket() call failed");
        exit(-1);
    }

    // Construct the server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(port);
    server_address.sin_addr.s_addr = inet_addr(hostname);

    if (server_address.sin_addr.s_addr == (unsigned long) INADDR_NONE)
    {
        // Look up host address
        host = gethostbyname(hostname);

        if(host == (struct hostent *) NULL)
        {
            perror("Error: Host not found");
            printf("h_errno = %d\n", h_errno);
            close(server_socket);
            exit(-1);
        }

        memcpy(&server_address.sin_addr, host->h_addr, sizeof(server_address.sin_addr));
    }

    // Establish connection to the server
    if (connect(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
    {
        perror("Error: connect() call failed");
        close(server_socket);
        exit(-1);
    }
    else
        debug_printf("Connection established successfully\n");

    return 0;
}

int tcp_close(void)
{
    close(server_socket);
    return 0;
}

void tcp_dump_data(void *data, int length)
{
    int i;
    
    for (i=0; i<length; i++)
    {
        if ((i%10 == 0) && (i !=0 ))
        {
            debug_printf_raw("\n");
            debug_printf("%32s"," ");
        }
        debug_printf_raw("0x%02x ", ((unsigned char *)data)[i]);
    }
}

int tcp_write(void *buffer, int length)
{
    // Debug
    debug_printf("Sending TCP data (%4d bytes):  ", length);
    tcp_dump_data(buffer, length);
    debug_printf_raw("\n");

    // FIXME: introduce timeout handling
    return write(server_socket, buffer, length);
}

int tcp_read(void *buffer, int length)
{
    int size;

    size = read(server_socket, buffer, length);

    // Debug
    if (size)
    {
        debug_printf("Received TCP data (%4d bytes): ", size);
        tcp_dump_data(buffer, size);
        debug_printf_raw("\n");
    }

    // FIXME: introduce timeout handling
    return size;
}
