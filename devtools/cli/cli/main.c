#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef void (*command_fn)(int, char**);

void help(int argc, char **argv);

typedef struct {
    char *type;
    command_fn fn;
	char *description;
} command;

void serial(int argc, char **argv) {
    system("ipython -i -c \"%load devtools/serial_interactive.py\"");
}

void avr_gcc(int argc, char **argv) {
    system("avr-gcc -print-search-dirs");
}

void fc1(int argc, char **argv) {
    system("cd fc1 && make clean && make flash");
}

void sensor_calibrate(int argc, char **argv) {
    system("cd sensor_calibrate && idf.py build");
}

void sim(int argc, char **argv) {
    system("cd ./physics_sim/build && cmake ../ && make && ./sim");
}

void gen_compile_commands(int argc, char **argv) {
    system("cmake -Bfc3/build fc3");
    if (access("compile_commands.json", R_OK) != 0) {
        system("ln -s fc3/build/compile_commands.json .");
    }
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

void help(int argc, char *argv[]) {
    printf("Available commands:\n");
    for (int i = 0; commands[i].type != NULL; i++) {
        printf("  %s: %s\n", commands[i].type, commands[i].description);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2)
		goto invalid_input;

    for (int i = 0; commands[i].type != NULL; i++) {
        if (strcmp(argv[1], commands[i].type) == 0) {
            commands[i].fn(argc, argv);
            return 0;
        }
    }
invalid_input:
	printf("Usage: %s <command>\n", argv[0]);
	help(argc, argv);
	return 1;
}
