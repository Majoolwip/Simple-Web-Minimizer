/*
Author: Ryan Moore
Date: March 12, 2019
Description: Contains operations for creating minimized forms of HTML, CSS, and
Javascript text.
*/
#include "stdio.h"
#include "web_minimizer.h"
#include "string.h"

#define START_OF_CHARS 33

/**
  Removes excess hidden characters but leaves one space between words in the
  input button

  @param char* Buffer to be edited.
  @return int Length of the text after editing
  */
int remove_excess_spaces(char* input)
{
  int reading = 0;
  int writing = 0;
  int flag = 0;

  while (1) {
    if (input[reading] >= START_OF_CHARS) {
      flag = 1;
    }

    if (flag) {
      input[writing] = input[reading];
      writing++;
    }

    if (input[reading] < START_OF_CHARS && flag == 1) {
      if (input[reading] != 0) {
        input[writing - 1] = ' ';
      }
      flag = 0;
    }

    if (input[reading] == 0) {
      input[writing] = 0;
      break;
    }
    reading++;
  }
  return writing;
}

/**
  Removes all characters inbetween the two const char sequences.

  @param char* Buffer to be edited.
  @param const char* Beginning char sequence
  @param const char* Ending char sequence
  @return int Length of the text after editing
  */
int remove_between(char* input, const char* begin, const char* end)
{
  int writing = 0;
  int reading = 0;
  char quote = 0;
  int tag = 0;
  const int begin_length = strlen(begin);
  const int end_length = strlen(end);
  while (1) {
    if (strncmp(&input[reading], begin, begin_length) == 0) {
      while (strncmp(&input[reading], end, end_length) != 0) {
        reading++;
        if (input[reading + end_length] == 0) {
          break;
        }
      }
      reading += end_length;
      continue;
    }
    input[writing] = input[reading];
    writing++;
    if (input[reading] == 0) {
      break;
    }
    reading++;
  }
  return writing;
}


/**
  Minimizes a given input of HTML, assumes proper syntax is used.

  @param char* Buffer to be edited.
  @return int Length of the text after editing
  */
int minimize_html(char* input)
{
  remove_between(input, "<!--", "-->");
  return remove_excess_spaces(input);
}

/**
  Minimizes a given input of Javascript, assumes proper syntax is used.

  @param char* Buffer to be edited.
  @return int Length of the text after editing
  */
int minimize_js(char* input)
{
  remove_between(input, "//", "\n");
  remove_between(input, "/*", "*/");
  return remove_excess_spaces(input);
}

/**
  Minimizes a given input of CSS, assumes proper syntax is used.

  @param char* Buffer to be edited.
  @return int Length of the text after editing
  */
int minimize_css(char* input)
{
  remove_between(input, "/*", "*/");
  return remove_excess_spaces(input);
}
