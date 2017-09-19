
#include <stdio.h>
#include <stdlib.h>

// Escreve na tela o valor correspondente a A[i,j]
// idx para variável tipo Matrix
#define idx(a,i,j) a.mat[i][j]
// idxp para variável tipo ponteiro para Matrix
#define idxp(a,i,j) a->mat[i][j]

typedef struct{             // tipo de dados Matrix
    int m, n;               // ordem da matriz
    double **mat;           // valores armazenados
} Matrix;

void allocMatrix(Matrix* mat, int m, int n){
    int i, j;
    //mat = (Matrix*) malloc(sizeof(Matrix));
    mat->m = m, mat->n = n;
    mat->mat = (double**) malloc(m* sizeof(double*));                           // alocando a lista de linhas
    for(i = 0; i < m; i++) mat->mat[i] = (double*) malloc(n*sizeof(double));    // alocando linhas da matriz
    for(i = 0; i < m; i++) for(j = 0; j < n; j++) mat->mat[i][j] = 0.0f;     // zerando valores iniciais
}

void freeMatrix(Matrix* m){
    int i;
    for(i = 0; i < m->m; i++) free(m->mat[i]);
    free(m->mat); //free(m);
}

void printMatrix(Matrix* m){
    int i, j;
    for(i = 0; i < m->m; i++){
        for(j = 0; j < m->n; j++)
            printf("%.2f ", idxp(m, i, j));
        printf("\n");
    }
    printf("\n");
}

Matrix multiplyMatrix(Matrix* a, Matrix* b)
{

    if(a->n == b->m){
        int i, j, k; Matrix res;
        allocMatrix(&res, a->m, b->n);
        for(i = 0; i < a->m; i++)                       // O( (a->m)*(b->n)*(a->n)  )  ou O(mpn)
        for(j = 0; j < b->n; j++)                       // O(   (b->n) * (a->n)     )  ou O(pn)
        for(k = 0; k < a->n; k++)                       // O(         a->n          )  ou O(n)
            idx(res,i,j) += idxp(a,i,k) * idxp(b,k,j);  // O(          1            )
        printMatrix(&res);
        return res;
    }else{
        fprintf(stderr, "Erro: matrizes com dimensões incompatíveis.\n");
        exit(EXIT_FAILURE);
    }
}

Matrix readMatrix(char* filename)
{
    printf("lendo %s...\n", filename);
    int m, n, i, j;
    FILE* file = fopen(filename, "r");
    fscanf(file, "%d %d\n", &m, &n);
    Matrix matrix;
    allocMatrix(&matrix, m, n);
    float tmp;
    for(i = 0; i < matrix.m; i++)
    for(j = 0; j < matrix.n; j++)
    {
        fscanf(file, "%f", &tmp);
        matrix.mat[i][j] = tmp;
    }
    //printMatrix(&matrix);
    return matrix;
}


int main(int argc, char**argv)
{

    char *filename1, *filename2;

    if(argc == 3)
    {
        filename1 = argv[1];
        filename2 = argv[2];
    }
    else
    {
        filename1 = "../inputs/mat1.txt";
        filename2 = "../inputs/mat2.txt";
    }

    Matrix A, B, R;

    A = readMatrix(filename1);
    printMatrix(&A);


    B = readMatrix(filename2);
    printMatrix(&B);


    R = multiplyMatrix(&A, &B);
    printMatrix(&R);

    freeMatrix(&R);
    freeMatrix(&B);
    freeMatrix(&A);

    return 0;
}
