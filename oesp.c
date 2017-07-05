#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define LIM 1001313

void OddEvenSort (int *A, int N, int numThread){
    int exch0, exch1 = 1, trips = 0, i;
    while(exch1){
        exch0 = 0;
        exch1 = 0;
#pragma omp parallel num_threads(numThread)
{
        int temp;
#pragma omp for
        for(i = 0; i < N-1; i += 2){
            if(A[i] > A[i+1]){
                temp = A[i];
                A[i] = A[i+1];
                A[i+1] = temp;
                exch0 = 1;
            }
        }
        if (exch0 || !trips) {
#pragma omp for
            for(i = 1; i < N-1; i += 2){
                if(A[i] > A[i+1]){
                    temp = A[i];
                    A[i] = A[i+1];
                    A[i+1] = temp;
                    exch1 = 1;
                }
            }
        } // i f exch0
} // end p a r a l l e l
        trips = 1;
    }
}

void lerValores(char *url, int *C, int tam){
    FILE *arq;
    int i = 0;
	arq = fopen(url, "r");
	if(arq == NULL)
			printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		while( (fscanf(arq,"%d\n", &C[i]))!=EOF ){
            i++;
            if(i == tam)
                break;
		}
	}
	fclose(arq);
}

char *replace_str(char *str, char *orig, char *rep)
{
  static char buffer[4096];
  char *p;

  if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
    return str;

  strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

  return buffer;
}

void imprimir(int *C, int tam, float temp, int numThread){
    int i;
    FILE *arq;
    char *url;
    char str[10];
    sprintf(str,"%d-%d",tam,numThread);
    url = (char*)malloc(sizeof(char)*100);
    strcpy(url, "oesp-");
    strcat(url, str);
    strcat(url, "elementos-");
    strcat(url, __DATE__);
    strcat(url, "-");
    strcat(url, replace_str(replace_str(__TIME__,":","-"),":","-"));
    strcat(url, ".txt");
    printf("Arquivo de Saida:%s\n",url);
    arq = fopen(url,"r");
    if(arq == NULL)
        arq = fopen(url,"wt");
    else
        arq = fopen(url,"a");
    if(arq == NULL){
        printf("Erro, nao foi possivel criar o arquivo\n");
        exit(1);
    }
    fprintf(arq,"Tempo:%f\n",temp);
    fprintf(arq,"\n-------------------\n");
    fprintf(arq,"Valores:\n");
    fprintf(arq,"-------------------\n");
    for(i=0;i < tam;i++)
        fprintf(arq,"%d\n",C[i]);
    fprintf(arq,"-------------------\n\n");
    fclose(arq);
}

int main(int argc, char *argv[]){
    int tam = atoi(argv[1]);
    char* url = argv[2];
    int numThread = atoi(argv[3]);
	if(tam > LIM || tam <= 2 || argc != 4){
        printf("Erro, parametros invalidos\n");
        exit(1);
	}
	int *C;
	C = (int*) malloc(sizeof(int)* tam);
	if(C == NULL){
        printf("Erro, nao foi possivel criar vetor");
        exit(1);
	}
	lerValores(url,C,tam);
	float tempo;
	tempo = omp_get_wtime();
    OddEvenSort(C,tam, numThread);
    float temp = omp_get_wtime() - tempo;
    imprimir(C,tam,temp,numThread);
    free(C);
	exit(1);
}
