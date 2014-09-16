/*
 * Test Gear library test application
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "testgear/testgear.h"

#define TIMEOUT 1000 // ms
#define LOOPS 1

// Available tests
#define DUMMY_TEST
//#define FB_TEST
//#define SHELL_TEST
//#define AUDIO_TEST

// Test specific settings
#define FB_DELAY 1000000 // ms

void error(char *msg)
{
    printf("Error: %s\n", tg_error);
    exit(-1);
}

int main(int argc, char *argv[])
{
    int cd;
    int status;
    int ret;
    char description[256] = "";
    char plugins[256] = "";
    int loop=LOOPS;

    if (argc != 2)
    {
        printf("Usage: %s <ip>\n", argv[0]);
        exit(1);
    }

    // Connect to test gear device server
    cd = tg_connect(argv[1]);
    if (cd < 0)
        error(tg_error);

    // List available plugins
    tg_list_plugins(cd, (char *) &plugins);
    printf("Available plugins: %s\n", plugins);

    while (loop--)
    {
        printf("LOOP=%d\n", loop);
#ifdef DUMMY_TEST

        // Dummy plugin parameters
        struct
        {
            char char0;
            short short0;
            int int0;
            float float0;
            char string0[256];
        } dummy;

        char properties[1024] = "";

        // Load dummy plugin
        status = tg_plugin_load(cd, "dummy");
        if (status != 0)
            error(tg_error);


        // List properties
        status = tg_plugin_list_properties(cd, "dummy", (char *) &properties);
        if (status != 0)
            error(tg_error);
        printf("dummy properties: %s\n", properties);

        // Set dummy char
        status = tg_set_char(cd, "dummy.char0", 42);
        if (status != 0)
            error(tg_error);

        // Get dummy char
        status = tg_get_char(cd, "dummy.char0", &dummy.char0);
        if (status != 0)
            error(tg_error);
        else
            printf("dummy.char0 = %d\n", dummy.char0);

        // Set dummy short
        status = tg_set_short(cd, "dummy.short0", 4242);
        if (status != 0)
            error(tg_error);

        // Get dummy short
        status = tg_get_short(cd, "dummy.short0", &dummy.short0);
        if (status != 0)
            error(tg_error);
        else
            printf("dummy.short0 = %d\n", dummy.short0);

        // Set dummy int
        status = tg_set_int(cd, "dummy.int0", 424242);
        if (status != 0)
            error(tg_error);

        // Get dummy int
        status = tg_get_int(cd, "dummy.int0", &dummy.int0);
        if (status != 0)
            error(tg_error);
        else
            printf("dummy.int0 = %d\n", dummy.int0);

        // Set dummy float
        status = tg_set_float(cd, "dummy.float0", 42.42);
        if (status != 0)
            error(tg_error);

        // Get dummy float
        status = tg_get_float(cd, "dummy.float0", &dummy.float0);
        if (status != 0)
            error(tg_error);
        else
            printf("dummy.float0 = %f\n", dummy.float0);

        // Set dummy string
        status = tg_set_string(cd, "dummy.string0", "Hello world!");
        if (status != 0)
            error(tg_error);

        // Get dummy string
        status = tg_get_string(cd, "dummy.string0", (char *) &dummy.string0);
        if (status != 0)
            error(tg_error);
        else
            printf("dummy.string0 = %s\n", dummy.string0);

        // Run dummy command
        status = tg_run(cd, "dummy.command0", &ret);
        if (status != 0)
            error(tg_error);
        else
            printf("command0() returned %d\n", ret);


        // Unload dummy plugin
        status = tg_plugin_unload(cd, "dummy");
        if (status != 0)
            error(tg_error);

#endif // SHELL_TEST



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
        if (status != 0)
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
        if (status != 0)
            error(tg_error);
        printf("Description of fb: %s\n", description);

        status = tg_describe(cd, "fb.set_resolution", (char *) &description);
        if (status != 0)
            error(tg_error);
        printf("Description of fb.set_resolution: %s\n", description);


        // Query various plugin variables
        status = tg_get_string(cd, "fb.filename", (char *) &fb.filename);
        if (status != 0)
            error(tg_error);
        printf("fb.filename: %s\n", fb.filename);

        status = tg_get_int(cd, "fb.xres", &fb.xres);
        if (status != 0)
            error(tg_error);
        printf("fb.xres: %d\n", fb.xres);

        status = tg_get_int(cd, "fb.yres", &fb.yres);
        if (status != 0)
            error(tg_error);
        printf("fb.xres: %d\n", fb.yres);


        // Configure framebuffer device
        status = tg_set_string(cd, "fb.device", "/dev/fb0");
        if (status != 0)
            error(tg_error);

        status = tg_set_int(cd, "fb.depth", 32);
        if (status != 0)
            error(tg_error);

        status = tg_run(cd, "fb.set_depth", &ret);
        if (status != 0)
            error(tg_error);

        status = tg_set_int(cd, "fb.xres", 1024);
        if (status != 0)
            error(tg_error);

        status = tg_set_int(cd, "fb.yres", 768);
        if (status != 0)
            error(tg_error);

        status = tg_run(cd, "fb.set_resolution", &ret);
        if (status != 0)
            error(tg_error);


        // Draw patterns
        status = tg_run(cd, "fb.draw_pattern", &ret);
        if (status != 0)
            error(tg_error);

        usleep(FB_DELAY);

        status = tg_set_int(cd, "fb.pattern", 1);
        if (status != 0)
            error(tg_error);

        status = tg_run(cd, "fb.draw_pattern", &ret);
        if (status != 0)
            error(tg_error);

        usleep(FB_DELAY);

        status = tg_set_int(cd, "fb.pattern", 2);
        if (status != 0)
            error(tg_error);

        status = tg_run(cd, "fb.draw_pattern", &ret);
        if (status != 0)
            error(tg_error);

        usleep(FB_DELAY);

        status = tg_set_int(cd, "fb.pattern", 3);
        if (status != 0)
            error(tg_error);

        status = tg_run(cd, "fb.draw_pattern", &ret);
        if (status != 0)
            error(tg_error);

        usleep(FB_DELAY);

        status = tg_set_int(cd, "fb.pattern", 4);
        if (status != 0)
            error(tg_error);

        status = tg_run(cd, "fb.draw_pattern", &ret);
        if (status != 0)
            error(tg_error);

        usleep(FB_DELAY);

        // Draw image
        status = tg_set_string(cd, "fb.filename", "test.bmp");
        if (status != 0)
            error(tg_error);
/*
        status = tg_run(cd, "fb.show_image", &ret);
        if (status != 0)
            error(tg_error);
*/


        // Unload framebuffer plugin
        status = tg_plugin_unload(cd, "fb");
        if (status != 0)
            error(tg_error);

