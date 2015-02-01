/* Program ßý Mg. */

/*
	Mµcro, v0.0.12 ßý Mg, CopyRight 2015.
	License : GPLv2 and further
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "regex.h"

/* µ_OPTIONS STRUCT */
struct u_option {
	int recursive;
	int strict;
	int quiet;
	int bare;
	int hidden;
	int errverb;
	char * ofname;
	char * filename;
	char * rootpath;
	FILE * stdout;
};


int ls(char rep[], struct u_option camembert);			/* LS recursive					*/
void help();											/* Help, I need somebody. Help 	*/
struct u_option u_parse_opt(int argc, char *argv[]);	/* Options parser				*/
void textcolor(int attr, int fgcolor, int bgcolor);		/* Color the text				*/

/* Main function */
int main(int argc, char *argv[]) {

	if(argc == 1) {printf("No filename given... Type mucro -h for help.\n");exit(0);}

	/* Option parse */
	struct u_option camembert;
	camembert = u_parse_opt(argc, argv);
	if(strcmp(camembert.filename,"nyan") == 0) {
		printf("No filename entered, using the standard instead, hehe!\n");
	}

	int founds = 0;

	textcolor(0,7,0);printf("");
	FILE * ofpnt = NULL;
	if(strcmp(camembert.ofname," ") != 0) {
		ofpnt = fopen(camembert.ofname,"r+");
		if(ofpnt == NULL) {
			char errstr[500];
			textcolor(0,7,0);
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

	camembert.stdout = ofpnt;
	char rep[255] = ".";
	char * pntrep = rep;
	if(!strcmp(camembert.rootpath, " ") == 0) {
		pntrep = camembert.rootpath;
	}

	textcolor(1,1,0);
	if(!camembert.bare) {
		printf("Looking for '%s' %s %s ...",
			camembert.filename,
			(camembert.recursive == 1?"from":"in"),
			(strcmp(camembert.rootpath," ") == 0?"current path":camembert.rootpath));
		textcolor(0,7,0);
		printf("\n");
	}
	textcolor(0,7,0);
	
	founds = ls(pntrep, camembert);

	textcolor(1,2,0);
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

	textcolor(0,7,0);
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
		if((strstr(iterator->d_name,camembert.filename) != NULL && camembert.strict == 0)
		|| (regex_cmp(camembert.filename,iterator->d_name) == 1 && camembert.strict == 1)) {

			++founds;
			if(!camembert.quiet) {
				if(!camembert.bare) {textcolor(0,1,0);printf("Found at ");textcolor(0,2,0);}

				printf("%s%s%s", rep, (rep[strlen(rep)-1] == '/'?"":"/"), iterator->d_name);
				textcolor(0,7,0);
				printf("\n");

				if(camembert.stdout != NULL) {
					
					char towrite[500];
					if(!camembert.bare) {
						sprintf(towrite,"Found at %s%s%s\n",rep, (rep[strlen(rep)-1] == '/'?"":"/"),iterator->d_name);
						fwrite(towrite,11+strlen(rep)+strlen(iterator->d_name),1,camembert.stdout);
					}
					else if(camembert.bare) {
						sprintf(towrite,"%s%s%s\n",rep, (rep[strlen(rep)-1] == '/'?"":"/"), iterator->d_name);
						fwrite(towrite,strlen(towrite),1,camembert.stdout);
					}
				}
			}
		}

		/*
		  If it's a dir, and if we are in recursive mode, let's go in it
		*/
		if((int)(iterator->d_type) == 4 && camembert.recursive == 1) {
			char nwdir[255];
			sprintf(nwdir,"%s%s%s",rep,(rep[strlen(rep)-1] == '/'?"":"/"),iterator->d_name);
			founds += ls(nwdir, camembert);
		}
	}
	closedir(directory);
	return founds;
}

/* Function printing help line, it seems logic */
void help() {
	printf("Mµcro, v0.0.12 ßý Mg, CopyRight 2015\n\n");
	printf("	mucro [-hsnq|filename] {-o filename}\n");
	printf("\nOptions :\n");
	printf("    -h | -help 	        : print the option (this)\n");
	printf("    -s | -strict        : search only the exact string\n");
	printf("    -n | -non-recursive : search only in this directory\n");
	printf("    -q | -quiet		: Quiet mode. Only print number of matches\n");
	printf("    -o | -output 	: Save the output to the filename\n");
	printf("    -b | -bare		: Print/save only the locations of files\n");
	printf("    -r | -root		: Directory's path to start searching from\n");
	printf("    -t | -hidden	: Seach even in hidden directories (which their names start by '.')\n");
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
	returned.hidden 	= 0;
	returned.errverb 	= 0;
	returned.filename 	= "nyan";
	returned.ofname 	= " ";
	returned.rootpath 	= " ";
	returned.stdout 	= NULL;

	
	int i = 1;
	while(i < argc) {

		if(strcmp(argv[i],"-help") == 0 || strcmp(argv[i],"-h") == 0) 			{help();exit(0);}
		if(strcmp(argv[i],"-strict") == 0 || strcmp(argv[i],"-s") == 0) 		{returned.strict = 1;i++;continue;}
		if(strcmp(argv[i],"-non-recursive") == 0 || strcmp(argv[i],"-n") == 0) 	{returned.recursive = 0;i++;continue;}
		if(strcmp(argv[i],"-quiet") == 0 || strcmp(argv[i],"-q") == 0) 			{returned.quiet = 1;i++;continue;}
		if(strcmp(argv[i],"-b") == 0 || strcmp(argv[i],"-bare") == 0) 			{returned.bare = 1;i++;continue;}
		if(strcmp(argv[i],"-v") == 0 || strcmp(argv[i],"-verbose") == 0) 		{returned.errverb = 1;i++;continue;}
		if(strcmp(argv[i],"-t") == 0 || strcmp(argv[i],"-hidden") == 0) 		{returned.hidden = 1;i++;continue;}
		
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
	
	return returned;
}

/* Text's coloring function */
void textcolor(int attr, int fgcolor, int bgcolor) {
	char colcomm[13];
	sprintf(colcomm,"%c[%d;%d;%dm",0x1B, attr, fgcolor+30, bgcolor+40);
	printf("%s", colcomm);
}
