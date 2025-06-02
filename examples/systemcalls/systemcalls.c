#include "systemcalls.h"
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdarg.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    int ret = system(cmd);
    if (ret == -1) {
        return false;
    } else {
        return WIFEXITED(ret) && WEXITSTATUS(ret) == 0;
    }
}

/**
 * @param count - The number of arguments passed to the function. The variables are command to execute.
 *   followed by arguments to pass to the command.
 * @param ... - A list of 1 or more arguments. The first is always the full path to the command to execute,
 *   followed by arguments.
 * @return true if the command executed successfully, false otherwise.
*/
bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    for (int i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    va_end(args);

    fflush(stdout);  // 防止 fork 之後 printf 被重複輸出

    pid_t pid = fork();
    if (pid == -1) {
        return false;
    }

    if (pid == 0) {
        // child process
        execv(command[0], command);
        // 如果 execv 成功不會回到這裡，失敗才會執行以下程式
        exit(1);
    } else {
        // parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            return false;
        }
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}

/**
 * @param outputfile - The full path to the file to write with command output.
 * All other parameters, see do_exec above.
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char *command[count + 1];
    for (int i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    va_end(args);

    fflush(stdout);

    pid_t pid = fork();
    if (pid == -1) {
        return false;
    }

    if (pid == 0) {
        // child process: 重導 stdout
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execv(command[0], command);
        exit(1);  // execv failed
    } else {
        // parent
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            return false;
        }
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}

