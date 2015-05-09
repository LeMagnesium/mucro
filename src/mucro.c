/* Program ßý Mg. */

/*
	Mµcro, v0.1.1 ßý Mg, CopyRight 2015.
	License : GPLv3 and further
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

#ifndef WIN32
#include <regex.h>
#endif


/* µ_OPTIONS STRUCT */
struct u_option {
	int recursive;
	int strict;
	int quiet;
	int bare;
	int color;
	int hidden;
	int errverb;
	char * ofname;
	char * filename;
	char * rootpath;
	FILE * outfile;
#ifndef WIN32
	regex_t regex;
#endif

};


/* Prototypes */
int ls(char rep[], struct u_option camembert);						/* LS recursive					*/
void help();														/* Help, I need somebody. Help 	*/
struct u_option u_parse_opt(int argc, char *argv[]);				/* Options parser				*/
void textcolor(int attr, int fgcolor, int bgcolor, int disabled);	/* Color the text				*/
void easter();														/* ? */

/* Main function */
int main(int argc, char *argv[]) {

	if(argc == 1) {printf("No filename given... Type mucro -h for help.\n");exit(0);}

	/* Option parse */
	struct u_option camembert;
	camembert = u_parse_opt(argc, argv);
	if (strcmp(camembert.filename,"nyan") == 0) {
		printf("No filename entered, using the default one instead, hehe!\n");
#ifndef WIN32
		if (camembert.strict == 0)
		{
			regcomp(&camembert.regex, "nyan", REG_NOSUB | REG_EXTENDED);
		}
#endif
	}

	/* Verbose stuff */
	if (camembert.errverb) {
		printf("=> µOptions: \n");
		printf("\t- Recursive mode %s (default = enabled)\n",(camembert.recursive==1?"enabled":"disabled"));
		printf("\t- Strict mode %s (default = disabled)\n",(camembert.strict==1?"enabled":"disabled"));
		printf("\t- Quiet mode %s (default = disabled)\n",(camembert.quiet==1?"enabled":"disabled"));
		printf("\t- Bare mode %s (default = disabled)\n",(camembert.bare==1?"enabled":"disabled"));
		printf("\t- Color mode %s (default = disabled)\n",(camembert.color==1?"enabled":"disabled"));
		printf("\t- Hidden files' protection is %s (default = enabled).\n\t%s",
			(camembert.hidden==0?"enabled":"disabled"),(camembert.hidden==0?"NOTHING EMBARASSING WILL BE FOUND!\n":"ANYTHING HIDDEN *CAN* MATCH\n"));
			easter();
		printf("\t- Verbose mode enabled (default = disabled)\n");
		printf("\t- Output filename is %s (default = none)\n",camembert.ofname);
		printf("\t- Seek filename is %s (default = nyan)\n",camembert.filename);
		printf("\t- Used rootpath is %s (default = ./)\n",camembert.rootpath);
	}

	int founds = 0;

	textcolor(0,7,0,camembert.color);printf("");
	FILE * ofpnt = NULL;
	if(strcmp(camembert.ofname," ") != 0) {
		ofpnt = fopen(camembert.ofname,"r+");
		if(ofpnt == NULL) {
			char errstr[500];
			textcolor(0,7,0,camembert.color);
			sprintf(errstr,"Error with %s",camembert.ofname);
			perror(errstr);
			printf("mucro keeps going, without output file\n");
		}
		else {
			/* We only use fseek if ofpnt have been set (!= NULL) */
			fseek(ofpnt, 0, SEEK_END);
			char anothertowrite[255];
			if(!camembert.bare) {
				fwrite("-Mµcro Output-\n",16,1,ofpnt);
				sprintf(anothertowrite,"Mµcro search result for %s :\n",camembert.filename);
				fwrite(anothertowrite,28+strlen(camembert.filename),1,ofpnt);
			}
			else {
				sprintf(anothertowrite,"# Mµcro : %s\n",camembert.filename);
				fwrite(anothertowrite,12+strlen(camembert.filename),1,ofpnt);
			}
		}
	}

	camembert.outfile = ofpnt;
	char rep[255] = ".";
	char * pntrep = rep;
	if(!strcmp(camembert.rootpath, " ") == 0) {
		pntrep = camembert.rootpath;
	}

	textcolor(1,1,0,camembert.color);
	if(!camembert.bare) {
		printf("Looking for '%s' %s %s ...",
			camembert.filename,
			(camembert.recursive == 1?"from":"in"),
			(strcmp(camembert.rootpath," ") == 0?"current path":camembert.rootpath));
		textcolor(0,7,0,camembert.color);
		printf("\n");
	}
	textcolor(0,7,0,camembert.color);

	founds = ls(pntrep, camembert);

	textcolor(1,2,0,camembert.color);
	if(!camembert.bare) {
		printf("Found %d times.\n", founds);

		if(ofpnt != NULL) {
			char towrite[200];char neonum[6];
			sprintf(towrite, "Found %d times.\n", founds);
			sprintf(neonum, "%d", founds);
			fwrite(towrite,14+strlen(neonum),1,ofpnt);
			fclose(ofpnt);
		}
	}

#ifndef WIN32
	if (camembert.strict == 0)
	{
		/* Liberating regex's memory */
		regfree (&camembert.regex);
	}
#endif

	textcolor(0,7,0,camembert.color);
	return (EXIT_SUCCESS);
}

