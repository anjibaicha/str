#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fatal_error.h"
#include "str.h"

#define streql(x, y) strcmp(x, y) == 0

void test_str();
void test_str_new();
void test_str_get();
void test_str_append();
void test_str_set_chunk_size();
void test_str_size();
void test_str_length();
void test_file_reading();

int
main()
{
  test_str();

  // create a Str
  Str *greeting = str_new("Hello");

  // use as little memory as possible
  str_set_chunk_size(greeting, 1);

  // append a c-string, an integer, and a character
  str_append(greeting, ", world! The year is ");
  str_append_int(greeting, 2017);
  str_append_char(greeting, '.');

  // display contents and metadata
  printf("\n%s\n", str_get(greeting));
  printf("length: %lu characters\n", str_length(greeting));
  printf("memory used: %lu bytes\n", str_size(greeting));

  // destroy it
  str_free(greeting);
}

void
test_str()
{
  test_str_new();
  puts("test_str_new() passed");

  test_str_get();
  puts("test_str_get() passed");

  test_str_append();
  puts("test_str_append() passed");

  test_str_set_chunk_size();
  puts("test_str_set_chunk_size() passed");

  test_str_size();
  puts("test_str_size() passed");

  test_str_length();
  puts("test_str_length() passed");

  test_file_reading();
  puts("test_file_reading() passed");

  puts("all tests passed");
}

void
test_str_new()
{
  Str *s1 = NULL;
  s1 = str_new("Hello, world!");
  assert(s1 != NULL);
  str_free(s1);

  Str *s2 = NULL;
  s2 = str_new("");
  assert(s2 != NULL);
  str_free(s2);
}

void
test_str_get()
{
  Str *s1 = str_new("Hello, world!");
  assert(streql(str_get(s1), "Hello, world!"));
  str_free(s1);

  Str *s2 = str_new("");
  assert(streql(str_get(s2), ""));
  str_free(s2);
}

void
test_str_append()
{
  Str *s1 = str_new("Hello, world!");
  str_append(s1, " How are you?");
  assert(streql(str_get(s1), "Hello, world! How are you?"));
  str_free(s1); 

  Str *s2 = str_new("Hello, world!");
  str_append(s2, "");
  assert(streql(str_get(s2), "Hello, world!"));
  str_free(s2);

  Str *s3 = str_new("");
  str_append(s3, "Hello, world!");
  assert(streql(str_get(s3), "Hello, world!"));
  str_free(s3);

  Str *s4 = str_new("");
  for (char letter = 'a'; letter <= 'z'; letter += 1) {
    str_append_char(s4, letter);  
  }
  assert(streql(str_get(s4), "abcdefghijklmnopqrstuvwxyz"));
  str_free(s4);

  Str *s5 = str_new("The year is currently: ");
  str_append_int(s5, 2017);
  assert(streql(str_get(s5), "The year is currently: 2017"));
  str_free(s5);
}

void
test_str_set_chunk_size()
{
  Str *s1 = str_new("");
  str_set_chunk_size(s1, 1);
  str_append(s1, "Hello, world!");
  assert(str_size(s1) == 14);
  str_free(s1);

  Str *s2 = str_new("Hello, world!");
  str_set_chunk_size(s2, 25);
  str_append(s2, "How are you doing today?");
  assert(str_size(s2) == 39);
  str_free(s2);
}

void
test_str_size()
{
  Str *s1 = str_new("");
  assert(str_size(s1) == 1);
  str_free(s1);
}

void
test_str_length()
{
  Str *s1 = str_new("");
  assert(str_length(s1) == 0);
  str_free(s1);

  Str *s2 = str_new("Hello, world!");
  assert(str_length(s2) == 13);
  str_free(s2);
}

void
test_file_reading()
{
  FILE *f_stream = fopen("test.txt", "r");   
  if (f_stream == NULL) fatal_error("test_file_reading(): error opening test.txt");

  // read file in the regular way
  if (fseek(f_stream, 0, SEEK_END) != 0) fatal_error("test_file_reading(): fseek error");
  const long f_length = ftell(f_stream);
  if (f_length == EOF) fatal_error("test_file_reading(): ftell error");
  rewind(f_stream);
  char *buffer = malloc(f_length + 1);
  if (buffer == NULL) fatal_error("test_file_reading(): malloc error");
  if (fread(buffer, f_length, 1, f_stream) < 1) fatal_error("test_file_reading(): fread error");
  rewind(f_stream);

  // read file in the str way
  Str *str_buffer = str_new("");
  int c = '\0';
  while ((c = fgetc(f_stream)) != EOF) {
    str_append_char(str_buffer, c);
  }

  assert(streql(buffer, str_get(str_buffer)));

  free(buffer);
  str_free(str_buffer);
}

