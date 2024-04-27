#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    return system("cd fc1 && make clean && make flash");
}

int sensor_calibrate(int argc, char **argv) {
    return system("cd sensor_calibrate && idf.py build");
}

int sim(int argc, char **argv) {
    return system("cd ./physics_sim/build && cmake ../ && make && ./sim");
}

int gen_compile_commands(int argc, char **argv) {
    int ret;
    if (0 != (ret = system("cmake -Bfc3/build fc3")))
        return ret;
    /* If compile_commands.json already exists, skip soft linking it */
    if (0 == (ret = access("compile_commands.json", R_OK)))
        return ret;
    if (0 != (ret = system("ln -s fc3/build/compile_commands.json .")))
        return ret;
    return ret;
}

command commands[] = {
    {"avr-gcc", avr_gcc, "Show location of avr gcc libraries"},
    {"fc1", fc1, "Compile and flash FC1"},
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
    if (argc < 2)
        goto invalid_input;

    for (int i = 0; commands[i].type != NULL; i++) {
        if (strcmp(argv[1], commands[i].type) == 0) {
            return commands[i].fn(argc, argv);
        }
    }
invalid_input:
    printf("Usage: %s <command>\n", argv[0]);
    help(argc, argv);
    return 1;
}
