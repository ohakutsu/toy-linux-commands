#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 2048

struct cmd {
  char **argv;
  int argc;
};

struct cmd *parse_cmd(char *line, int len);
static void init_cmd(struct cmd *cmd);

int main() {
  while (1) {
    char line[BUFFER_SIZE];
    struct cmd *command;
    char *result;
    pid_t pid;

    printf("$ ");
    result = fgets(line, BUFFER_SIZE, stdin);
    if (result == NULL) {
      fprintf(stderr, "fgets error");
    }
    command = parse_cmd(line, strlen(line));

    pid = fork();
    if (pid < 0) {
      fprintf(stderr, "failed fork");
    } else if (pid == 0) {
      int result;
      result = execvp(command->argv[0], command->argv);
      if (result == -1) {
        perror(command->argv[0]);
        exit(99);
      }
    } else {
      int status;
      waitpid(pid, &status, 0);
    }
  }

  exit(0);
}

struct cmd *parse_cmd(char *line, int len) {
  struct cmd *command = calloc(1, sizeof(struct cmd));
  char *p = line;

  init_cmd(command);

  while (p < line + len) {
    if (*p == ' ' || *p == '\n') {
      *p = '\0';
    } else {
      char *next = p + 1;

      command->argv[command->argc] = p;
      command->argc++;

      while (*next != ' ' && *next != '\n') {
        ++p;
        next = p + 1;
      }
    }

    ++p;
  }

  return command;
}

static void init_cmd(struct cmd *cmd) {
  cmd->argv = calloc(10, sizeof(char *));
  cmd->argc = 0;

  return;
}
