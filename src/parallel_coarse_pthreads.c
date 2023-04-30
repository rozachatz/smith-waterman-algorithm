#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <pthread.h>
#include <sched.h>

double gettime(void){
	struct timeval ttime;
	gettimeofday(&ttime, NULL);
	return ttime.tv_sec+ttime.tv_usec * 0.000001;
}

int maxnumber(int a,int b,int c){
	int maxnum=-100;
	if(a>maxnum){ maxnum=a;}
	if(b>maxnum){maxnum=b;}
	if(c>maxnum){maxnum=c;}
	if(maxnum<0){maxnum=0;}
	return maxnum;
}

int match, mismatch,gap, traceback_steps, length[5000], w=0, traceback_steps=0, pairs=0,dim0=0,dim1=0;
double total_traceback_time=0, total_score_time=0;
char *source = NULL;
long bufsize;
char * str1="D: ";
char * str2="Q: ";

struct Water{
	int begin,end;
	FILE* f2;
};

void * Waterman(void * data){

	struct Water *sc = (struct Water *)data;
	int k;
	for(int q=sc->begin; q < sc->end ; q+=2 ){

		char* seq0=(char*)calloc(((length[q+1]-length[q])-3),1), *seq1=(char*)calloc(((length[q+2]-length[q+1])-3),1);
		int start0=length[q]+2, end0=length[q+1], start1=length[q+1]+2, end1=length[q+2];
		int a=start0, notneeded0=0, notneeded1=0, s=0;
		for (int u = 0; u < end0-start0; u++){
			if((int)source[a]==13 || (int)source[a]==10 || (int)source[a]==9 )notneeded0++;
			else {seq0[s]=source[a];s++;}
			a++;
		}
		int v=start1;s=0;
		for (int u = 0; u < end1-start1; u++){
			if((int)source[v]==13 || (int)source[v]==10 || (int)source[v]==9 )notneeded1++;
			else {seq1[s]=source[v];s++;}
			v++;
		}

	    /* Increment line count */
		pairs++;

		//***********************************************************************************************
		//IMPLEMENT ALGORITHM HERE...


		//important initializations
		int score, gap1, gap2,  n=end0-start0-notneeded0, m=end1-start1-notneeded1, c=m+1, r=n+1;
	    short int** dp;

	    dp = malloc(r * sizeof(short int*));
	    dim0+=r;dim1+=c;
	    
	    for (int i = 0; i < r; i++) {
	    	dp[i] = malloc(c * sizeof(short int));
	    }

	    for(int i=0;i<c;i++) dp[0][i]=0; 
	    	for(int i=0;i<r;i++) dp[i][0]=0;

	    	double time5=gettime();
	    	int max=0,min,tmp=0,temp=0;
	    	a=0;
	    	if(r>c)min=c;else min=r;

	    	for (int i = 0; i < r+c-1; i++) {
	    		k=i;
	    		if(i<min){tmp++;}
	    		else if(i>=min && i<maxnumber(c,r,-1) && i<c){temp++;}
	    		else if(i>=min && i<maxnumber(c,r,-1) && i>=c){a++;}
	    		else if(i>=maxnumber(c,r,-1)){a++;tmp--;temp++;}

	    		for (int j = a; j <tmp+a; j++){

	    			if(j!=0 && (tmp+a+temp-j-1)!=0){
	    				if(seq0[j-1]==seq1[tmp+a+temp-j-1-1]){
	    					score=dp[j-1][tmp+a+temp-j-1-1]+match;
	    				}
	    				else{
	    					score=dp[j-1][tmp+a+temp-j-1-1]+mismatch;
	    				}
	    				gap1=dp[j-1][tmp+a+temp-j-1]+gap;
	    				gap2=dp[j][tmp+a+temp-j-1-1]+gap;
	    				dp[j][tmp+a+temp-j-1]=maxnumber(score,gap1,gap2);
	    				if(dp[j][tmp+a+temp-j-1]>max) max=dp[j][tmp+a+temp-j-1];
	    			}
	    		}
	    	}


	    	double time6=gettime();
	    	int f=0;
	        total_score_time+=time6-time5;

			for(int i=1;i<n+1;i++){ //compare each letter of seq1 with seq2

				for(int j=1;j<m+1;j++){

					if(dp[i][j]==max) {

						f++;
						traceback_steps++;

						//TRACEBACK!
						 int l = n + m; // maximum possible length 
						 int d,p;  
						 d = i; p = j;
						 int start_D=j-1;

						 int xpos = l, ypos = l; 

					    // Final res
						 int * xans = malloc((l+1) * sizeof(int));
						 int * yans = malloc((l+1) * sizeof(int));
						 double time7=gettime();
						 while ( d != 0 && p !=0 && d<=n && p<=m)
						 { 	

						 	if (seq0[d-1]== seq1[p-1]) 
						 	{ 
						 		xans[xpos--] = (int)seq0[d - 1]; 
						 		yans[ypos--] = (int)seq1[p- 1]; 
						 		d--; p--; start_D--;
						 	} 
						 	else if (dp[d - 1][p - 1] + mismatch == dp[d][p]) 
						 	{ 
						 		xans[xpos--] = (int)seq0[d - 1]; 
						 		yans[ypos--] = (int)seq1[p - 1]; 
						 		d--; p--;start_D--; 
						 	} 
						 	else if (dp[d - 1][p] + gap == dp[d][p]) 
						 	{ 
						 		xans[xpos--] = (int)seq0[d - 1]; 
						 		yans[ypos--] = (int)'_'; 
						 		d--; 
						 	}
					        else //if (dp[k][p - 1] + gap == dp[k][p]) 
					        { 
					        	xans[xpos--] = (int)'_'; 
					        	yans[ypos--] = (int)seq1[p - 1]; 
					        	p--; 
					        }
					    }

					    double time8=gettime();
					    total_traceback_time+=time8-time7;
					    fprintf(sc->f2,"Match %d[Score: %d Start:%d Stop:%d]\n",f,max,start_D,j-1 );
					    fprintf(sc->f2,"\n" );
					    fprintf(sc->f2,"Q:	" );

					    for(int i=xpos+1;i<=l;i++){
					    	fwrite(&xans[i],1, 1,sc->f2 );
					    }

					    fprintf(sc->f2,"\n" );
					    fprintf(sc->f2,"D:	" );

					    for(int i=ypos+1;i<=l;i++){
					    	fwrite(&yans[i],1, 1,sc->f2 );
					    }

					    fprintf(sc->f2,"\n" );
					    free(xans);
					    free(yans);

					}	
				}
			}

			for (int z = 0; z < r; z++){
				free(dp[z]);
			}
			free(dp);
			free(seq0);free(seq1);
	}
}
	


