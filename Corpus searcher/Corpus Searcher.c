#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mpi.h"


int main(int argc, char *argv[])
{
    char lines[1500];
    char query[100];
    char cpyStr[100];
    char * queryArr[100];
    char * subQueryArr[100];
    int querySize = 0;
    FILE * file1, * file2;
    char fileName[100];
    int parserSize;

    int isend[50];
    int rank, size;
    int num_of_elements = 0;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    num_of_elements = 50 / size;
    int reminder = num_of_elements * size;
    int irecv[num_of_elements];

    if(rank == 0)
    {
        int m;
        for(m = 0; m < 50; m++)
        {
            isend[m] = m + 1;
        }
        printf("enter query : ");
        gets(query);
        char * parserQuery = strtok(query, " ");
        parserSize = strlen(parserQuery);
        queryArr[0] = parserQuery;
        int i = 1;
        while (parserQuery != NULL)
        {
            parserQuery = strtok(NULL, " ");
            queryArr[i] = parserQuery;
            parserSize += strlen(parserQuery);
            i++;
        }
        printf("\nsize = %d", parserSize);
        //parserSize = 
        querySize = i - 1;
        printf("\nquery = %s", queryArr[0]);
        printf("\nquery size = %d", querySize);
        printf("\n");
    }

    /*int n;
    for(n = 1; n < size; n++)
    {
        MPI_Send(queryArr, 7, MPI_CHAR, n, 0, MPI_COMM_WORLD);
        MPI_Send(&querySize, 1, MPI_INT, n, 0, MPI_COMM_WORLD);
    }*/
    MPI_Bcast(&querySize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&queryArr, 100, MPI_CHAR, 0, MPI_COMM_WORLD);


    MPI_Scatter(&isend, num_of_elements, MPI_INT, &irecv, num_of_elements, MPI_INT, 0, MPI_COMM_WORLD);

    int i = 0;
    int j = 0;
    char * queryCheck;  
    bool match = true;
    int resultsFound = 0;
    int newResults = 0;
    int m ;
    
    /*for(m = 1; m < size; m++)
    {
        MPI_Recv(querySize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(queryArr, querySize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }*/

    if(rank == 0)
    {
        file1 = fopen("Quick-Lookup-TestCases.txt", "a");
        fputs("Query : ", file1);
        fputs(query, file1);
        fclose(file1);
        if(reminder != 50)
        {
            for(j = reminder + 1; j < 50; j++)
            {
                int m = snprintf(fileName, 50, "Aristo-Mini-Corpus P-%d.txt", j);
                file1 = fopen(fileName, "r");
                file2 = fopen("TestCase-01.txt", "a");
                while(!feof(file1))
                {
                    fgets(lines, 1500, file1);

                    for(i = 0; i < querySize; i++)
                    {
                        queryCheck = strstr(lines, queryArr[i]);
                        if(queryCheck)
                        {
                            match = true;
                        }
                        else
                        {
                            match = false;
                            break;
                        }
                    }
                    if(match == true)
                    {
                        fputs(lines, file2);
                        newResults++;
                    }
                }
                fclose(file1);
                j++;
            }
        }
        int y;
        for(y = 1; y < size; y++)
        {
            MPI_Recv(&resultsFound, 1, MPI_INT, y, 0, MPI_COMM_WORLD, &status);
            newResults += resultsFound;
        }
        char results[100];
        snprintf(results, 100, "%d", newResults);
        file1 = fopen("Quick-Lookup-TestCases.txt", "a");
        fputs("\nsearch results found : ", file1);
        fputs(results, file1);
        fclose(file1);
    }

    printf("\nmy rank = %d", rank);
    /*printf("\nnumber of elements = %d", num_of_elements);
        printf("\n");
        int k;
        for(k = 0; k < num_of_elements; k++)
        {
            printf("recived = %d", irecv[k]);
        }
    */
   printf("\nquery = %s", queryArr[0]);
   printf("\nquery size = %d", querySize);

    for(j = 0; j < num_of_elements; j++)
    {
        int m = snprintf(fileName, 50, "Aristo-Mini-Corpus P-%d.txt", irecv[j]);
        file1 = fopen(fileName, "r");
        file2 = fopen("TestCase-01.txt", "a");
        while(!feof(file1))
        {
            fgets(lines, 1500, file1);

            for(i = 0; i < querySize; i++)
            {
                queryCheck = strstr(lines, queryArr[i]);
                if(queryCheck)
                {
                    match = true;
                }
                else
                {
                    match = false;
                    break;
                }
            }
            if(match == true)
            {
                fputs(lines, file2);
                resultsFound++;
            }
        }
        fclose(file1);
        j++;
    }

    if(rank != 0)
    {
        MPI_Send(&resultsFound, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    fclose(file2);

    MPI_Finalize();
    return 0;
}