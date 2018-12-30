#ifndef MAYTEST_H
#define MAYTEST_H 1

#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

extern int MT_colour_flag;
extern int MT_verbose;
extern int MT_fail;
extern int MT_force_flag;
extern int MT_total;
extern int MT_testc;
extern char** MT_testv;
extern int MT_suitec;
extern char** MT_suitev;

#define MAX_BUFFER 4096

//Colour codes
#define MTC_RESET "\e[0m"
#define MTC_FAIL "\e[101m\e[97m\e[1m"
#define MTC_PASS "\e[92m\e[1m"
#define MTC_DEBUGPRINT "\e[1m"

#define MT_HELP() printf("Usage: %s [options]\n\n\
Run tests for given program or library\n\n\
Options:\n\
  -h, --help\t\tshow this help message and exit\n\
  -v, --verbose\t\tmake output more verbose\n\
  --no-colour\t\tswitch output to colourless\n\
  -f, --force\t\tforce all tests to run regardless of failures\n\
  -t, --test\t\trun specified test, use multiple times for multiple tests\n\
  -s, --suite\t\trun specified suite, use multiple times for multiple suites\n", argv[0])

#define COLOUR(c) (MT_colour_flag ? c : "")
#define VERB0(m) printf("%s\n", m)
#define VERB1(m) if (MT_verbose > 0) printf("%s\n", m)
#define VERB2(m) if (MT_verbose > 1) printf("%s\n", m)
#define XSTR(s) #s
#define STR(s) XSTR(s)

#define DEBUG(m, ...) {\
	char buffer[MAX_BUFFER];\
	snprintf(buffer, MAX_BUFFER, "[%sDEBUG%s] " #m " %s:%s", COLOUR(MTC_DEBUGPRINT), COLOUR(MTC_RESET), __VA_ARGS__, __FILE__, STR(__LINE__));\
	VERB2(buffer);}

#define ASSERT(x) if (!(x)) {\
	char buffer[MAX_BUFFER];\
	snprintf(buffer, MAX_BUFFER, "[%sFAIL%s] Assertion: %s %s:%s", COLOUR(MTC_FAIL), COLOUR(MTC_RESET), #x, __FILE__, STR(__LINE__));\
	VERB1(buffer);\
	MT_fail++; if (MT_force_flag == 0) return;} else {\
	char buffer[MAX_BUFFER];\
	snprintf(buffer, MAX_BUFFER, "[%sPASS%s] Assertion: %s %s:%s", COLOUR(MTC_PASS), COLOUR(MTC_RESET), #x, __FILE__, STR(__LINE__));\
	VERB2(buffer);}

#define RUN(x) do { \
	int run = 0;\
	if (MT_testc > 0) {\
		for (int i = 0; i < MT_testc; i++) {\
			if (strcmp(MT_testv[i], #x) == 0) run++;\
		}\
	} else {\
		run++;\
	}\
	if (run > 0) {\
	int cur_MT_fail = MT_fail;\
	MT_total++;\
	x();\
	if (cur_MT_fail < MT_fail) {\
		char buffer[MAX_BUFFER];\
		snprintf(buffer, MAX_BUFFER, "[%sFAIL%s] Test: %s %s:%s", COLOUR(MTC_FAIL), COLOUR(MTC_RESET), #x, __FILE__, STR(__LINE__));\
		VERB0(buffer);\
	} else {\
		char buffer[MAX_BUFFER];\
		snprintf(buffer, MAX_BUFFER, "[%sPASS%s] Test: %s %s:%s", COLOUR(MTC_PASS), COLOUR(MTC_RESET), #x, __FILE__, STR(__LINE__));\
		VERB1(buffer);\
	}}} while (0);

#define SUITE(x) do { \
	int run = 0;\
	if (MT_suitec > 0) {\
		for (int i = 0; i < MT_suitec; i++) {\
			if (strcmp(MT_suitev[i], #x) == 0) run++;\
		}\
	} else {\
		run++;\
	}\
	if (run > 0) {\
	int cur_MT_fail = MT_fail;\
	x();\
	if (cur_MT_fail < MT_fail) {\
		char buffer[MAX_BUFFER];\
		snprintf(buffer, MAX_BUFFER, "[%sFAIL%s] Suite: %s %s:%s", COLOUR(MTC_FAIL), COLOUR(MTC_RESET), #x, __FILE__, STR(__LINE__));\
		VERB1(buffer);\
	} else {\
		char buffer[MAX_BUFFER];\
		snprintf(buffer, MAX_BUFFER, "[%sPASS%s] Suite: %s %s:%s", COLOUR(MTC_PASS), COLOUR(MTC_RESET), #x, __FILE__, STR(__LINE__));\
		VERB2(buffer);\
	}}} while (0);

#define MT_MAIN(x) int MT_colour_flag;\
int MT_verbose;\
int MT_fail;\
int MT_force_flag;\
int MT_total;\
int MT_testc;\
char** MT_testv;\
int MT_suitec;\
char** MT_suitev;\
int main(int argc, char** argv) { \
	int c;\
	int digit_optind = 0;\
	MT_verbose = 0;\
	MT_colour_flag = 1;\
	MT_fail = 0;\
	MT_force_flag = 0;\
	MT_total = 0;\
	MT_testc = 0;\
	MT_testv = calloc(sizeof(char*), argc);\
	MT_suitec = 0;\
	MT_suitev = calloc(sizeof(char*), argc);\
	while(1) {\
		int this_option_optind = optind ? optind : 1;\
		int option_index = 0;\
		static struct option long_options[] = {\
			{"help", no_argument, 0, 'h'},\
			{"no-colour", no_argument, 0, 0},\
			{"verbose", no_argument, 0, 'v'},\
			{"force", no_argument, 0, 'f'},\
			{"test", required_argument, 0, 't'},\
			{"suite", required_argument, 0, 's'},\
		};\
		c = getopt_long(argc, argv, "hvft:s:", long_options, &option_index);\
		if (c == -1) break;\
		switch (c) {\
			case 0:\
				if(strcmp(long_options[option_index].name, "no-colour") == 0) {\
					MT_colour_flag = 0;\
				}\
				break;\
			case 'h':\
				MT_HELP();\
				return 0;\
			case 'v':\
				MT_verbose++;\
				break;\
			case 'f':\
				MT_force_flag = 1;\
				break;\
			case 't':\
				MT_testv[MT_testc] = calloc(sizeof(char), MAX_BUFFER);\
				strcpy(MT_testv[MT_testc], optarg);\
				MT_testc++;\
				break;\
			case 's':\
				MT_suitev[MT_suitec] = calloc(sizeof(char), MAX_BUFFER);\
				strcpy(MT_suitev[MT_suitec], optarg);\
				MT_suitec++;\
				break;\
			default:\
				break;\
		}\
	}\
	x();\
	if (MT_fail == 0) {\
		char buffer[MAX_BUFFER];\
		snprintf(buffer, MAX_BUFFER, "[%sPASS%s] All %d test(s) pass", COLOUR(MTC_PASS), COLOUR(MTC_RESET), MT_total);\
		VERB0(buffer);\
	} else {\
		char buffer[MAX_BUFFER];\
		snprintf(buffer, MAX_BUFFER, "[%sFAIL%s] %d/%d test(s) pass", COLOUR(MTC_PASS), COLOUR(MTC_RESET), (MT_total-MT_fail), MT_total);\
		VERB0(buffer);\
	}\
	free(MT_testv);\
	free(MT_suitev);\
	return 0;\
}

#endif