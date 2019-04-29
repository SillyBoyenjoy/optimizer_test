#include "auxfunc.h"

double GetTime(void){
    struct timeval s_time;
    double time;
    gettimeofday(&s_time, 0);
    time = (double)(s_time.tv_sec*1000000 + s_time.tv_usec);
    time = time/1000000;
    return time;
}

int PathConv(char *path, char *mid_path){
	char tmp[MAXFILENAME] = "./";
	strcat(tmp, mid_path);
	strcat(tmp, path);
	strcpy(path, tmp);
	return 1;
}

int GetRefval(char *Filename, double *Objval){
    FILE *fp; 
	char line[MAXLINE], *cval;

    if( (fp=fopen(Filename, "r")) == NULL ){
        printf("cannot open file %s\n", Filename);
		return 0;
    } 

	if(fgets(line, MAXLINE, fp) == NULL){
		printf("cannot get the first line\n");
		return 0;
    }
    fclose(fp);

	cval = line;
	while((*cval < 48)||(*cval > 57)){
		if ( *cval == 45 )
			break;
		cval++;
	}
	*Objval = atof(cval);
    return 1;
}

int GetFilename(const char *Filedir, char (*Filies)[MAXFILENAME]){
	int f_n = 0;
	FILE *fp;

	if( (fp=fopen(Filedir, "r")) == NULL ){
        printf("Cannot open file %s\n", Filedir);
		return 0;
    } 

	while(fgets(*(Filies+f_n), MAXFILENAME, fp)){
		Filies[f_n][strlen(Filies[f_n])-1] = 0;
		Filies[f_n][strlen(Filies[f_n])-1] = 0;
		f_n++;
    }

    fclose(fp);
	return f_n;
}

int ExFilename(char *Path, char *Filename, char *Extensions, char *Exfilename){
	memset(Exfilename, 0, MAXFILENAME);
	strcpy(Exfilename, Path);
	strcat(Exfilename, Filename);
    strcat(Exfilename, Extensions);
	return 1;
}

int CopyLogFile(FILE *Destfile){
    FILE *Srcfile; 
    char line[MAXLINE];

    if ( (Srcfile=fopen("./log/cplexlog.txt", "r+")) == NULL ){
        printf("Cannot open file cplexlog.log.\n");
        return 0;
    }

    while ( fgets(line, MAXLINE, Srcfile) ){
        fputs(line, Destfile);
    }
    return 1;
}

