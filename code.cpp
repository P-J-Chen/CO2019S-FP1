#include<sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include <bits/stdc++.h>
#include <unordered_set>
#include <string>
#include <vector>
using namespace std;

char *sequences;
int *lengths;
int maxlen;
int unique_count;
int *indexes;
int *outputA;
int *outputA_default;

int countlines(const char *filename)
{
  FILE *fp = fopen(filename,"r");
  int ch=0;
  int lines=0;

  if (fp == NULL)
  return 0;

  while(!feof(fp))
  {
  ch = fgetc(fp);
  if(ch == '\n')
    {
    lines++;
    }
  }
  fclose(fp);
  return lines;
}

void init(const char *sFileName, int count, int maxlen)
{
    char mapped[]={'A','C','G','T'};
    FILE *fp = NULL;
    char hex[1024] = "";
    int each = 0;
    size_t used = 0;
    size_t offset = 0;
    
	
    if ( ( fp = fopen ( sFileName, "r")) == NULL) {
        printf ( "could not open file\n");
        exit(0);
    }
    int i=0;
    int j;
    while ( fgets ( hex, sizeof ( hex), fp)) {
        used = 0;
        j=0;
        while (( sscanf ( hex + used, " /x%x%n", &each, &offset)) == 1){
            sequences[i*maxlen+j++]=(char)mapped[((char)each&0xc0)>>6];
            if(j<lengths[i]){
                sequences[i*maxlen+j++]=(char)mapped[((char)each&0x30)>>4];
            }
            if(j<lengths[i]){
                sequences[i*maxlen+j++]=(char)mapped[((char)each&0x0c)>>2];
            }
            if(j<lengths[i]){
                sequences[i*maxlen+j++]=(char)mapped[((char)each&0x03)];
            }
            if(j==lengths[i])
                sequences[i*maxlen+j]='\0';
            used += offset;
        }
        indexes[i]=i;
        i++;
    }
    fclose(fp);
    
    /*for(i=0;i<count;i++){
        for(j=0;j<lengths[i];j++)
            printf("%c",sequences[i*maxlen+j]);
        printf(" %d\n",i);
    }*/
}

/*Implement this function*/
int distinctStringSequential(){

}

/*Implement this function*/
void losSequential(){

}

void max_overlap_index(int current_idx, int count, int maxlen){
    int shift_start,shift_end,smaller,l;
    if(indexes[current_idx]==-1){
        outputA_default[current_idx]=-1;
        return;
    }
    int max_overlap=0;
    for(int i=0;i<current_idx;i++){
        shift_start=lengths[current_idx]>=lengths[i]?lengths[current_idx]-lengths[i]:0;
        shift_end=lengths[current_idx];
        smaller=lengths[current_idx]<=lengths[i]?lengths[current_idx]:lengths[i];
        
        for(int k=shift_start;k<shift_end;k++){
            l=0;
            while(sequences[current_idx*maxlen+k+l]==sequences[i*maxlen+l] && l<smaller)
                l++;
            if(l==shift_end-k){
                if(max_overlap<l){
                    outputA_default[current_idx]=i;
                    max_overlap=l;
                }
            }
        }
    }
    for(int i=current_idx+1;i<count;i++){
        shift_start=lengths[current_idx]>=lengths[i]?lengths[current_idx]-lengths[i]:0;
        shift_end=lengths[current_idx];
        smaller=lengths[current_idx]<=lengths[i]?lengths[current_idx]:lengths[i];
        
        for(int k=shift_start;k<shift_end;k++){
            l=0;
            while(sequences[current_idx*maxlen+k+l]==sequences[i*maxlen+l] && l<smaller)
                l++;
            if(l==shift_end-k){
                if(max_overlap<l){
                     outputA_default[current_idx]=i;
                     max_overlap=l;   
                }
            }
        }
    }
}

void defaultCompute(int count, int maxlen){
    int unique_count=0;
    int i,j,k;
    
    for(i=1;i<count;i++){
        for(j=0;j<i;j++){
            if(lengths[i]==lengths[j] && indexes[j]!=-1){
                k=0;
                while(sequences[i*maxlen+k]==sequences[j*maxlen+k] && sequences[i*maxlen+k]!='\0')
                    k++;
                if(k==lengths[i]+1){
                    indexes[i]=-1;
                    indexes[j]=-1;
                }
            }
        }
        for(j=i+1;j<count;j++){
            if(lengths[i]==lengths[j] && indexes[j]!=-1){
                k=0;
                while(sequences[i*maxlen+k]==sequences[j*maxlen+k] && sequences[i*maxlen+k]!='\0')
                    k++;
                if(k==lengths[i]+1){
                    indexes[i]=-1;
                    indexes[j]=-1;
                }
            }
        }
    if(indexes[i]!=-1)
        unique_count++;
    
    }
    unique_count++;
    /*for(i=0;i<count;i++){
        if(indexes[i]==-1){
            for(j=0;j<lengths[i];j++)
                printf("%c",sequences[i*maxlen+j]);
        printf("\n");
        }
    }*/
    //printf("Unique Count: %d\n",unique_count);
    int overlap_count=0;
    for(i=0;i<count;i++){
        max_overlap_index(i, count, maxlen);
        if(outputA_default[i]!=-1){
            overlap_count++;
        }
    }
    //printf("Overlapping count: %d\n",overlap_count);
}

int checker(int count){
    for(int i=0;i<count;i++){
        if(outputA_default[i]!=outputA[i]){
           printf("Incorrect result\n");
           return 0;
        }
    }
    return 1;
}

int main()
{
    int count=countlines("lengths");
    int maxlen=0;
    lengths=(int *)malloc(count*sizeof(int));
    FILE* lengthFile = fopen("lengths", "r");
    int iter = 0;
    int i=0;
    struct timeval  TimeValue_Start;
    struct timeval  TimeValue_Final;
    struct timezone TimeZone_Start;
    struct timezone TimeZone_Final;
    long time_start, time_end;
    double time_overhead_default, time_overhead_student;
    
    for(i=0; fscanf(lengthFile, "%d", &iter) && !feof(lengthFile);i++){
        lengths[i]=iter;
        if(maxlen<iter)
            maxlen=iter;
    }
    fclose(lengthFile);
    sequences=(char *)calloc((maxlen+1)*count,sizeof(char));
    indexes=(int *)calloc(count,sizeof(int));
    outputA=(int*)malloc(count*sizeof(int));
    outputA_default=(int*)malloc(count*sizeof(int));
    for(i=0;i<count;i++){
        outputA[i]=-1;
        outputA_default[i]=-1;
    }
    
    init("data",count,maxlen);
    
    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    defaultCompute(count, maxlen);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead_default = (time_end - time_start)/1000.0;
	
    
    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    distinctStringSequential();
    losSequential();
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead_student = (time_end - time_start)/1000.0;
	
    if(checker(count)==1)
        printf("Speedup:%lf\n",time_overhead_default/time_overhead_student);
    free(sequences);
    free(lengths);
    free(indexes);
    free(outputA);
    free(outputA_default);
    return 0;
}
