#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>


double gettime(void){
	struct timeval ttime;
	gettimeofday(&ttime, NULL);
	return ttime.tv_sec+ttime.tv_usec * 0.000001;
}
char* removeblanks(char *str){
	int i=0, j=0;
	while(str[j]){
		if(str[i]!=' ')
			str[j++]=str[i];
		i++;
	}
	str[j]='\0';
	return str;
}
int maxnumber(int a,int b,int c){
	int maxnum=-100;
	if(a>maxnum){ //large value
		maxnum=a;
	}
	if(b>maxnum){
		maxnum=b;
	}
	if(c>maxnum){
		maxnum=c;
	}
	if(maxnum<0){
		maxnum=0;
	}
	return maxnum;
}
// main program
int main(int argc,char *argv[]){ 
	double time0 = gettime();
	int match;
	int mismatch;
	int gap; 
	int optind=1;
	char * path;
	char * out_path;
	double total_traceback_time=0,total_score_time=0;int threads;
	// decode arguments
	/* for(i=0; i<argc; ++i)
	{   printf("Argument %d : %s \n", i, argv[i]);
	}*/
	if(argc<11){
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
		else if (!strcmp(argv[optind], "-input")) {
			optind++;
			path=argv[optind];
		}
		else if (!strcmp(argv[optind], "-match")) {
			optind++;
			match = atoi( argv[optind] );	
		}
		else if (!strcmp(argv[optind], "-mismatch")) {
			optind++;
			mismatch = atoi( argv[optind] );
		}	
		else if (!strcmp(argv[optind], "-gap")) {
			optind++;
			gap = atoi( argv[optind] );
		}
		else{
			fprintf(stderr, "%s is not a right flag to use!\n" ,argv[optind]);
			printf("Usage: program -name ID -input PATH -match INT1 -mismatch INT2 -gap INT3.\n");
			fprintf(stderr,"exiting...\n");
			return -1;
		}
		optind++;
	}

	FILE *fp = fopen(path, "r");FILE * f2= fopen(out_path,"a");
	int seq=0;
	int traceback_steps=0;
	int pairs=0;int dim0=0;int dim1=0;
	char *source = NULL;
	long bufsize;
	char * str1="D: ";
	char * str2="Q: ";
            
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

	int k=0,i=0,w;
	int length[5000];
	while(i<bufsize+1){
		if(((source[i]=='D' || source[i]=='Q') && source[i+1]==':') ||source[i]=='\0'){	
			length[w]=i;
			w++;
		}
		i++;
	}
	#
	for(int q=0; q < w-2 ; q+=2 )
	{
		
		char* seq0=(char*)calloc(((length[q+1]-length[q])-3),1);
		char* seq1=(char*)calloc(((length[q+2]-length[q+1])-3),1);
		int start0=length[q]+2,start1=length[q+1]+2;
		int end0=length[q+1],end1=length[q+2];
		
		int a=start0;int notneeded0=0;int notneeded1=0;int s=0;
		for (int u = 0; u < end0-start0; u++)
		{
			if((int)source[a]==13 || (int)source[a]==10 || (int)source[a]==9 )notneeded0++;
			else {seq0[s]=source[a];s++;}
			a++;
		}
		int v=start1;s=0;
		for (int u = 0; u < end1-start1; u++)
		{
			if((int)source[v]==13 || (int)source[v]==10 || (int)source[v]==9 )notneeded1++;
			else {seq1[s]=source[v];s++;}
			v++;
		}

		/* Increment our line count */
		pairs++;

		//***********************************************************************************************
		//IMPLEMENT ALGORITHM HERE...

		//important initialiazations
		int score;
		int gap1;
		int gap2;
		
		//int** dp=NULL;
		int n=end0-start0-notneeded0; //sequence1 length
		int m=end1-start1-notneeded1; //sequence2 length
		int c=m+1;
		int r=n+1;

		dim0+=r;dim1+=c;
		short int** dp;
		
		dp = malloc(r * sizeof(short int*));
		
		for (int i = 0; i < r; i++) {
			dp[i] = malloc(c * sizeof(int));
		}

		for(int i=0;i<m+1;i++) dp[0][i]=0; 

		for(int i=0;i<n+1;i++) dp[i][0]=0;

		double time5=gettime();
		int max=0,min,tmp=0,l,temp=0;
		a=0;
		if(r>c)min=c;else min=r;
		for (int i = 0; i < r+c-1; i++) {
			k=i;
			if(i<min){tmp++;}
			else if(i>=min && i<maxnumber(c,r,-1) && i<c){temp++;}
			else if(i>=min && i<maxnumber(c,r,-1) && i>=c){a++;}
			else if(i>=maxnumber(c,r,-1)){a++;tmp--;temp++;}
			
			for (int j = a; j <tmp+a; j++)
			{	
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
		total_score_time+=time6-time5;
		int f=0;
		
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
						
					int xpos = l, ypos=l; 
					
					// Final answers for the respective strings 
					int * xans = malloc((l+1) * sizeof(int));
					int * yans = malloc((l+1) * sizeof(int));
					if (xans == NULL || yans == NULL ) {
						fprintf(stderr, "malloc failed\n");
						return(-1);
					}
						
					double time7=gettime();
					while ( d != 0 && p !=0 && d<=n && p<=m){ 	
						if (seq0[d-1]== seq1[p-1]){ 
							xans[xpos--] = (int)seq0[d - 1]; 
							yans[ypos--] = (int)seq1[p- 1]; 
							d--; p--; start_D--;
						} 
						else if (dp[d - 1][p - 1] + mismatch == dp[d][p]){ 
							xans[xpos--] = (int)seq0[d - 1]; 
							yans[ypos--] = (int)seq1[p - 1]; 
							d--; p--;start_D--; 
						} 
						else if (dp[d - 1][p] + gap == dp[d][p]){ 
							xans[xpos--] = (int)seq0[d - 1]; 
							yans[ypos--] = (int)'_'; 
							d--; 
						}
						else{ //if (dp[k][p - 1] + gap == dp[k][p])  
							xans[xpos--] = (int)'_'; 
							yans[ypos--] = (int)seq1[p - 1]; 
							p--; 
						}
					}
					double time8=gettime();
					total_traceback_time+=time8-time7;
					fprintf(f2,"Match %d[Score: %d Start:%d Stop:%d]\n",f,max,start_D,j-1 );
					fprintf(f2,"\n" );
					// fwrite(str1,sizeof(str2), 1,f2 );
					fprintf(f2,"Q:	" );
					for(int i=xpos+1;i<=l;i++){
						fwrite(&xans[i],1, 1,f2 );
					}
					fprintf(f2,"\n" );
					fprintf(f2,"D:	" );
					for(int i=ypos+1;i<=l;i++){
						fwrite(&yans[i],1, 1,f2 );
					}
					fprintf(f2,"\n" );
					free(xans);
					free(yans);
				}	
			}
		}
					

		for (int i = 0; i < r; i++) {
			free(dp[i]);
		}
		free(dp);
		free(seq0);free(seq1);

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