/* FileSystem parser function, aka. LS Recursive */
int ls(char rep[], struct u_option camembert) {

	int founds = 0;
	DIR *directory = opendir(rep);
	if(directory == NULL) {
		if(!camembert.errverb == 0) {
			char errorstr[500];
			sprintf(errorstr, "Error with %s ", rep);
			perror(errorstr);
		}
		return founds;
	}

	struct dirent *iterator;
	while((iterator = readdir(directory)) != NULL) {
		/* We have to avoid '.', '..' and .* directories *
		 * (current, parent and hidden)					 */

		if(strcmp(iterator->d_name,".") == 0) {continue;}
		if(strcmp(iterator->d_name,"..") == 0) {continue;}
		if(iterator->d_name[0] == '.' && camembert.hidden == 0) {continue;}

		// TODO: Use here future regex_analyzer function
		//if((regex_find(camembert.filename,iterator->d_name) == 1 && camembert.strict == 0)
		//if(regex_cmp(camembert.filename,iterator->d_name) == 1) {
#ifndef WIN32
		int matching = 1;
		if (camembert.strict == 0)
		{
			matching = regexec (&camembert.regex, iterator->d_name, 0, NULL, 0);
		}
#endif

#ifdef WIN32
		if (
		(strstr(iterator->d_name,camembert.filename) != NULL && camembert.strict == 0)||
		(strcmp(iterator->d_name,camembert.filename) == 0 && camembert.strict == 1))
#else
		if (camembert.strict == 1 && (strcmp(camembert.filename, iterator->d_name) == 0)
		|| (camembert.strict == 0 && matching == 0))
#endif
		{

			++founds;
			if(!camembert.quiet) {
				if(!camembert.bare) {textcolor(0,1,0,camembert.color);printf("Found at ");textcolor(0,2,0,camembert.color);}

				printf("%s%s%s", rep, (rep[strlen(rep)-1] == '/'?"":"/"), iterator->d_name);
				textcolor(0,7,0,camembert.color);
				printf("\n");

				if(camembert.outfile != NULL) {

					char towrite[500];
					if(!camembert.bare) {
						sprintf(towrite,"Found at %s%s%s\n",rep,
#ifndef WIN32
							(rep[strlen(rep)-1] == '/'?"":"/"),
#else
							(rep[strlen(rep)-1] == '\\'?"":"\\"),
#endif
							iterator->d_name);
						fwrite(towrite,11+strlen(rep)+strlen(iterator->d_name),1,camembert.outfile);
					}
					else if(camembert.bare) {
						sprintf(towrite,"%s%s%s\n",rep,
#ifndef WIN32
							(rep[strlen(rep)-1] == '/'?"":"/"),
#else
							(rep[strlen(rep)-1] == '\\'?"":"\\"),
#endif
							iterator->d_name);
						fwrite(towrite,strlen(towrite),1,camembert.outfile);
					}
				}
			}
		}

		/*
		  If it's a dir, and if we are in recursive mode, let's go in it
		*/
		if((int)(iterator->d_type) == 4 && camembert.recursive == 1) {
			char nwdir[255];
			sprintf(nwdir,"%s%s%s",rep,
#ifndef WIN32
							(rep[strlen(rep)-1] == '/'?"":"/"),
#else
							(rep[strlen(rep)-1] == '\\'?"":"\\"),
#endif
							iterator->d_name);
			founds += ls(nwdir, camembert);
		}
	}
	closedir(directory);
	return founds;
}

