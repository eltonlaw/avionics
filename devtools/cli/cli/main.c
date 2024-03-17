#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef void (*command_fn)(int, char**);

typedef struct {
    char* type;
    command_fn fn;
} command;

void handle_serial(int argc, char **argv) {
    system("ipython -i -c \"%load devtools/serial_interactive.py\"");
}

void handle_avr_gcc(int argc, char **argv) {
    system("avr-gcc -print-search-dirs");
}

void handle_fc1(int argc, char **argv) {
    system("cd fc1 && make clean && make flash");
}

void handle_sensor_calibrate(int argc, char **argv) {
    system("cd sensor_calibrate && idf.py build");
}

void handle_physics_sim(int argc, char **argv) {
    system("cd ./physics_sim/build && cmake ../ && make && ./sim");
}

void gen_compile_commands(int argc, char **argv) {
    system("cmake -Bfc3/build fc3");
    if (access("compile_commands.json", R_OK) != 0) {
        system("ln -s fc3/build/compile_commands.json .");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    command commands[] = {
        {"avr-gcc", handle_avr_gcc},
        {"fc1", handle_fc1},
        {"gen-compile-commands", gen_compile_commands},
        {"sensor-calibrate", handle_sensor_calibrate},
        {"serial", handle_serial},
        {"sim", handle_physics_sim},
        {NULL, NULL}  // Sentinel value to mark the end of the array
    };

    for (int i = 0; commands[i].type != NULL; i++) {
        if (strcmp(argv[1], commands[i].type) == 0) {
            commands[i].fn(argc, argv);
            return 0;
        }
    }

    printf("Invalid command\n");
    return 1;
}