// main program
int main(int argc,char *argv[]){ 

	double time0 = gettime();
	int optind=1,threads;
	char * path, * out_path;

	if(argc<13){
		printf("Inadequate/wrong arguments or flags.\n");
		printf("Usage: program -name ID -input PATH -match INT1 -mismatch INT2 -gap INT3 -threads INT4.\n");
		printf("Exiting..\n");
		return 1;
	}  

	while ((optind < argc) && (argv[optind][0]=='-')) {

		if (!strcmp(argv[optind], "-name")) {
			optind++;
			out_path=argv[optind];
		}
		else if (!strcmp(argv[optind], "-input")){
			optind++;
			path=argv[optind];
		}
		else if (!strcmp(argv[optind], "-match")){
			optind++;
			match = atoi( argv[optind] );
		}
		else if (!strcmp(argv[optind], "-mismatch")){
			optind++;
			mismatch = atoi( argv[optind] );
		}
		else if (!strcmp(argv[optind], "-gap")){
			optind++;
			gap = atoi( argv[optind] );
		}
		else if (!strcmp(argv[optind], "-threads")){
			optind++;
			threads = atoi( argv[optind] );
		}
		else {
			fprintf(stderr, "%s is not a right flag to use!\n" ,argv[optind]);
			printf("Usage: program -name ID -input PATH -match INT1 -mismatch INT2 -gap INT3 -threads INT4.\n");
			fprintf(stderr,"exiting...\n");
			return -1;
		}
		optind++;
	}
	FILE *fp = fopen(path, "r");FILE * f2= fopen(out_path,"a");

	/* Go to the end of the file. */
	if (fseek(fp, 0L, SEEK_END) == 0) {
		/* Get the size of the file. */
		bufsize = ftell(fp);
		if (bufsize == -1) { /* Error */ }

		/* Allocate our buffer to that size. */
		source = malloc(sizeof(char) * (bufsize + 1));

		/* Go back to the start of the file. */
		if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

		/* Read the entire file into memory. */
		size_t newLen = fread(source, sizeof(char), bufsize, fp);
		if ( ferror( fp ) != 0 ) {
			fputs("Error reading file", stderr);
		} else {
		source[newLen++] = '\0'; /* Just to be safe. */
		}
	}


	int b=0;
	while(b<bufsize+1){
		if(((source[b]=='D' || source[b]=='Q') && source[b+1]==':') ||source[b]=='\0'){	
			length[w]=b;
			w++;
		}
		b++;
	}


	/*PARALLEL REGION-INITIALIZATIONS*/
	if(((w-1)/2)<threads) threads=threads-(w-1)/2;

	struct Water st[threads];
	pthread_t pthreads[threads];

	int chunk=(w-1)/threads;
	for(int e=0;e<threads;e++){
	st[e].begin=(e*chunk);
	st[e].end=(e+1)*chunk;
	if(e==threads-1)st[e].end=w-2;
	st[e].f2=f2;
	pthread_create(&pthreads[e],SCHED_OTHER,Waterman,(void*)&st[e]);

	}

	fprintf(stderr, " threads are %d\n",threads );
	for (int i = 0; i < threads; i++){
		pthread_join(pthreads[i],NULL);
	}
	double time1 = gettime();
	fprintf(stderr, "A)Total pairs of Q & D : %d\n",pairs );
	fprintf(stderr, "B)Total number of cells that had been used : %d\n",dim0+dim1 );
	fprintf(stderr, "C)Total number of traceback steps : %d\n",traceback_steps );
	fprintf(stderr, "D)Total runtime : %f\n",time1-time0 );
	fprintf(stderr, "E)Total score calculation time :%f\n",total_score_time );
	fprintf(stderr, "F)Total traceback time : %f\n",total_traceback_time);
	int cups0=(dim0+dim1)/(time1-time0);
	fprintf(stderr, "G)Cell Updates per second (regarding total runtime): %d\n",cups0 );
	int cups1=(dim0+dim1)/total_score_time;
	fprintf(stderr, "H)Cell Updates per second (regarding total score calculation time): %d\n",cups1);
	fprintf(stderr, "=====================================================================================================================================================================\n");

	return 0;
}