#endif // FB_TEST


#ifdef SHELL_TEST

        // Shell plugin parameters
        struct
        {
            char command[256];
        } shell;

        char properties[1024] = "";

        // Load shell plugin
        status = tg_plugin_load(cd, "shell");
        if (status != 0)
            error(tg_error);


        // List properties
        status = tg_plugin_list_properties(cd, "shell", (char *) &properties);
        if (status != 0)
            error(tg_error);
        printf("properties: %s\n", properties);

        // Set shell command string
        status = tg_set_string(cd, "shell.command", "touch bla.txt; exit 42");
        if (status != 0)
            error(tg_error);

        // Run shell command
        status = tg_run(cd, "shell.command", &ret);
        printf("Command returned code %d\n", ret);
        if (status != 0)
            error(tg_error);


        // Unload shell plugin
        status = tg_plugin_unload(cd, "shell");
        if (status != 0)
            error(tg_error);

#endif // SHELL_TEST


#ifdef AUDIO_TEST

        // Load audio plugin
        status = tg_plugin_load(cd, "audio");
        if (status != 0)
            error(tg_error);


        // Set audio device
        status = tg_set_string(cd, "audio.device", "default");
        if (status != 0)
            error(tg_error);

        // Set audio device sampling rate
        status = tg_set_int(cd, "audio.rate", 44100);
        if (status != 0)
            error(tg_error);

        // Set wav test file
        status = tg_set_string(cd, "audio.wav-file", "arriva.wav");
        if (status != 0)
            error(tg_error);

        // Run play wav command
        status = tg_run(cd, "audio.play-wav", &ret);
        if (status != 0)
            error(tg_error);

        // Set wav test file
        status = tg_set_string(cd, "audio.wav-file", "11k16bitpcm.wav");
        if (status != 0)
            error(tg_error);

        // Run play wav command
        status = tg_run(cd, "audio.play-wav", &ret);
        if (status != 0)
            error(tg_error);

        // Set wav test file
        status = tg_set_string(cd, "audio.wav-file", "11k8bitpcm.wav");
        if (status != 0)
            error(tg_error);

        // Run play wav command
        status = tg_run(cd, "audio.play-wav", &ret);
        if (status != 0)
            error(tg_error);

        // Set wav test file
        status = tg_set_string(cd, "audio.wav-file", "santa.wav");
        if (status != 0)
            error(tg_error);

        // Run play wav command
        status = tg_run(cd, "audio.play-wav", &ret);
        if (status != 0)
            error(tg_error);


        // Set tone type
        status = tg_set_string(cd, "audio.tone-type", "sine");
        if (status != 0)
            error(tg_error);

        // Set tone frequency
        status = tg_set_int(cd, "audio.tone-frequency", 200);
        if (status != 0)
            error(tg_error);

        // Set tone time
        status = tg_set_int(cd, "audio.tone-time", 1);
        if (status != 0)
            error(tg_error);

        // Run tone generator command
        status = tg_run(cd, "audio.generate-tone", &ret);
        if (status != 0)
            error(tg_error);

        // Set tone frequency
        status = tg_set_int(cd, "audio.tone-frequency", 400);
        if (status != 0)
            error(tg_error);

        // Run tone generator command
        status = tg_run(cd, "audio.generate-tone", &ret);
        if (status != 0)
            error(tg_error);

        // Set tone frequency
        status = tg_set_int(cd, "audio.tone-frequency", 800);
        if (status != 0)
            error(tg_error);

        // Run tone generator command
        status = tg_run(cd, "audio.generate-tone", &ret);
        if (status != 0)
            error(tg_error);

        // Set tone frequency
        status = tg_set_int(cd, "audio.tone-frequency", 1600);
        if (status != 0)
            error(tg_error);

        // Run tone generator command
        status = tg_run(cd, "audio.generate-tone", &ret);
        if (status != 0)
            error(tg_error);

        // Set tone type
        status = tg_set_string(cd, "audio.tone-type", "noise");
        if (status != 0)
            error(tg_error);

        // Run tone generator command
        status = tg_run(cd, "audio.generate-tone", &ret);
        if (status != 0)
            error(tg_error);


        // Unload audio plugin
        status = tg_plugin_unload(cd, "audio");
        if (status != 0)
            error(tg_error);

#endif // AUDIO_TEST

    }
    // Disconnect
    tg_disconnect(cd);

    return 0;
}
