#include <stdio.h>
#include <stdlib.h>
#include <omp.h>



int **allocate_2d_Matrix(int r, int m);
void printMat(int **mat, int r, int c);

#define CHUNKSIZE 1
#define THREADS 4
int main(int argc, char *argv[])
{
    int r1, c1;
    int input;

    printf("enter number of rows and columns of the matrices : ");
    scanf("%d %d", &r1, &c1);

    int **mat1, **mat2, **result;
    mat1 = allocate_2d_Matrix(r1, c1);
    mat2 = allocate_2d_Matrix(r1, c1);
    result = allocate_2d_Matrix(r1, c1);
    
    printf("\nenter the elements of the first matrix : ");
    int i ,j;
    for(i = 0; i < r1; i++){
        for(j = 0; j < c1; j++){
            scanf("%d", &input);
            mat1[i][j] = input;
        }
    }
    printf("\nenter the elements of the second matrix : ");

    for(i = 0; i < r1; i++){
        for(j = 0; j < c1; j++){
            scanf("%d", &input);
            mat2[i][j] = input;
        }
    }

    for(i = 0; i < r1; i++)
    {
        for(j = 0; j < c1; j++)
        {
            result[i][j] = 0;
        }
    }

    #pragma omp parallel num_threads(THREADS) shared(mat1, mat2, result) private(i, j)
    {
        #pragma omp for schedule(static, CHUNKSIZE)
        for(i = 0; i < r1; i++)
        {
            for(j = 0; j < c1; j++)
            {
                result[i][j] = mat1[i][j] + mat2[i][j];
                printf ("IN thread %d result[%d][%d] = %d\n", omp_get_thread_num(), i, j, result[i][j]);
            }
        }
    }

    printMat(result, r1, c1);

}


int **allocate_2d_Matrix(int m, int n)
{
    int **mat, *linear;
    
    linear = malloc (sizeof (int) * m * n);
    mat = malloc (sizeof (int *) * m);

    int i;
    for(i = 0; i < m; i++){
        mat[i] = &linear[i*n];
    }
    return mat;
}

void printMat(int **mat, int r, int c){
    int i;
    for(i = 0; i < r; i++){
        int j;
        for(j = 0; j < c; j++){
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}