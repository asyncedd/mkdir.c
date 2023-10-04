#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct arguments {
  char **dir_names;
  int num_dirs;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = (struct arguments *)state->input;

  switch (key) {
    case ARGP_KEY_ARG:
      arguments->dir_names[arguments->num_dirs++] = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1) {
        argp_usage(state);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = {0, parse_opt, "DIRECTORY_NAME...", 0};

int main(int argc, char *argv[]) {
  struct arguments arguments;

  arguments.num_dirs = 0;
  arguments.dir_names = (char **)malloc(argc * sizeof(char *));
  if (arguments.dir_names == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return 1;
  }

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  for (int i = 0; i < arguments.num_dirs; i++) {
    if (mkdir(arguments.dir_names[i], 0777) == 0) {
      printf("Directory '%s' created successfully.\n", arguments.dir_names[i]);
    } else {
      perror("Error creating directory");
    }
  }

  free(arguments.dir_names);

  return 0;
}
