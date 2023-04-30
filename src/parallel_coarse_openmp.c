#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

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
	int match, mismatch, gap, optind=1;
	char * path;
	char * out_path;
	double total_traceback_time=0,total_score_time=0;int threads;
	if(argc<13){
		printf("Inadequate/wrong arguments or flags.\n");
		printf("Usage: program -name ID -input PATH -match INT1 -mismatch INT2 -gap INT3 -threads INT4.\n");
		printf("Exiting..\n");
		return 1;
	}   
	while ((optind < argc) && (argv[optind][0]=='-')) {
		if (!strcmp(argv[optind], "-name")){
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
		else if (!strcmp(argv[optind], "-threads")) {
			optind++;
			threads = atoi( argv[optind] );			
		}else{
			fprintf(stderr, "%s is not a right flag to use!\n" ,argv[optind]);
			printf("Usage: program -name ID -input PATH -match INT1 -mismatch INT2 -gap INT3 -threads INT4.\n");
			fprintf(stderr,"exiting...\n");
			return -1;
		}
		optind++;
	}
      
    FILE *fp = fopen(path, "r");FILE * f2= fopen(out_path,"a");
  int seq=0, traceback_steps=0, pairs=0, dim0=0, dim1=0;
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


	int b=0,w;
	int length[5000];

	while(b<bufsize+1){
		if(((source[b]=='D' || source[b]=='Q') && source[b+1]==':') ||source[b]=='\0'){	
			length[w]=b;
			w++;
		}
		b++;
	}



	/*PARALLEL REGION-INITIALAZATIONS*/
	char* seq0,*seq1;
	int q,start0, end0, start1, end1,a,notneeded0,notneeded1,s,u,v,r,c,n,m,score,gap1,gap2,i,temp,j,k,max,min,tmp,l,d,p,start_D,xpos,ypos,f,z;
	int *xans,*yans;
	short int** dp;
	double time5,time6,time7,time8;    
	omp_set_num_threads(threads);
	#pragma omp parallel for shared(w,length,pairs,source,dim0,dim1,traceback_steps,total_score_time,total_traceback_time) private(seq0,seq1,start0,end0,start1,end1,a,notneeded0,notneeded1,s,u,v,r,c,n,m,score,gap1,gap2,dp,i,temp,j,d,p,start_D,xpos,ypos,max,min,l,f,time5,time6,time7,time8,z,tmp) 
	for(q=0; q < w-2 ; q+=2 ){
		seq0=(char*)calloc(((length[q+1]-length[q])-3),1);
		seq1=(char*)calloc(((length[q+2]-length[q+1])-3),1);

		start0=length[q]+2;
		end0=length[q+1];
		start1=length[q+1]+2;
		end1=length[q+2];
		
		a=start0;notneeded0=0;notneeded1=0;s=0;
		for (u = 0; u < end0-start0; u++){
			if((int)source[a]==13 || (int)source[a]==10 || (int)source[a]==9 )notneeded0++;
			else {seq0[s]=source[a];s++;}
			a++;
		}
		
		v=start1;s=0;
		for (u = 0; u < end1-start1; u++)
		{
			if((int)source[v]==13 || (int)source[v]==10 || (int)source[v]==9 )notneeded1++;
			else {seq1[s]=source[v];s++;}
			v++;	
		}

		pairs++;
		//***********************************************************************************************
		//IMPLEMENT ALGORITHM HERE...
	
		n=end0-start0-notneeded0; //sequence1 length
		m=end1-start1-notneeded1; //sequence2 length
		c=m+1;
		r=n+1;

		dim0+=r;dim1+=c;
	

		dp = malloc(r * sizeof( int*));
		
		for (i = 0; i < r; i++) {
		dp[i] = malloc(c * sizeof( int));
		}

		for(i=0;i<c;i++) dp[0][i]=0; 
	
		for(i=0;i<r;i++) dp[i][0]=0;
		time5=gettime();
		max=0,min,tmp=0;
		a=0;if(r>c)min=c;else min=r;
		
		a=0;temp=0;
		for (i = 0; i < r+c-1; i++) {
			k=i;
			if(i<min){tmp++;}
			else if(i>=min && i<maxnumber(c,r,-1) && i<c){temp++;}
			else if(i>=min && i<maxnumber(c,r,-1) && i>=c){a++;}
			else if(i>=maxnumber(c,r,-1)){a++;tmp--;temp++;}
			
			for (j = a; j <tmp+a; j++)
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
					if(dp[j][tmp+a+temp-j-1]>max) 
						max=dp[j][tmp+a+temp-j-1];
				}
			}
		}
		
		time6=gettime();
		total_score_time+=time6-time5;
		f=0;
		
	 	for(i=1;i<r;i++){ //compare each letter of seq1 with seq2
			for(j=1;j<c;j++){
				if(dp[i][j]==max) {
					f++;
					traceback_steps++;

					//TRACEBACK!
					l = n + m; // maximum possible length  
					d = i;
					p = j;
					start_D=j-1;
					
					xpos = l; 
					ypos = l; 
				
					// Final answers for the respective strings 
					xans = malloc((l+1) * sizeof(int));
					yans = malloc((l+1) * sizeof(int));
					if (xans == NULL || yans == NULL ) {
					fprintf(stderr, "malloc failed\n");
					}
					
					time7=gettime();
					while ( d != 0 && p !=0 && d<=n && p<=m)
					{ 	
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
					time8=gettime();
					total_traceback_time+=time8-time7;
					fprintf(f2,"Match %d[Score: %d Start:%d Stop:%d]\n",f,max,start_D,j-1 );
					fprintf(f2,"\n" );
					// fwrite(str1,sizeof(str2), 1,f2 );
					fprintf(f2,"Q:	" );
					for(z=xpos+1;z<=l;z++){
					fwrite(&xans[z],1, 1,f2 );
					}
					fprintf(f2,"\n" );
					fprintf(f2,"D:	" );
					for(z=ypos+1;z<=l;z++){
					fwrite(&yans[z],1, 1,f2 );
					}

					fprintf(f2,"\n" );
					free(xans);
					free(yans);					    
				}
			}
		}	
		for (z = 0; z < r; z++) {
			free(dp[z]);
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
