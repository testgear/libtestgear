/*
 * Test Gear library test application
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "testgear/testgear.h"

#define TIMEOUT 1000 // ms

#define TG_SERVER "127.0.0.1"

// Active tests
//#define FB_TEST
#define SHELL_TEST
#define AUDIO_TEST

void error(char *msg)
{
    printf("Error: %s\n", tg_error);
    exit(-1);
}

int main(void)
{
    int cd;
    int status;
    int ret;
    char description[256];
    int loop=10000;

    // Connect to test gear device server
    cd = tg_connect(TG_SERVER);
    if (cd < 0)
        error(tg_error);

    while (loop--)
    {
#ifdef FB_TEST

        // Framebuffer plugin parameters
        struct
        {
            char version[256];
            char description[256];
            char author[256];
            char license[256];
            char filename[256];
            int xres;
            int yres;
        } fb;


        // Load framebuffer plugin
        status = tg_plugin_load(cd, "fb");
        if (status < 0)
            error(tg_error);


        // Query general plugin information
        status = tg_get_string(cd, "fb.version", (char *) &fb.version);
        printf("fb.version: %s\n", fb.version);

        tg_get_string(cd, "fb.description", (char *) &fb.description);
        printf("fb.description: %s\n", fb.description);

        tg_get_string(cd, "fb.author", (char *) &fb.author);
        printf("fb.author: %s\n", fb.author);

        tg_get_string(cd, "fb.license", (char *) &fb.license);
        printf("fb.license: %s\n", fb.license);


        // Query plugin descriptions
        status = tg_describe(cd, "fb", (char *) &description);
        if (status < 0)
            error(tg_error);
        printf("Description of fb: %s\n", description);

        status = tg_describe(cd, "fb.set_resolution", (char *) &description);
        if (status < 0)
            error(tg_error);
        printf("Description of fb.set_resolution: %s\n", description);


        // Query various plugin variables
        status = tg_get_string(cd, "fb.filename", (char *) &fb.filename);
        if (status < 0)
            error(tg_error);
        printf("fb.filename: %s\n", fb.filename);

        status = tg_get_int(cd, "fb.xres", &fb.xres);
        if (status < 0)
            error(tg_error);
        printf("fb.xres: %d\n", fb.xres);


        // Set various framebuffer parameters
        status = tg_set_string(cd, "fb.device", "/dev/fb0");
        if (status < 0)
            error(tg_error);

        status = tg_set_int(cd, "fb.xres", 1024);
        if (status < 0)
            error(tg_error);

        status = tg_set_int(cd, "fb.yres", 768);
        if (status < 0)
            error(tg_error);

        status = tg_set_int(cd, "fb.pattern", 1);
        if (status < 0)
            error(tg_error);

        status = tg_set_string(cd, "fb.filename", "test.bmp");
        if (status < 0)
            error(tg_error);


        // Query various plugin variables
        status = tg_get_string(cd, "fb.filename", (char *) &fb.filename);
        if (status < 0)
            error(tg_error);
        printf("fb.filename: %s\n", fb.filename);

        status = tg_get_int(cd, "fb.xres", &fb.xres);
        if (status < 0)
            error(tg_error);
        printf("fb.xres: %d\n", fb.xres);


        // Run command
        status = tg_run(cd, "fb.open", &ret);
        if (status < 0)
            error(tg_error);

        status = tg_run(cd, "fb.set_resolution", &ret);
        if (status < 0)
            error(tg_error);

        status = tg_run(cd, "fb.draw_pattern", &ret);
        if (status < 0)
            error(tg_error);

        sleep(3);

        status = tg_set_int(cd, "fb.pattern", 2);
        if (status < 0)
            error(tg_error);

        status = tg_run(cd, "fb.draw_pattern", &ret);
        if (status < 0)
            error(tg_error);

        sleep(3);

        status = tg_set_int(cd, "fb.pattern", 3);
        if (status < 0)
            error(tg_error);

        status = tg_run(cd, "fb.draw_pattern", &ret);
        if (status < 0)
            error(tg_error);

        sleep(3);

        status = tg_set_int(cd, "fb.pattern", 4);
        if (status < 0)
            error(tg_error);

        status = tg_run(cd, "fb.draw_pattern", &ret);
        if (status < 0)
            error(tg_error);

        sleep(3);

        status = tg_set_int(cd, "fb.pattern", 0);
        if (status < 0)
            error(tg_error);

        status = tg_run(cd, "fb.draw_pattern", &ret);
        if (status < 0)
            error(tg_error);


        // Unload framebuffer plugin
        status = tg_plugin_unload(cd, "fb");
        if (status < 0)
            error(tg_error);

#endif // FB_TEST


#ifdef SHELL_TEST

        // Shell plugin parameters
        struct
        {
            char command[256];
        } shell;


        // Load shell plugin
        status = tg_plugin_load(cd, "shell");
        if (status < 0)
            error(tg_error);


        // Set shell command string
        status = tg_set_string(cd, "shell.command", "touch bla.txt; exit 42");
        if (status < 0)
            error(tg_error);

        // Run shell command
        status = tg_run(cd, "shell.command", &ret);
        printf("Command returned code %d\n", ret);
        if (status < 0)
            error(tg_error);


        // Unload shell plugin
        status = tg_plugin_unload(cd, "shell");
        if (status < 0)
            error(tg_error);

#endif // SHELL_TEST


#ifdef AUDIO_TEST

        // Load audio plugin
        status = tg_plugin_load(cd, "audio");
        if (status < 0)
            error(tg_error);


        // Set audio device
        status = tg_set_string(cd, "audio.device", "default");
        if (status < 0)
            error(tg_error);

        // Set audio device sampling rate
        status = tg_set_int(cd, "audio.rate", 44100);
        if (status < 0)
            error(tg_error);

        // Set tone type
        status = tg_set_string(cd, "audio.tone-type", "sine");
        if (status < 0)
            error(tg_error);

        // Set tone frequency
        status = tg_set_int(cd, "audio.tone-frequency", 200);
        if (status < 0)
            error(tg_error);

        // Set tone time
        status = tg_set_int(cd, "audio.tone-time", 1);
        if (status < 0)
            error(tg_error);

        // Run tone generator command
        status = tg_run(cd, "audio.generate-tone", &ret);
        if (status < 0)
            error(tg_error);

        // Set tone frequency
        status = tg_set_int(cd, "audio.tone-frequency", 400);
        if (status < 0)
            error(tg_error);

        // Run tone generator command
        status = tg_run(cd, "audio.generate-tone", &ret);
        if (status < 0)
            error(tg_error);

        // Set tone frequency
        status = tg_set_int(cd, "audio.tone-frequency", 800);
        if (status < 0)
            error(tg_error);

        // Run tone generator command
        status = tg_run(cd, "audio.generate-tone", &ret);
        if (status < 0)
            error(tg_error);

        // Set tone frequency
        status = tg_set_int(cd, "audio.tone-frequency", 1600);
        if (status < 0)
            error(tg_error);

        // Run tone generator command
        status = tg_run(cd, "audio.generate-tone", &ret);
        if (status < 0)
            error(tg_error);

        // Set tone type
        status = tg_set_string(cd, "audio.tone-type", "noise");
        if (status < 0)
            error(tg_error);

        // Run tone generator command
        status = tg_run(cd, "audio.generate-tone", &ret);
        if (status < 0)
            error(tg_error);


        // Unload audio plugin
        status = tg_plugin_unload(cd, "audio");
        if (status < 0)
            error(tg_error);

#endif // AUDIO_TEST

    }
    // Disconnect
    tg_disconnect(cd);

    return 0;
}