/* Function printing help line, it seems logic */
void help() {
	printf("Mµcro, v0.1.1 ßý Mg, CopyRight 2015\n\n");
	printf("	mucro [-hsnq|filename] {-o filename}\n");
	printf("\nOptions :\n");
	printf("    -h | -help 	        : print the option (this)\n");
	printf("    -s | -strict        : search only the exact string, don't use regex\n");
	printf("    -n | -non-recursive : search only in this directory\n");
	printf("    -q | -quiet		: Quiet mode. Only print number of matches\n");
	printf("    -o | -output 	: Save the output to the filename\n");
	printf("    -b | -bare		: Print/save only the locations of files\n");
	printf("    -r | -root		: Directory's path to start searching from\n");
	printf("    -t | -hidden	: Seach even in hidden directories (which their names start by '.')\n");
	printf("    -c | -color		: Enables colors (always turned off)\n");
	printf("    -v | -verbose	: Show errors in stdout\n");
	printf("    filename       : name of the file to search\n");
	printf("If you need some help contact me at mg<dot>minetest<at>gmail<dot>com or open an issue on the bucktracker at http://github.com/LeMagnesium/mucro/issues/ .\n");
}

/* Mµcro options' parser function */
struct u_option u_parse_opt(int argc, char * argv[]) {

	struct u_option returned;

	/* Here are initialized the default values for µ_option */
	returned.strict		= 0;
	returned.recursive 	= 1;
	returned.quiet 		= 0;
	returned.bare 		= 0;
	returned.color		= 0;
	returned.hidden 	= 0;
	returned.errverb 	= 0;
	returned.filename 	= "nyan";
	returned.ofname 	= " ";
	returned.rootpath 	= " ";
	returned.outfile 	= NULL;


	int i = 1;
	while(i < argc) {

		if(strcmp(argv[i],"-help") == 0 || strcmp(argv[i],"-h") == 0) 			{help();exit(0);}
		if(strcmp(argv[i],"-strict") == 0 || strcmp(argv[i],"-s") == 0) 		{returned.strict = 1;i++;continue;}
		if(strcmp(argv[i],"-non-recursive") == 0 || strcmp(argv[i],"-n") == 0) 	{returned.recursive = 0;i++;continue;}
		if(strcmp(argv[i],"-quiet") == 0 || strcmp(argv[i],"-q") == 0) 			{returned.quiet = 1;i++;continue;}
		if(strcmp(argv[i],"-b") == 0 || strcmp(argv[i],"-bare") == 0) 			{returned.bare = 1;i++;continue;}
		if(strcmp(argv[i],"-v") == 0 || strcmp(argv[i],"-verbose") == 0) 		{returned.errverb = 1;i++;continue;}
		if(strcmp(argv[i],"-t") == 0 || strcmp(argv[i],"-hidden") == 0) 		{returned.hidden = 1;i++;continue;}

		if(strcmp(argv[i],"-c") == 0 || strcmp(argv[i],"-color") == 0) {
#ifndef WIN32
			returned.color = 1;
#else
			printf("Colors asked, but system is Windows. Ignoring\n");
#endif
			i++;continue;
		}
		if(strcmp(argv[i],"-r") == 0 || strcmp(argv[i],"-root") == 0) {
			if (i == argc-1) {printf("No path given with -r... See mucro -h.\n");exit(0);}
			returned.rootpath = argv[++i];i++;continue;
		}

		if(strcmp(argv[i],"-o") == 0 || strcmp(argv[i],"-output") == 0) {
			if (i == argc-1) {printf("No filename entered for -o... See mucro -h.\n");exit(0);}
			returned.ofname = argv[++i];i++;continue;
		}

		if(argv[i][0] == '-') {
			printf("Unknown option %s. Enter mucro -h to see available help.\n", argv[i]);
			exit(0);
		}

		else {
			returned.filename = argv[i];i++;continue;
		}
	}

#ifndef WIN32
	if (returned.strict == 0)
	{
		/* If strict mod is turned off, defining regex */
		int err;
		err = regcomp (&returned.regex, returned.filename,REG_NOSUB | REG_EXTENDED);
		if (err != 0)
		{
			printf("\"%s\" is not a valid regex! Terminated.\n",returned.filename);
			exit(1);
		}
	}
#endif

