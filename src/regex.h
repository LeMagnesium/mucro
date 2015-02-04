/* Library ßý Mg */

/*
  Library regex.h ; regex comparating/analzing function.
  
  Functions:
  * int regex_cmp (char * regex, char * string);
  * int max (int a, int b);

*/

#ifdef REGEX_LIB
	/* Nothing happens */
#else

#define REGEX_LIB 112358132134
#include <string.h>
#include <stdio.h>

/* Regex Strict Comparaison */
int regex_cmp(char * regex, char * string) {
	if(strcmp(regex,string) == 0) {return 1;}
	int i = 0;
	int idem = 1;
	int seek_mode = 0;
	char seek_caracter = '\0';
	int anti_parser_position = 0;
	
	
	//printf("%d\n",max(strlen(regex),strlen(string)));
	while(i < max(strlen(regex),strlen(string))) {
		printf("%c",string[i]);
		
		//printf("Comparaison between %c and %c...\n",regex[i],string[i]);
		if(regex[i] == string[i]) {i++;continue;}
		else if(regex[i] == '?') {i++;continue;}
		else if(regex[i] == '*') {
			if(regex[i+1] == '\0') {
				// End of regex, end of string can be anything
				break;
			}
			else {
				seek_caracter = regex[i+1];
				//printf("Seeking %c,\n",seek_caracter);
				while (string[i] != seek_caracter) {
					if(i == max(strlen(regex),strlen(string))) {
						printf("%c\n",regex[i+1]);return 0;
					}
					i++;
					printf("%c",string[i]);
				}
			}
			i++;continue;
		}
		else {idem = 0;break;}
		i++;
	}
	printf("\n");
	return idem;
}

/* Regex's find 
int regex_find(char * regex, char * string) {
	if(strcmp(regex,string) == 0) {return 1;}
	int i = 0;
	int idem = 1;
	int difference = 0;
	int seek_mode = 0;
	int anti_parser_position = 0;

	if(!(regex[0] == '?' && regex[1] == string[1])) {
		while(regex[difference] != string[0]) {
			difference++;
		}
	}

	
	while(i < max(strlen(regex),strlen(string))) {
		if(regex[i] == string[i]) {i++;continue;}
		else if(regex[i] == '?') {i++;continue;}
		else {idem = 0;break;}
		i++;
	}
	
	return idem;
} */

/* Useful functions */
int max(int a, int b) {
	return (a >= b?a:b);
}

#endif
