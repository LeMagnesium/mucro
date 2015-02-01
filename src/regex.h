/* Library ßý Mg */

/*
  Library regex.h ; regex comparating/analzing function.
  
  Functions:
  * -- -- ();

*/

#ifdef REGEX_LIB
	/* Nothing happens */
#else

#define REGEX_LIB 112358132134
#include <string.h>
#include <stdio.h>

int regex_cmp(char * regex, char * string) {
	if(strcmp(regex,string) == 0) {return 1;}
	int i = 0;
	int idem = 1;
	int seek_mode = 0;
	int anti_parser_position = 0;
	
	while(i < max(strlen(regex),strlen(string))) {
		if(regex[i] == string[i]) {i++;continue;}
		else if(regex[i] == '?') {i++;continue;}
		else {idem = 0;break;}
		i++;
	}
	
	return idem;
}

int max(int a, int b) {
	return (a >= b?a:b);
}

#endif
