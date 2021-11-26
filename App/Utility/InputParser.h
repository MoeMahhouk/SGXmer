#ifndef TEST_APP_INPUTPARSER_H
#define TEST_APP_INPUTPARSER_H


#include <getopt.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif


void say_usage();
void say_help(void);
void parseInput(int argc, char **argv);


#if defined(__cplusplus)
}
#endif




#endif //TEST_APP_INPUTPARSER_H
