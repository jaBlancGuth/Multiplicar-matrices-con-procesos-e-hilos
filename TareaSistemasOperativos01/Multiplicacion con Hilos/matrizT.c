#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // time()
#include<pthread.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>

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
    return arr;
}

long** intInitializeMatrixC(long **arr,int n){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++)
            arr[i][j] = 0;
    }
    
    return arr;
}

typedef struct arg_struct
{
  long **a;
  long **b;
  long **c;
  int row;
  int mat;
} thr_args;

void multi(void *row_args){
    thr_args *args = (thr_args *) row_args;
    
    for(int i = 0; i < args->mat; i++)
    {
        for(int j=0;j<args->mat;j++){ 
            int multi =  args->a[args->row][j]*args->b[j][i];
            args->c[args->row][i]+= multi;
        }
    }
    pthread_exit(0);  
}

void save_to_file (int n, long **mat, int r){
    char str[50];
    FILE *fp;
    char listLine[10];
    sprintf(listLine,"mat_%d.txt", r);
    fp  = fopen (listLine, "a");
    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%s", "[");
        for (int j = 0; j < n; j++)
        {
            if(j==n-1){
                fprintf(fp, "%li %s %s", mat[i][j],"]","\n");
            }
            else{
                fprintf(fp, "%li %s", mat[i][j],",");
            }
        }
        
    }
    fclose (fp);
}

long multiplicar(int n,int indice){

    long **MatA;
    long **MatB;
    long **MatC;
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    MatA = intCreateMatrix(n);

    srand(time(0));
    MatA = intInitializeMatrix(MatA,n);

    MatB = intCreateMatrix(n);
    
    MatB = intInitializeMatrix(MatB,n);

    MatC=intCreateMatrix(n);
    MatC=intInitializeMatrixC(MatC,n);

    pthread_t *thrs;
    thr_args *args;
    if ((thrs = malloc(sizeof(pthread_t) * (unsigned long) n)) == NULL ||
        (args = malloc(sizeof(thr_args) * (unsigned long) n)) == NULL)
    {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
    {
        args[i] = (thr_args) {
        .a = MatA,
        .b = MatB,
        .c = MatC,
        .row = i,
        .mat= n
        };
        pthread_create(&thrs[i], NULL, (void *) &multi, (void *) &args[i]);
    }
    for (int t = 0; t < n; t++)
        pthread_join(thrs[t], NULL);
    if (thrs != NULL)
    {
        free(thrs);
        thrs = NULL;
    }
    if (args != NULL)
    {
        free(args);
        args = NULL;
    }
    gettimeofday(&stop, NULL);
    save_to_file ( n, MatC, indice);
    free (MatA);
    free (MatB);
    free (MatC);
    long tiempo = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    return tiempo;
}

int main(){
    int n;
    printf("Inserte el n para crear las matrices: ");
    scanf("%d", &n);
    long tiempo[100];
    long promedio=0;    
    FILE *fp = fopen("Stats", "a");
    if (fp == NULL)
    {
        printf("Error opening the file %s", "stats");
        return -1;
    }
    // write to the text file 
    int contador=0;
    
    for(int i=0; i<100;i++){
        
        tiempo[i]=multiplicar(n,i);
        promedio+=tiempo[i]; 
        contador+=1;  
        srand(time(0));
    }
    promedio = promedio / n;
    fprintf(fp, "%s %lu", "Promedio:",promedio);
    fclose(fp);
}