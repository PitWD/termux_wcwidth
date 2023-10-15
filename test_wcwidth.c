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
    srand(clock());
    
    // Generate a random number between 0 and 255 (inclusive)
    int randomNum = rand() % 256;

    // Convert the random number to an unsigned char
    unsigned char randomByte = (unsigned char)randomNum;

    return randomByte;
}

void runTimeTest(void){
	wchar_t testChar[1000000];
	
	size_t i;
	clock_t startTimeRandom = clock();
	for (i = 0; i < 1000000; i++){
		testChar[i] = randomByte();
	}
	for (i = 0; i < 1000000; i++){
		testChar[i] *= 256;
		testChar[i] += randomByte();
		i++;
	}
	clock_t stopTimeRandom = clock();
	printf("\nStart Randomizer:%f\n", (float)startTimeRandom/CLOCKS_PER_SEC);
	printf("Stop Randomizer:%f\n", (float)stopTimeRandom/CLOCKS_PER_SEC);
	printf("%lu Randoms / Second\n", (unsigned long)(1500000/((float)(stopTimeRandom-startTimeRandom)/CLOCKS_PER_SEC)));

	clock_t startTimeWC = clock();
	for (i = 0; i < 1000000; i++){
		for (size_t j = 0; j < 10; j++){
			wcwidth(testChar[i]);
		}
	}
	clock_t stopTimeWC = clock();
	printf("\nStart wcwidth():%f\n", (float)startTimeWC/CLOCKS_PER_SEC);
	printf("Stop wcwidth():%f\n", (float)stopTimeWC/CLOCKS_PER_SEC);
	printf("%lu wcwidth() / Second\n\n", (unsigned long)(10000000/((float)(stopTimeWC-startTimeWC)/CLOCKS_PER_SEC)));

}

int main() {
	printf("\n");
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

	if (test_failures > 0) printf("%d (original) tests FAILED, ", test_failures);
	printf("%d (original) tests OK\n", tests_run - test_failures);
	
	// Pit's Tests...
	runTimeTest();

	return (test_failures == 0) ? 0 : 1;
}

