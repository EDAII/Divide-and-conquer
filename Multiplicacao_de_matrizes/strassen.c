#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void strassen(double **a, double **b, double **c, int tam);
void sum(double **a, double **b, double **result, int tam);
void subtract(double **a, double **b, double **result, int tam);
double **allocate_real_matrix(int tam, int random);
double **free_real_matrix(double **v, int tam);

void strassen(double **a, double **b, double **c, int tam){
	// Caso trivial caso a matriz seja 1 X 1
	if(tam == 1){
		c[0][0] = a[0][0] * b[0][0];
		return;
	}

	// Outros casos são tratados aqui
	else{
		int newTam = tam/2;
		double **a11, **a12, **a21, **a22;
		double **b11, **b12, **b21, **b22;
		double **c11, **c12, **c21, **c22;
		double **p1, **p2, **p3, **p4, **p5, **p6, **p7;

		//Alocação da memória
		//Sub matriz A:
		a11 = allocate_real_matrix(newTam, -1); // Use random -1 to make a new random matrix
		a12 = allocate_real_matrix(newTam, -1);
		a21 = allocate_real_matrix(newTam, -1);
		a22 = allocate_real_matrix(newTam, -1);
		//Sub matriz B:
		b11 = allocate_real_matrix(newTam, -1);
		b12 = allocate_real_matrix(newTam, -1);
		b21 = allocate_real_matrix(newTam, -1);
		b22 = allocate_real_matrix(newTam, -1);
		//Sub matriz C:
		c11 = allocate_real_matrix(newTam, -1);
		c12 = allocate_real_matrix(newTam, -1);
		c21 = allocate_real_matrix(newTam, -1);
		c22 = allocate_real_matrix(newTam, -1);
		//Alocação dos auxiliares p-n
		p1 = allocate_real_matrix(newTam, -1);
		p2 = allocate_real_matrix(newTam, -1);
		p3 = allocate_real_matrix(newTam, -1);
		p4 = allocate_real_matrix(newTam, -1);
		p5 = allocate_real_matrix(newTam, -1);
		p6 = allocate_real_matrix(newTam, -1);
		p7 = allocate_real_matrix(newTam, -1);

		double **aResult = allocate_real_matrix(newTam, -1);
		double **bResult = allocate_real_matrix(newTam, -1);

		int i,j;

		// Dividir para conquistar método em 4 sub matrizes
		for(i=0; i<newTam; i++){
			for(j=0; j<newTam; j++){
				a11[i][j] = a[i][j];
				a12[i][j] = a[i][j + newTam];
				a21[i][j] = a[i + newTam][j];
				a22[i][j] = a[i + newTam][j + newTam];

				b11[i][j] = b[i][j];
				b12[i][j] = b[i][j + newTam];
				b21[i][j] = b[i + newTam][j];
				b22[i][j] = b[i + newTam][j + newTam];
			}
		}

		// Calculando p1 à p7:

		sum(a11, a22, aResult, newTam);// a11 + a22
		sum(b11, b22, bResult, newTam);// b11 + b22
		strassen(aResult, bResult, p1, newTam); // p1 = (a11 + a22) * (b11 + b22)

		sum(a21, a22, aResult, newTam); // a21 + a22
		strassen(aResult, b11, p2, newTam); // p2 = (a21 + a22) * b11

		subtract(b12, b22, bResult, newTam); // b12 - b22
		strassen(a11, bResult, p3, newTam); // p3 = a11 * (b12 - b22)

		subtract(b21, b11, bResult, newTam); // b21 - b11
	        strassen(a22, bResult, p4, newTam); // p4 = a22 * (b21 - b11)

		sum(a11, a12, aResult, newTam); // a11 - a12
		strassen(aResult, b22, p5, newTam); // p5 = (a11 + a12) * (b22)

		subtract(a21, a11, aResult, newTam); // a21 - a11
		sum(b11, b12, bResult, newTam); // b11 + b12
		strassen(aResult, bResult, p6, newTam); // p6 = (a21 - a11) * (b11 + b12)

		subtract(a12, a22, aResult, newTam); // a12 - a22
		sum(b21, b22, bResult, newTam); // b21 + b22
		strassen(aResult, bResult, p7, newTam); // p7 = (a12 - a22) * (b21 + b22)

		// Calculando c11, c12, c21, c22:

		sum(p3, p5, c12, newTam); // c12 = p3 + p5
		sum(p2, p4, c21, newTam); // c21 = p2 + p4

		sum(p1, p4, aResult, newTam); // p1 + p4
		sum(aResult, p7, bResult, newTam); // p1 + p4 + p7
		subtract(bResult, p5, c11, newTam); // c11 = (p1 + p4) - (p5 + p7)

		sum(p1, p3, aResult, newTam); // p1 + p3
		sum(aResult, p6, bResult, newTam); // p1 + p3 +p6
		subtract(bResult, p2, c22, newTam); // c22 = (p1 + p3) - (p2 + p6)

		// Jutando os resultados encontrados
		for (i = 0; i < newTam; i++){
			for(j = 0; j < newTam; j++){
				c[i][j] = c11[i][j];
				c[i][j + newTam] = c12[i][j];
				c[i + newTam][j] = c21[i][j];
				c[i + newTam][j + newTam] = c22[i][j];
			}
		}

		// Desalocando vetores da memória (free):
		a11 = free_real_matrix(a11, newTam);
		a12 = free_real_matrix(a12, newTam);
		a21 = free_real_matrix(a21, newTam);
		a22 = free_real_matrix(a22, newTam);

		b11 = free_real_matrix(b11, newTam);
		b12 = free_real_matrix(b12, newTam);
		b21 = free_real_matrix(b21, newTam);
		b22 = free_real_matrix(b22, newTam);

		c11 = free_real_matrix(c11, newTam);
		c12 = free_real_matrix(c12, newTam);
		c21 = free_real_matrix(c21, newTam);
		c22 = free_real_matrix(c22, newTam);

		p1 = free_real_matrix(p1, newTam);
		p2 = free_real_matrix(p2, newTam);
		p3 = free_real_matrix(p3, newTam);
		p4 = free_real_matrix(p4, newTam);
		p5 = free_real_matrix(p5, newTam);
		p6 = free_real_matrix(p6, newTam);
		p7 = free_real_matrix(p7, newTam);
		aResult = free_real_matrix(aResult, newTam);
		bResult = free_real_matrix(bResult, newTam);
	}

}

