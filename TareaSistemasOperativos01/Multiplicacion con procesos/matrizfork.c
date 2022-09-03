#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // time()
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
long** intCreateMatrix(int n){
    long **arr;
    int i,j;
    arr = (long **)malloc(n * sizeof(long *));
    for (i=0; i<n; i++)
        arr[i] = (long *)malloc(n * sizeof(long));
    return arr;

}

long** intInitializeMatrix(long **arr,int n){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++)
            arr[i][j] = rand()%10;
    }
    /*for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            printf("%ld ",arr[i][j]);
        printf("\n");
    }
    printf("\n");*/
    return arr;
    //free (arr);
}



long multiplicar(int NumerodeMatriz,int n){

    long **a;
    long **b;

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    a = intCreateMatrix(n);
   
    a = intInitializeMatrix(a,n);

    b = intCreateMatrix(n);
  
    
    b = intInitializeMatrix(b,n);

   
    const char* str1 = "Mat_";

    char *num;
    char buffer[10];

    if (asprintf(&num, "%d", NumerodeMatriz) == -1) {
        perror("asprintf");
    } else {
        strcat(strcpy(buffer, str1), num);
        free(num);
    }



    strcat(buffer,".txt");
    FILE *fp = fopen(buffer, "a");
    if (fp == NULL)
    {
        printf("Error opening the file %s", buffer);
        return -1;
    }
    // write to the text file 
    fprintf(fp, "%s", "[");

    for (int filaA = 0; filaA < n; filaA++)
    {

        if(fork() == 0){ 
          
            for (int colB = 0; colB < n; colB++)
            {   
               
                long  result=0;
                for(int k=0;k<n;k++){             
                    result+=a[filaA][k]*b[k][colB];    
        
                }
                if(colB==n-1){
                    fprintf(fp, "%li %s %s", result,"]","\n");
                }

                else{
                    fprintf(fp, "%li %s", result,",");
                }

            }
            exit(0);


                }

        else{
            wait(NULL);
        }

           
    }
    fclose(fp);

    gettimeofday(&stop, NULL); 

    free (a);
    free (b);
    return (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
}


int main(){
    int n;
    printf("Inserte el n para crear las matrices: ");
    scanf("%d", &n);
    long tiempo[100];
    long promedio=0;    
    FILE *fp = fopen("stats", "a");
    if (fp == NULL)
    {
        printf("Error opening the file %s", "stats");
        return -1;
    }
    // write to the text file 
    for(int i=0; i<100;i++){
        tiempo[i]=multiplicar(i,n);
        promedio+=tiempo[i];   

    }

    promedio=promedio/n;
 
    fprintf(fp, "%s %lu", "Promedio",promedio);
    fclose(fp);



}
