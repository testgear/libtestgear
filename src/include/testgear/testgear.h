/*
 * Copyright (c) 2012-2013, Martin Lund
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

#ifndef TESTGEAR_H
#define TESTGEAR_H

/*
 * Test Gear client API
 */

// Connection management functions
int tg_connect(char *name);
int tg_disconnect(int handle);

// Plugin managment functions
int tg_plugin_load(int handle, char *name);
int tg_plugin_unload(int handle, char *name);

// Variable management functions
int tg_get_char(int handle, char *name, char *value);
int tg_get_short(int handle, char *name, short *value);
int tg_get_int(int handle, char *name, int *value);
int tg_get_float(int handle, char *name, float *value);
int tg_get_string(int handle, char *name, char *string);

int tg_set_char(int handle, char *name, char value);
int tg_set_short(int handle, char *name, short value);
int tg_set_int(int handle, char *name, int value);
int tg_set_float(int handle, char *name, float value);
int tg_set_string(int handle, char *name, char *string);

// Run command function
int tg_run(int handle, char *name, int *return_value);

// Descriptive function
int tg_describe(int handle, char *name, char *string);

// Error message holder
extern char *tg_error;

#endif