// Função para somar matrizes
void sum(double **a, double **b, double **result, int tam){
	int i, j;
	for(i = 0; i < tam; i++){
		for(j = 0; j < tam; j++){
			result[i][j] = a[i][j] + b[i][j];
		}
	}
}

// Função de subtrair
void subtract(double **a, double **b, double **result, int tam) {

    int i, j;

    for (i = 0; i < tam; i++) {
        for (j = 0; j < tam; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

// Função para alocar as matrizes
double **allocate_real_matrix(int tam, int random){
	int i, j, n = tam, m = tam;
	double **v, a;

	v = (double **) malloc (n * sizeof(double*));
	if(v == NULL){
		printf("Memoria insuficiente\n");
		return (NULL);
	}

	for(i = 0; i < n; i++){
		v[i] = (double *) malloc (m * sizeof(double));
			if(v[i] == NULL){
				printf("Memoria insuficiente\n");
				free_real_matrix(v,n);
				return (NULL);
			}
			if(random == 0){
				for(j = 0; j < m; j++){
					v[i][j] = 0.0;
				}
			}
			if(random == 2){
				for(j = 0; j < m; j++){
					v[i][j] = rand() % 101;
				}
			}

			else{
				if(random == 1){
					for(j = 0; j < m; j++){
						a = rand();
						v[i][j] = (a - (int)a) * 10;
					}
				}
			}
	}
return(v);
}

// Função para desalocar a matriz
double **free_real_matrix(double **v, int tam) {

    int i;

    if (v == NULL) {
        return (NULL);
    }

    for (i = 0; i < tam; i++) {
        if (v[i]) {
            free(v[i]);
            v[i] = NULL;
        }
    }

    free(v);
    v = NULL;

    return (NULL);
}

int main(int argc, char *argv[]){
	int length = atoi(argv[1]);
	clock_t inicio;
	FILE * time_set = fopen("Time.txt","w+");
	FILE * matrix_A = fopen("A.txt","w+");
	FILE * matrix_B = fopen("B.txt","w+");
	FILE * matrix_C = fopen("C.txt","w+");

	double **matrixA, **matrixB, **matrixC;

	matrixA = allocate_real_matrix(length, 2);
	matrixB = allocate_real_matrix(length, 2);
	matrixC = allocate_real_matrix(length, 0);

	// for(int i = 0; i < length; i++){
	// 	for(int j = 0; j < length; j++){
	// 		matrixA[i][j] = rand() % 101;
	// 		matrixB[i][j] = rand() % 101;
	// 	}
  //
	// }

	inicio = clock();
	strassen(matrixA, matrixB, matrixC, length);
	clock_t final = clock() - inicio;

	fprintf(time_set, "%lf\n", ((double)final/CLOCKS_PER_SEC) );

	int i, j;
	for(i = 0; i < length; i++){
		for(j = 0; j < length; j++){
			fprintf(matrix_A, "%lf ", matrixA[i][j]);
			fprintf(matrix_B, "%lf ", matrixB[i][j]);
			fprintf(matrix_C, "%lf ", matrixC[i][j]);
		}
		fprintf(matrix_A, "\n");
		fprintf(matrix_B, "\n");
		fprintf(matrix_C, "\n");
	}

//Desalocando matrizes
matrixA = free_real_matrix(matrixA, length);
matrixB = free_real_matrix(matrixB, length);
matrixC = free_real_matrix(matrixC, length);

// Fechando arquivos
fclose(time_set);
fclose(matrix_A);
fclose(matrix_B);
fclose(matrix_C);

	return 0;
}
