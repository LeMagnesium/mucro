/* Library ßý Mg */

/*
  Library libutil.h ; useful functions.
  void textcolor(int attr, int fgcolor, int bgcolor); => Color the text


*/

#ifdef LBU_LOADED
	/* Nothing >_> */
#else

#define LBU_LOADED 11235813
#include <string.h>
#include <stdio.h>

/* Color the text */
void textcolor(int attr, int fgcolor, int bgcolor) {
	char colcomm[13];
	sprintf(colcomm,"%c[%d;%d;%dm",0x1B, attr, fgcolor+30, bgcolor+40);
	printf("%s", colcomm);
}

#endif

