#pragma once
#include "Errors.h"

#define VE_DEBUG

#define NORMAL_INFO(fmt, ...) printf("%s: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)

#ifdef VE_DEBUG

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

/* Function used for assertions handling */
static bool assertFun(bool condition, const char* message, int line, const char* function, const char* file, bool *ignore) 
{
	if (!condition) {
		printf("Assert failed!\n"
			"\t%s\n"
			"\tFunction: %s, line: %d, file: %s\n", message, function, line, file);
		while (1) {
			printf("Continue, stop, exit or ignore? [c/s/e/i]");
			char c = _getch();
			printf("\n");
			switch (c) {
				case 'e':
				case 'E':
					exit(1);
				case 'i':
				case 'I':
					*ignore = true;
				case 'c':
				case 'C':
					return false;
				case 's':
				case 'S':
					return true;
				default:
					break;
			}
		}
	}
	return false;
}

#define DEBUG_INFO(fmt, ...) printf("%s\n" fmt"\n", __FUNCTION__, ##__VA_ARGS__)

/* It will only work if it was compiled in debug mode */
#ifdef _DEBUG
#define DEBUG_LINE __asm {int 3}
#else /* _DEBUG */

#define DEBUG_LINE													\
	do {															\
		printf("Not compiled with debugger support, exiting...");	\
		exit(VE_EMODE);												\
	} while (0)
#endif /* _DEBUG */

#define assert(exp, desc, ...)										\
	do {															\
		if(!(exp)) {												\
			static bool ignore = false;								\
			if (!ignore) {											\
				char buf[100];										\
				sprintf_s(buf, 100, desc, ##__VA_ARGS__);			\
				if (assertFun((exp), (buf), __LINE__,				\
					__FUNCTION__, __FILE__, &ignore)) {				\
					DEBUG_LINE;										\
				}													\
			}														\
		}															\
	} while (0)


#define warnOn(exp, fmt, ...)										\
	do {															\
		if(exp)														\
			printf("WARNING: " fmt, ##__VA_ARGS__)					\
	} while(0)

#else // VE_DEBUG

#define assert(exp, desc, ...)		
#define DEBUG_INFO(fmt, ...)

#endif // VE_DEBUG