	return returned;
}

/* Text's coloring function */
void textcolor(int attr, int fgcolor, int bgcolor, int enabled) {
	if (enabled == 0) {return;}
	char colcomm[13];
	sprintf(colcomm,"%c[%d;%d;%dm",0x1B, attr, fgcolor+30, bgcolor+40);
	printf("%s", colcomm);
}

/* ? */
void easter() {

	/* Determine whether we are on easter */
	struct tm now;
	time_t nowadays;
	time (&nowadays);
	now = *localtime(&nowadays);
	int year = now.tm_year + 1900;
	int a,b,c,d,e,f,g;
	a = year % 19 +1;
	b = year/ 100 + 1;
	c = (3*b) / 4 - 12;
	d = (8 * b + 5) / 25 - 5;
	e = (year * 5) / 4 - 10 - c;
	f = ((11 * a + 20 + d - c)  % 30 + 30) % 30;
	if (f == 24 || (f == 25 && a > 11)) f++;
	g = 44 - f;
	if (g < 21) g+=30;
	int fin = g + 7 - (e + g) % 7;
	// fin is the amount of days after March 1st
	if (!((fin <= 31 && now.tm_mday == fin && now.tm_mon == 2)
		|| (fin > 31 && now.tm_mday == fin-31 && now.tm_mon == 3))) return; // Must be March, and day == fin, or April, and day == fin - 31;
	printf(	"\t#############################################################\n\t#                    .________.                             #\n\t#                  ./##########\\_.                          #\n\t#               _./###############\\.                        #\n\t#             ./###################|                        #\n\t#            /##########_.##########\\                       #\n\t#            |######___/ |###########|                      #\n\t#            |_____/      \\######____|                      #\n\t#            |             \\####/    |                      #\n\t#            |-\\    /--\\    \\##/     |                      #\n\t#            |()\\  | () |    \\|     /                       #\n\t#             \\ //  \\--/           |                        #\n\t#              |/._                |                        #\n\t#              |         /          \\                       #\n\t#               \\ ======/          / \\_.                    #\n\t#                \\.               /  _/ \\__.                #\n\t#            .___/ \\_____________/  /     / \\___.__.        #\n\t#        .__/   /  |   \\     /     | ____/__/       \\_.     #\n\t#     ._/      /   |   /\\___/\\    / /      \\        |  \\    #\n\t#    /|       |    \\__/ /   \\ \\__/ |        |       |   \\   #\n\t#   / |        \\       |     |     |       /        /    |  #\n\t#  /  |        /       |     |     |      /        |     |  #\n\t#  |  |       |        |     |     |      \\        /      \\ #\n\t#  |  |       |        |     |    /        \\      |  __   | #\n\t#              \\       |     |   /         /      | /  \\  | #\n\t#                      |     |                    |/    | | #\n\t#                      \\      \\                         |/  #\n\t#############################################################\n\t#   | |=  \\ / /-\\ | |  |/ |\\| /-\\ \\  /  \\  / |_| /-\\ -x-    #\n\t#   | |    |  \\-/ |_|  |\\ | | \\-/ \\/\\/  \\/\\/ | | | |  |     #\n\t#                                                           #\n\t#                    |  |\\/| |= /-\\ |\\|                     #\n\t#                    |  |  | |- | | | |                     #\n\t#############################################################\n");

}
