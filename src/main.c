/*
Author: Ryan Moore
Date: March 12, 2019
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "web_minimizer.h"

static const struct {
  const char* ending;
  const char* new_ending;
} exts[] = {
  {".html", ".min.html"},
  {".js", ".min.js"},
  {".css", ".min.css"},
  {0, 0}
};

int main(int argc, char** argv)
{
  int i;
  int in_length;
  int ext_length;
  static char new_name[256];
  static char* buffer;
  long file_length;
  long past_file_length;
  FILE* input_fd;
  FILE* output_fd;

  if (argc != 2) {
    printf("Web Minimizer is a useful tool for making compact HTML, CSS, and JavaScript."\
           "This tool can be used as a standalone program or built into your project.\n"\
           "Usage:\n  webmin <input>\n\n");
    return -1;
  }

  in_length = strlen(argv[1]);
  for (i = 0; exts[i].ending != 0; i++) {
    ext_length = strlen(exts[i].ending);
    if (ext_length < in_length) {
      if (strncmp(&argv[1][in_length - ext_length], exts[i].ending, ext_length) == 0) {
        (void)sprintf(&new_name[0], "%s", argv[1]);
        for (;argv[1][in_length] != '.'; in_length--){}
        (void)sprintf(&new_name[in_length], "%s", exts[i].new_ending);
        break;
      }
    }
  }

  if (new_name[0] == 0) {
    printf("Unrecognized filetype on file: %s\n", argv[1]);
    return -1;
  }

  input_fd = fopen(argv[1], "r");

  if (input_fd) {
    fseek(input_fd, 0, SEEK_END);
    file_length = ftell(input_fd);
    fseek(input_fd, 0, SEEK_SET);
    buffer = malloc(file_length);
    if (buffer) {
      fread(buffer, 1, file_length, input_fd);
    }
    fclose(input_fd);
  }

  in_length = strlen(new_name);
  if (strcmp(&new_name[in_length - ext_length], ".html") == 0) {
    minimize_html(buffer);
  }

  past_file_length = file_length;
  output_fd = fopen(new_name, "w+");
  file_length = strlen(buffer);
  fwrite(buffer, 1, file_length, output_fd);

  printf("Inital Size: %d Bytes, Minimized Size: %d Bytes, Reduction of %.1f\%\n",
          past_file_length, file_length, 100.0 - ((double)file_length / (double)past_file_length) * 100.0);

  return 0;
}
