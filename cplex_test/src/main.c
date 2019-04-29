/* --------------------------------------------------------------------------
* File: cplex_text
* Version 1.0.4
* --------------------------------------------------------------------------
* Author: Liu Chong
* ExLibrary: ilcplex/cplex.h
* --------------------------------------------------------------------------
*/
#include "main.h"

int main (int argc, char *argv[]){
	/* Declare and allocate space for the variables and arrays where we will
		store the optimization results including the status, objective value,
		and variable values. */

	double  objval = 0, refval = 0, GapVal = 0.001;
	double	s_time = 0, e_time = 0, d_time = 0, t_time = 0.0, time_limit = 600.0;
	double  *x = NULL;
	char    logfile[MAXFILENAME] = "./log/cplex_log", openfile[MAXFILENAME] = {0}, solfile[MAXFILENAME] = {0};
	char    path_e[MAXFILENAME] = "exs/", path_r[MAXFILENAME] = "res/", path_f[MAXFILENAME] = "filies.txt";
	char    filies[MAXFILIES][MAXFILENAME] = {0};
    char    *SetName = NULL;

	CPXENVptr     env = NULL;
	CPXLPptr      mip = NULL;
	CPXINT		  Threads = 0;
    int           i, f_screen = 0, f_log = 0;
	int           status, s_status;
	int			  f_i, f_n = 0, accept_n = 0, fail_n = 0, infesiable = 0, time_excee = 0, sol_wrong = 0;
    TESTenv tenv;

    /* Input the arguments */
	if ( argc < 3 ){
        printf("Deficient arguments.\n");
		goto TERMINATE;
	}

    SetName = argv[1];
    if ( (strncmp(SetName, "BenchmarkSets/", strlen("BenchmarkSets/")) != 0)    &&
         (strncmp(SetName, "EasySets/", strlen("EasySets/")) != 0)              &&
         (strncmp(SetName, "QPLIB/", strlen("QPLIB/")) != 0) ){
            printf("Unknonwn sets.\n");
            goto TERMINATE;
        }
    strcat(logfile, "_");
    strncat(logfile, SetName, strlen(SetName)-1);

    for ( i = 2; i < argc; ++i ){
        if ( strncmp(argv[i], "-Threads=", strlen("-Threads=")) == 0 ){
            Threads = atoi(argv[i] + strlen("-Threads="));
            printf("Set Threads %d\n", Threads);
            strcat(logfile, "_");
            strcat(logfile, argv[i] + strlen("-Threads="));
            strcat(logfile, ".txt");
        }
        else if ( strncmp(argv[i], "-Screen=on", strlen("-Screen=on")) == 0 ){
            f_screen = 1;
            printf("Screen display: on.\n");
        }
        else if ( strncmp(argv[i], "-Screen=off", strlen("-Screen=off")) == 0 ){
            f_screen = 0;
            printf("Screen display: off.\n");
        }
        else if ( strncmp(argv[i], "-Log=on", strlen("-Log=on")) == 0 ){
            f_log = 1;
            printf("Log record: on.\n");
        }
        else if ( strncmp(argv[i], "-Log=off", strlen("-Log=off")) == 0 ){
            f_log = 0;
            printf("Log record: off.\n");
        }
        else if ( strncmp(argv[i], "-Gap=", strlen("-Gap=")) == 0 ){
            GapVal = atof(argv[i] + strlen("-Gap="));
            printf("Set mip gap: %0.2f\n", GapVal);
        }
        else{
            printf("Unknonwn arguments: %s.\n", argv[i]);
        }
    }
    
    /* Set the Sets type, 1 means quadratic programming */
    if ( strcmp(argv[1], "QPLIB/") == 0 ){
        tenv.prob = 1;
    }
    else{
        tenv.prob = 0;
    }
    
    FILE *log = fopen(logfile, "w+");
    if ( log == NULL ){
        printf("Open log file failed!\n");
        goto TERMINATE;
    }

	/* Set the time limit according to threads */
    if ( tenv.prob == 0 ){ 
        switch ( Threads ) {
        case 1:
            time_limit = 75.0;
            break;
        case 4:
            time_limit = 37.0;
            break;
        case 8:
            time_limit = 38.0;
            break;
        case 12:
            time_limit = 75.0;
            break;
        case 40:
            time_limit = 55.0;
            break;
        default:
            usage (argv[0]);
            goto TERMINATE;
        }
    }

    printf("Now test in %d threads, time limit is %f.\n", Threads, time_limit);

    /* Initialize the CPLEX environment */
    env = CPXopenCPLEX (&status);
    if ( env == NULL ) {
	char  errmsg[CPXMESSAGEBUFSIZE];
	fprintf (stderr, "Could not open CPLEX environment.\n");
	CPXgeterrorstring (env, status, errmsg);
	fprintf (stderr, "%s", errmsg);
	goto TERMINATE;
    }

    /* Set solution time limit */
    status = CPXsetdblparam (env, CPXPARAM_TimeLimit, time_limit);
    if ( status ) {
        fprintf (stderr, "Failure to set the time limit, error %d.\n", status);
        goto TERMINATE;
    }

    /* Set solution gap as 0.1% */
    status = CPXsetdblparam (env, CPXPARAM_MIP_Tolerances_MIPGap, GapVal);
    if ( status ) {
        fprintf (stderr, 
                "Failure to set the mip gap, error %d.\n", status);
        goto TERMINATE;
    }

    /* Turn on output to the screen if screen is on */
    if ( f_screen == 1 ){
        status = CPXsetintparam (env, CPXPARAM_ScreenOutput, CPX_ON);
        if ( status ) {
            fprintf (stderr, 
                    "Failure to turn on screen indicator, error %d.\n", status);
            goto TERMINATE;
        }    
    }

	/* Set threads according to argv[1]*/
	status = CPXsetintparam (env, CPX_PARAM_THREADS, Threads);
	if ( status ) {
		fprintf (stderr, 
				"Failure to set the threads, error %d.\n", status);
		goto TERMINATE;
	}
   
	/* Create the problem, using the filename as the problem name */
	mip = CPXcreateprob (env, &status, "mip_test");
	if ( mip == NULL ) {
		fprintf (stderr, "Failed to create MIP.\n");
		goto TERMINATE;
	}
   
	/* Create the filies dictionary */
	PathConv(path_f, SetName);
	PathConv(path_e, SetName);
	PathConv(path_r, SetName);

	if((f_n = GetFilename(path_f, filies)) == 0){
		printf("Could not obtain any filies\n");
		goto TERMINATE;
	}

	for(f_i = 0; f_i < f_n; f_i++){
	/* Now read the file, and copy the data into the created mip */
        if ( tenv.prob == 0 ){
            ExFilename(path_e, filies[f_i], MPS, openfile);
        }
        else{
            ExFilename(path_e, filies[f_i], LP, openfile);
        }
	    status = CPXreadcopyprob (env, mip, openfile, NULL);
	    if ( status ) {
            tenv.test_sit = READ_CASE_FAIL;
	        goto res_handle;
	    }

        /* Optimize the problem, obtain solution and solution time. */
        if ( f_log == 1 ){
            status = CPXsetlogfilename(env, "./log/cplexlog.txt", "w+");
            if ( status ){
                printf("Open cplexlog.txt failed!");
                goto TERMINATE;
            }
        }

        s_time = GetTime();                         //Set time

        status = CPXmipopt (env, mip);
        if ( status ) {
                tenv.test_sit = CPLEX_OPT_FAIL;
                goto res_handle;
        }

        e_time = GetTime();
        d_time = e_time - s_time;
	    t_time += d_time;

        /* Cheak that the problem is fesiable or time exceeded? */
        s_status = CPXgetstat (env, mip);
        if ( s_status == 103 ){	/* CPXMIP is infisiable */
            tenv.test_sit = CASE_INFESIABLE;
                goto res_handle;
        }
        else if ( s_status == 108 ){
                tenv.test_sit = TIME_OUT | CASE_INFESIABLE;
                goto res_handle;
        }

        /* If the problem is QP and fesiable, just accept */
        if ( tenv.prob == 1 ){
            tenv.test_sit = ACCEPT;
            goto res_handle;
        }

        status  = CPXgetobjval (env, mip, &objval);
        if ( status ) {
            fprintf (stderr,"Failed to obtain objective value.\n");
            goto TERMINATE;
        }

        /* Obtain the reference val for .sol file. */
        ExFilename(path_r, filies[f_i], SOL, solfile);
        status= GetRefval(solfile, &refval);
        if ( status == 0 ){
            fprintf (stderr,"Failed to obtain reference value.\n");
            goto TERMINATE;
        }

        /* Print the test result */
        if( ((objval >= 0) && (objval<=1.01*refval) && (objval>=0.99*refval))	||
            ((objval < 0) && (objval>=1.01*refval) && (objval<=0.99*refval))	){
                tenv.test_sit = ACCEPT;
                goto res_handle;
        }
        else if( s_status == 107 ){
                tenv.test_sit = TIME_OUT | SOL_FAULT;
                goto res_handle;
        }
        else{
                tenv.test_sit = SOL_FAULT;
                goto res_handle;
            fail_n++;	
        }

res_handle:
        fprintf(log, "\n*******************LogFile:%3d*******************\n", f_i);
        if ( tenv.test_sit == ACCEPT ){
            accept_n++;
            printf("\033[1m\033[32mCase %s is acceptted! \033[0m\n", filies[f_i]);
            fprintf(log, "Case %s is acceptted!\n", filies[f_i]);    
        }
        else{
            fail_n++;
            if ( tenv.test_sit == READ_CASE_FAIL ){
                printf("\033[1m\033[31mCase %s read failed!\033[0m\n", filies[f_i]);
                fprintf(log, "Case %s read mps file failed!\n", filies[f_i]);
            }
            else if ( tenv.test_sit == CPLEX_OPT_FAIL ){ 
                sol_wrong++;
                printf("\033[1m\033[31mCPX optimize failed!. \033[0m\n");
                fprintf(log, "Case %s optimize failed!\n", filies[f_i]);
            }
            else if ( tenv.test_sit == CASE_INFESIABLE ){
                infesiable++;
                printf("\033[1m\033[31mCase %s is failed! MIP is infisiable.i \033[0m\n", filies[f_i]);
                fprintf(log, "Case %s is failed! MIP is infisiable. \n", filies[f_i]);
            }
            else if ( tenv.test_sit == (CASE_INFESIABLE | TIME_OUT) ){
                time_excee++;
                printf("\033[1m\033[31mCase %s is failed! Time limit exceeded and no integer solution. \033[0m\n", filies[f_i]);
                fprintf(log, "Case %s is failed! Time limit exceeded and no integer solution.\n", filies[f_i]);
            }
            else if ( tenv.test_sit == (SOL_FAULT | TIME_OUT) ){
                sol_wrong++;
                printf("\033[1m\033[31mCase %s is failed! Time limit exceeded and the solution is %f while the objective value is %f. \033[0m\n", filies[f_i], refval, objval);
                fprintf(log, "Case %s is failed! Time limit exceeded and the the solution is %f while the objective value is %f.\n", filies[f_i], refval, objval);
            }
            else if ( tenv.test_sit == SOL_FAULT ){
                sol_wrong++;
                printf("\033[1m\033[31mCase %s is failed! The the solution is %f while the objective value is %f. \033[0m\n", filies[f_i], refval, objval);
                fprintf(log, "Case %s is failed! The the solution is %f while the objective value is %f.\n", filies[f_i], refval, objval);
            }
        }
        printf("Time consuming %f sec.\n%d is left.\n", d_time, f_n-f_i-1);
        
        fprintf(log, "Time consuming %f sec.\n", d_time);
        if ( f_log == 1 ){
            fprintf(log, "*****************LogFileDetail*******************\n");
            CopyLogFile(log);           
        }

    /* END FOR */
    }

TERMINATE:

	printf("Running under %d threads.\n", Threads);
    printf("Total Time Comsuming %7.3f sec.\n", t_time);
	printf("%d accpeted, %d failed in %f sec, %d is infesiable, %d exceeded time, %d solution is wrong.\n", accept_n, fail_n, t_time, infesiable, time_excee, sol_wrong);

	fprintf(log, "\n==========Running under %d threads=========\n", Threads);
	fprintf(log, "====Total Time Comsuming %7.3f sec.=====\n", t_time);
	fprintf(log, "%d accpeted.\n", accept_n); 
    fprintf(log, "%d failed:\n", fail_n);
	fprintf(log, "\t%d is infesiable.\n", infesiable);
	fprintf(log, "\t%d exceeded time.\n", time_excee);
	fprintf(log, "\t%d solution is wrong.\n", sol_wrong);
	fclose(log);
   
	/* Free up the solution */

	free_and_null ((char **) &x);

	/* Free up the problem as allocated by CPXcreateprob, if necessary */

	if ( mip != NULL ) {
		status = CPXfreeprob (env, &mip);
		if ( status ) {
			fprintf (stderr, "CPXfreeprob failed, error code %d.\n", status);
		}
	}

	/* Free up the CPLEX environment, if necessary */

	if ( env != NULL ) {
		status = CPXcloseCPLEX (&env);

		/* Note that CPXcloseCPLEX produces no output,
			so the only way to see the cause of the error is to use
			CPXgeterrorstring.  For other CPLEX routines, the errors will
			be seen if the CPXPARAM_ScreenOutput indicator is set to CPX_ON. */

		if ( status ) {
		char  errmsg[CPXMESSAGEBUFSIZE];
			fprintf (stderr, "Could not close CPLEX environment.\n");
			CPXgeterrorstring (env, status, errmsg);
			fprintf (stderr, "%s", errmsg);
		}
	}
	return (status);
}/* END main */


/* This simple routine frees up the pointer *ptr, and sets *ptr to NULL */

static void 
free_and_null (char **ptr){
	if ( *ptr != NULL ) {
		free (*ptr);
		*ptr = NULL;
	}
} /* END free_and_null */ 

static void
usage (char *progname){
	fprintf (stderr,"Usage: %s -X filename\n", progname);
	fprintf (stderr,"  where X is num of threads:1, 4, 8 or 12 \n");
	fprintf (stderr,"  where filename is a directory file with extension .txt\n");
	fprintf (stderr,"  This program uses the CPLEX MIP optimizer.\n");
	fprintf (stderr," Exiting...\n");
} /* END usage */
