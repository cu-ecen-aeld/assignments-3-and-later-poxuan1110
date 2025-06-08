#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char *argv[]) {

    openlog("writer", LOG_PID, LOG_USER);

    if (argc != 3) {
        syslog(LOG_ERR, "Usage: %s <file_path> <text_to_write>", argv[0]);
        fprintf(stderr, "Usage: %s <file_path> <text_to_write>\n", argv[0]);
        closelog();
        return 1;
    }

    const char *file_path = argv[1];
    const char *text = argv[2];


    FILE *file = fopen(file_path, "w");
    if (!file) {
        syslog(LOG_ERR, "Error opening file: %s", file_path);
        perror("Error opening file");
        closelog();
        return 1;
    }

    if (fprintf(file, "%s", text) < 0) {
        syslog(LOG_ERR, "Error writing to file: %s", file_path);
        perror("Error writing to file");
        fclose(file);
        closelog();
        return 1;
    }

    syslog(LOG_DEBUG, "Writing '%s' to '%s'", text, file_path);


    fclose(file);
    closelog();

    return 0;
}
