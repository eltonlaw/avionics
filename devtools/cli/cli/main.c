#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "buildtools.h"

typedef int (*command_fn)(int, char**);

int help(int argc, char **argv);

typedef struct {
    char *type;
    command_fn fn;
    char *description;
} command;

int serial(int argc, char **argv) {
    return system("ipython -i -c \"%load devtools/serial_interactive.py\"");
}

int avr_gcc(int argc, char **argv) {
    return system("avr-gcc -print-search-dirs");
}

int fc1(int argc, char **argv) {
    return make(argc, argv, "./build/fc1");
}

int fc3(int argc, char **argv) {
    int err;
    if (0 != (err = cmake("fc3"))) {
        fprintf(stderr, "Error running cmake: %s\n", strerror(err));
        return err;
    }
    if (0 != (err = system("cp build/fc3/compile_commands.json ."))) {
        fprintf(stderr, "Error copying compile_commands.json: %s\n", strerror(err));
        return err;
    }
    return make(argc, argv, "./build/fc3");
}

int sensor_calibrate(int argc, char **argv) {
    return system("cd sensor_calibrate && idf.py build");
}

int sim(int argc, char **argv) {
    return system("cd ./physics_sim/build && cmake ../ && make && ./sim");
}

int gen_compile_commands(int argc, char **argv) {
    int err;
    if (0 != (err = system("cmake -Bfc3/build fc3")))
        return err;
    /* If compile_commands.json already exists, skip soft linking it */
    if (0 == (err = access("compile_commands.json", R_OK)))
        return err;
    if (0 != (err = system("ln -s fc3/build/compile_commands.json .")))
        return err;
    return err;
}

command commands[] = {
    {"avr-gcc", avr_gcc, "Show location of avr gcc libraries"},
    {"fc1", fc1, "Compile and flash FC1"},
    {"fc3", fc3, "Compile and flash FC3"},
    {"gen-compile-commands", gen_compile_commands, "Generate compile_commands.json for LSP"},
    {"sensor-calibrate", sensor_calibrate, "Build ESP32 sensor calibration app"},
    {"serial", serial, "Start ipython repl loading in pyserial utils"},
    {"sim", sim, "Build and start physics sim"},
    {"info", help, "Display help"},
    {NULL, NULL, NULL}
};

int help(int argc, char *argv[]) {
    printf("Available commands:\n");
    for (int i = 0; commands[i].type != NULL; i++) {
        printf("  %s: %s\n", commands[i].type, commands[i].description);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int err;
    if (argc < 2)
        goto invalid_input;

    for (int i = 0; commands[i].type != NULL; i++) {
        if (strcmp(argv[1], commands[i].type) == 0) {
            if (0 == (err = commands[i].fn(argc, argv)))
                return err;
            fprintf(stderr, "ERROR: %s\n", strerror(err));
            return 1;
        }
    }
invalid_input:
    printf("Usage: %s <command>\n", argv[0]);
    help(argc, argv);
    return 1;
}
