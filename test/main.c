#include <stdio.h>
#include <string.h>
#include "web_minimizer.h"

#define BUFFER_SIZE 4096

void test(const char* name, char* buffer, const char* test)
{
  printf(name);
  if (strcmp(buffer, test) == 0) {
    printf("PASSED\n");
  } else {
    printf("FAILED, Got: %s, Expected: %s\n",buffer, test);
  }
}

int main()
{
  int start = 0;
  static char buffer[BUFFER_SIZE];

  // Test 0: Check no modification when not needed
  (void)sprintf(buffer, "<test>");
  minimize_html(buffer);
  test("Test 0: ", buffer, "<test>");

  // Test 1: Check for front removal
  (void)sprintf(buffer, "  <test>");
  minimize_html(buffer);
  test("Test 1: ", buffer, "<test>");

  // Test 2: Remove end spaces
  (void)sprintf(buffer, "  <test>  ");
  minimize_html(buffer);
  test("Test 2: ", buffer, "<test>");

  // Test 3: Remove end hidden characters
  (void)sprintf(buffer, "  <test>\n\r\t");
  minimize_html(buffer);
  test("Test 3: ", buffer, "<test>");

  // Test 4: Remove space between ><
  (void)sprintf(buffer, "<head> <meta charset=\"utf-8\">\n</head>     <body>");
  minimize_html(buffer);
  test("Test 4: ", buffer, "<head><meta charset=\"utf-8\"></head><body>");

  (void)sprintf(buffer, "<DOCTYPE html>\n"\
                        "<html>\n"\
                        "  <head>\n"\
                        "    <meta charset=\"utf-8\">\n"\
                        "  </head>\n"\
                        "  <body>\n"\
                        "    <h1 style  =  \"float:left\">   Hey there    </h1>\n"\
                        "  </body>\n"\
                        "</html>\n");
  minimize_html(buffer);
  test("Test 5: ", buffer, "<DOCTYPE html><html><head><meta charset=\"utf-8\"></head><body><h1 style=\"float:left\">Hey there</h1></body></html>");

  // Test 6: Remove comments from start
  (void)sprintf(buffer, "<!-- This is a test --><DOCTYPE html>");
  minimize_html(buffer);
  test("Test 6: ", buffer, "<DOCTYPE html>");

  // Test 7: Remove comments from end
  (void)sprintf(buffer, "<head><!-- This is a test -->");
  minimize_html(buffer);
  test("Test 7: ", buffer, "<head>");

  // Test 8: Remove comments from middle
  (void)sprintf(buffer, "<head><!-- This is a test --><body>");
  minimize_html(buffer);
  test("Test 8: ", buffer, "<head><body>");

  // Test 9: Comment then newline
  (void)sprintf(buffer, "<!-- Comment -->\n<head>");
  minimize_html(buffer);
  test("Test 9: ", buffer, "<head>");

  // Test 10: Trailing comment
  (void)sprintf(buffer, "<head><!-- Oh no");
  minimize_html(buffer);
  test("Test 10: ", buffer, "<head>");

  // Test 11: Differing Spacing
  (void)sprintf(buffer, "<head> <head>  <head> ");
  minimize_html(buffer);
  test("Test 11: ", buffer, "<head><head><head>");

  // Test 12: Quoated Strings
  (void)sprintf(buffer, "<head id=\"<!-- Im a comment -->\">");
  minimize_html(buffer);
  test("Test 12: ", buffer, "<head id=\"<!-- Im a comment -->\">");

  // Test 13: Nested Quotes
  (void)sprintf(buffer, "<head id=\"\'test\'\">");
  minimize_html(buffer);
  test("Test 13: ", buffer, "<head id=\"\'test\'\">");

  printf("Testing CSS\n");

  // Test 1: Basic input
  (void)sprintf(buffer, "p {\n  background-color: #FFFFFF;\n}");
  minimize_css(buffer);
  test("Test 1: ", buffer, "p{background-color:#FFFFFF;}");

  // Test 2: Basic Input with comments
  (void)sprintf(buffer, "/* This is a comment */\np {\nbackground-color: #FFFFFF;\n}/*** Yo ***//**/");
  minimize_css(buffer);
  test("Test 2: ", buffer, "p{background-color:#FFFFFF;}");

  // Test 3: Nested Tags
  (void)sprintf(buffer, "div p {\n    background-color: #FFFFFF;\n}");
  minimize_css(buffer);
  test("Test 3: ", buffer, "div p{background-color:#FFFFFF;}");

  // Test 4: More nesting
  (void)sprintf(buffer, "h1 {\n color: red;\n }\ndiv ul{\ncolor:blue;\n}\n");
  minimize_css(buffer);
  test("Test 4: ", buffer, "h1{color:red;}div ul{color:blue;}");
  return 0;
}
