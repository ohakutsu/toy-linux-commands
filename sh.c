#include <ctype.h>
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

static void cmd_free(struct cmd *cmd);
static struct cmd *cmd_new(void);
static struct cmd *parse_cmd(char *line, int len);

int main() {
  while (1) {
    char line[BUFFER_SIZE];
    struct cmd *cmd;
    char *result;
    pid_t pid;

    printf("$ ");
    result = fgets(line, BUFFER_SIZE, stdin);
    if (result == NULL) {
      fprintf(stderr, "fgets error");
    }
    cmd = parse_cmd(line, strlen(line));

    pid = fork();
    if (pid < 0) {
      fprintf(stderr, "failed fork");
    } else if (pid == 0) {
      int result;
      result = execvp(cmd->argv[0], cmd->argv);
      if (result == -1) {
        /*perror(cmd->argv[0]);*/
        exit(99);
      }
    } else {
      int status;
      waitpid(pid, &status, 0);
    }

    cmd_free(cmd);
  }

  exit(0);
}

static struct cmd *parse_cmd(char *line, int len) {
  struct cmd *cmd = cmd_new();
  char *p = line;

  while (p < line + len) {
    while (p < line + len && isspace(*p)) {
      *p = '\0';
      ++p;
    }

    if (p < line + len) {
      cmd->argv[cmd->argc] = p;
      cmd->argc++;
    }

    while (p < line + len && !isspace(*p)) {
      ++p;
    }
  }

  return cmd;
}

static struct cmd *cmd_new() {
  struct cmd *cmd = calloc(1, sizeof(struct cmd));
  cmd->argv = calloc(10, sizeof(char *));
  cmd->argc = 0;

  return cmd;
}

static void cmd_free(struct cmd *cmd) {
  free(cmd->argv);
  free(cmd);

  return;
}
