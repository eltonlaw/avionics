#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "buildtools.h"

/* Run cmake setup in directory, output dir is in root build dir  */
int cmake(const char *project_path) {
    char cmd[256];

    snprintf(cmd, sizeof(cmd), "cmake -Bbuild/%s %s", project_path, project_path);

    int err = WEXITSTATUS(system(cmd));
    if (err != 0) {
        fprintf(stderr, "Error running cmake: %s\n", strerror(err));
        return err;
    }

    return 0;
}

/* Run make passing in subcommands */
int make(int argc, char **argv, char *path) {
    int err, i;
    char cmd[256] = "make";

    if (0 != (err = chdir(path))) {
        fprintf(stderr, "Error changing directory: %s\n", strerror(err));
        return err;
    }
    if (argc > 2) {
        for (i = 2; i < argc; i++) {
            strcat(cmd, " ");
            strcat(cmd, argv[i]);
        }
    }
    printf("%s\n", cmd);
    if (0 != (err = WEXITSTATUS(system(cmd))))
        fprintf(stderr, "Error calling system: %s\n", strerror(err));
    return err;
}
