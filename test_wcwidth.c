#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "wcwidth.h"

static int tests_run = 0;
static int test_failures = 0;

void assertWidthIs(int expected_width, wchar_t c) {
	tests_run++;
        int actual_width = wcwidth(c);
        if (actual_width != expected_width) {
                fprintf(stderr, "ERROR: wcwidth(U+%04x) returned %d, expected %d\n", c, actual_width, expected_width);
		test_failures++;
        }
}

unsigned char randomByte() {
    // Seed the random number generator with the current time
    srand(time(NULL));
    
    // Generate a random number between 0 and 255 (inclusive)
    int randomNum = rand() % 256;

    // Convert the random number to an unsigned char
    unsigned char randomByte = (unsigned char)randomNum;

    return randomByte;
}

void runTimeTest(void){
	wchar_t testChar[1000000];
	clock_t startTime;
	clock_t stopTime;
	size_t i;
	printf("\nStart-Randomizer:%f\n", (float)clock()/CLOCKS_PER_SEC);
	for (i = 0; i < 1000000; i++){
		testChar[i] = randomByte();
		testChar[i] *= 256;
	}
	for (i = 0; i < 1000000; i++){
		testChar[i] += randomByte();
	}
	printf("Stop-Randomizer:%f\n", (float)clock()/CLOCKS_PER_SEC);

	startTime = clock();
	for (i = 0; i < 1000000; i++){
		for (size_t j = 0; j < 10; j++){
			wcwidth(testChar[i]);
		}
	}
	stopTime = clock();
	printf("\nStart wcwidth():%f\n", (float)startTime/CLOCKS_PER_SEC);
	printf("Stop wcwidth():%f\n", (float)stopTime/CLOCKS_PER_SEC);
	printf("%lu wcwidth() / Second\n", (unsigned long)(10000000/((float)(stopTime-startTime)/CLOCKS_PER_SEC)));

}

int main() {
        assertWidthIs(1, L'a');
        assertWidthIs(1, L'ö');

	// Some wide:
	assertWidthIs(2, L'Ａ');
	assertWidthIs(2, L'Ｂ');
	assertWidthIs(2, L'Ｃ');
	assertWidthIs(2, L'中');
	assertWidthIs(2, L'文');
	assertWidthIs(2, 0x679C);
	assertWidthIs(2, 0x679D);
	assertWidthIs(2, 0x2070E);
	assertWidthIs(2, 0x20731);

	assertWidthIs(1, 0x11A3);

	assertWidthIs(2, 0x1F428); // Koala emoji.
        assertWidthIs(2, 0x231a);  // Watch emoji.

	if (test_failures > 0) printf("%d tests FAILED, ", test_failures);
	printf("%d tests OK\n", tests_run);
	runTimeTest();
	return (test_failures == 0) ? 0 : 1;
}

