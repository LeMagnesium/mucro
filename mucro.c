/* Program ßý Mg. */

/*
	Mµcro, v0.0.8 ßý Mg, CopyLeft (¢) 2014.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "libutil.h"

/* µ_OPTIONS STRUCT */
struct u_option {
	int recursive;
	int strict;
	int quiet;
	char * ofname;
	char * filename;
};

/* LS recursive */
int ls(char rep[], char* seek, int recursive, int strict, int quiet, FILE * ofpnt);
/* Help, I need somebody. Help */
void help();
/* Options parser */
struct u_option u_parse_opt(int argc, char *argv[]);

int main(int argc, char *argv[]) {

	/*
	  You are in text mode, you cannot start catfish, you want to find a
	  file on your disk, and you don't want to spend hours and hours to
	  browse the package list with aptitude... Use Mµcro, it's easy to use
	  and very easy to rewrite if you want to modify it.
	*/
	/*int arc = 2;
	return 0; RELIKA */

	if (argc == 1) {printf("No filename given... Type mucro -h for help.\n");exit(0);}

	/* Option parse */
	struct u_option camembert;
	camembert = u_parse_opt(argc, argv);
	if (strcmp(camembert.filename,"nyan") == 0) {
		printf("No filename entered, using dummy instead, hehe!\n");
	}

	int founds = 0; /* How many time we do find the file in the directory */
	textcolor(1,1,0);
	printf("Looking for %s %s current path...\n",
		camembert.filename,
		(camembert.recursive == 1?"from":"in"));
	textcolor(0,7,0);printf("");
	FILE * ofpnt = NULL;
	if (strcmp(camembert.ofname," ") != 0) {
		ofpnt = fopen(camembert.ofname,"r+");
		if (ofpnt == NULL) {
			ofpnt = fopen(camembert.ofname,"w");
			if (ofpnt == NULL) {
				char errstr[500];
				textcolor(0,7,0);
				sprintf(errstr,"Error with %s",camembert.ofname);
				perror(errstr);
				printf("mucro keeps going, without output file\n");
			}
		}
		fwrite("-Mµcro Output-\n",16,1,ofpnt);
		char anothertowrite[255];
		sprintf(anothertowrite,"Mµcro search result for %s :\n",camembert.filename);
		fwrite(anothertowrite,28+strlen(camembert.filename),1,ofpnt);
	}
	textcolor(0,7,0);
	founds = ls(".",camembert.filename,camembert.recursive,camembert.strict,camembert.quiet,ofpnt);
	textcolor(1,2,0);
	printf("Found %d times.\n", founds);
	if (ofpnt != NULL) {
		char towrite[200];char neonum[6];
		sprintf(towrite, "Found %d times.\n", founds);
		sprintf(neonum, "%d", founds);
		fwrite(towrite,14+strlen(neonum),1,ofpnt);
		fclose(ofpnt);
	}
	textcolor(0,7,0);
}

int ls(char rep[], char* seek, int recursive, int strict, int quiet, FILE * ofpnt) {
	/*
	  LS RECURSIVE FUNCTION
	*/
	int founds = 0;
	DIR *directory = opendir(rep);
	if (directory == NULL) {
		char errorstr[500];
		textcolor(0,7,0);
		sprintf(errorstr, "Error with %s ", rep);
		perror(errorstr);
		textcolor(0,7,0);return founds;
	}

	struct dirent *iterator;
	while ((iterator = readdir(directory)) != NULL) {
		if (strcmp(iterator->d_name,".") == 0) {continue;}
		if (strcmp(iterator->d_name,"..") == 0) {continue;}

		if (
		(strstr(iterator->d_name,seek) != NULL && strict == 0)||
		(strcmp(iterator->d_name,seek) == 0 && strict == 1)
		) {
			//textcolor(0,3,0);printf("%d:", ++founds);
			//printf("-increase-\n");
			++founds;
			if(!quiet) {
				textcolor(0,1,0);printf("Found at ");
				textcolor(0,2,0);printf("%s/%s", rep, iterator->d_name);
				textcolor(0,7,0);printf("\n");
				if (ofpnt != NULL) {
					char towrite[500];
					sprintf(towrite,"Found at %s/%s\n",rep,iterator->d_name);
					fwrite(towrite,11+strlen(rep)+strlen(iterator->d_name),1,ofpnt);
				}
			}
		}

		/*
		  If it's a dir, and if we are in recursive, let's go in there
		*/
		if ((int)(iterator->d_type) == 4 && recursive == 1) {
			char nwdir[255];
			sprintf(nwdir,"%s/%s",rep,iterator->d_name);
			//textcolor(0,4,0);printf("Opening %s...\n",nwdir);
			//textcolor(0,7,0);
			founds += ls(nwdir,seek,1,strict,quiet,ofpnt);
			//if (q != 0) {printf("-?:%d:increase:%d:?-\n",founds+=q,q);}
			//printf("Founds %d\n",founds);
			if (founds == 65535) {
				printf("Ouch, too many stuff founds!\n");
				return founds;
			}
		}
	}
	closedir(directory);
	return founds;
}

void help() {
	printf("Mµcro, v0.0.8 ßý Mg, CopyLeft (¢) 2014\n\n");
	printf("	mucro [-hsnq|filename] {-o filename}\n");
	printf("\nOptions :\n");
	printf("    -h | -help 	        : print the option (this)\n");
	printf("    -s | -strict        : search only the exact string\n");
	printf("    -n | -non-recursive : search only in this directory\n");
	printf("    -q | -quiet		: Quiet mode. Only print number of matches\n");
	printf("    -o | -output 	: Save the output to the filename");
	printf("    filename       : name of the file to search\n");
	printf("If you need some help contact me at mg.minetest@gmail.com\n");
}

struct u_option u_parse_opt(int argc, char * argv[]) {
	struct u_option returned;
	returned.recursive = 1;returned.strict = 0;
	returned.filename = "nyan";returned.quiet = 0;
	returned.ofname = " ";
	int i = 1;
	while (i < argc) {
		if (strcmp(argv[i],"-help") == 0 || strcmp(argv[i],"-h") == 0) {help();exit(0);} /* HELP! */
		if (strcmp(argv[i],"-strict") == 0 || strcmp(argv[i],"-s") == 0) {
			returned.strict = 1;i++;continue;
		}
		if (strcmp(argv[i],"-non-recursive") == 0 || strcmp(argv[i],"-n") == 0) {
			returned.recursive = 0;i++;continue;
		}
		if (strcmp(argv[i],"-quiet") == 0 || strcmp(argv[i],"-q") == 0) {
			returned.quiet = 1;i++;continue;
		}
		if (strcmp(argv[i],"-o") == 0 || strcmp(argv[i],"-output") == 0) {
			if (i == argc-1) {printf("No filename entered for -o... See mucro -h.\n");exit(0);}
			returned.ofname = argv[++i];i++;continue;
		}
		if (argv[i][0] == '-') {
			printf("Unknown option %s. Enter mucro -h to see help.\n", argv[i]);
			exit(0);
		}
		else {
			returned.filename = argv[i];i++;continue;
		}
	}
	return returned;
}
