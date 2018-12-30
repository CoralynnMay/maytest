#include "maytest.h"

void local_main();

MT_MAIN(local_main);

void test_pass() {
	DEBUG("%s", "Test pass");
	ASSERT(1 == 1);
}

void test_fail() {
	DEBUG("%s", "Test fail");
	ASSERT(1==0);
}

void test_suite() {
	RUN(test_pass);
	RUN(test_fail);
}

void local_main() {
	SUITE(test_suite);
}