#include "systemcalls.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
 */
bool do_system(const char *cmd)
{
    if (cmd == NULL) return false;

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    if (cmd == NULL) {
        return false;
    }

    int ret = system(cmd);
    return (ret == 0);

    if (ret == -1) {
        perror("system call failed");
        return false;
    }

    return WIFEXITED(ret) && WEXITSTATUS(ret) == 0;
}

/**
 * @param count - The number of arguments passed to the function. The variables are command to execute,
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
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        va_end(args);
        return false;
    }

    if (pid == 0) {
        // Child
        execv(command[0], command);
        perror("execv failed"); // If execv returns, it failed
        exit(EXIT_FAILURE);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        va_end(args);
        return false;
    }

    va_end(args);
    return WIFEXITED(status) && WEXITSTATUS(status) == 0;

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/

}

/**
 * @param outputfile - The full path to the file to write with command output.
 * All other parameters, see do_exec above.
 */
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    if (outputfile == NULL) return false;

    va_list args;
    va_start(args, count);

    char *command[count + 1];
    for (int i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        va_end(args);
        return false;
    }

    if (pid == 0) {
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open failed");
            exit(EXIT_FAILURE);
        }

        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2 failed");
            close(fd);
            exit(EXIT_FAILURE);
        }

        close(fd);
        execv(command[0], command);
        perror("execv failed");
        exit(EXIT_FAILURE);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        va_end(args);
        return false;
    }

    va_end(args);
    return WIFEXITED(status) && WEXITSTATUS(status) == 0;


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

}

