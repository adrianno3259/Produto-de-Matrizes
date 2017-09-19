
#include <stdio.h>
#include <stdlib.h>

#define idx(a,i,j) a.mat[i][j]
#define idxp(a,i,j) a->mat[i][j]

#define MATRIX
#ifdef MATRIX

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
    free(m->mat);
    //free(m);
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
    //printMatrix(a);
    //printMatrix(b);
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
    printMatrix(&matrix);
    return matrix;
}

#endif // MATRIX

#define BLOCK_MATRIX
#ifdef BLOCK_MATRIX

typedef struct{     // tipo de dados Matrix
    int m, n;       // ordem geral da matriz/dimensões do array
    int hp;         // número de partições horizontais
    int vp;         // número de partições verticais
    int* hDims;      // dimensões horizontais das partições
    int* vDims;      // dimensões verticais das partições
    double **mat;   // valores armazenados
} BlockMatrix;

void allocBlockMatrix(BlockMatrix* mat, int m, int n, int vp, int hp, int* vDims, int* hDims){
    int i, j;
    //mat = (Matrix*) malloc(sizeof(Matrix));
    mat->m = m, mat->n = n;
    mat->hp=hp, mat->vp=vp;
    mat->hDims=hDims, mat->vDims=vDims;
    mat->mat = (double**) malloc(m*sizeof(double*));                           // alocando a lista de linhas
    for(i = 0; i < m; i++) mat->mat[i] = (double*) malloc(n*sizeof(double));    // alocando linhas da matriz
    for(i = 0; i < m; i++) for(j = 0; j < n; j++) mat->mat[i][j] = 0.0f;     // zerando valores iniciais
}

void freeBlockMatrix(BlockMatrix* m){
    int i;
    for(i = 0; i < m->m; i++) free(m->mat[i]);
    free(m->mat);
    free(m->hDims);
    free(m->vDims);
}

BlockMatrix readBlockMatrix(char* filename){

    printf("lendo %s...\n", filename);
    int m, n, hp, vp, i, j;
    FILE* file = fopen(filename, "r");
    BlockMatrix matrix;
    fscanf(file, "%d %d\n", &m, &n);

    fscanf(file, "%d\n", &hp);
    printf("hp = %d\nhDims:\n", hp);
    int *hDims = (int*) malloc(hp*sizeof(int)), tmp_dim;
    for(i = 0 ; i < hp; i++){
        fscanf(file, "%d", &tmp_dim);
        printf("%d ", tmp_dim);
        hDims[i] = tmp_dim;
    }
    printf("\n");

    fscanf(file, "%d\n", &vp);
    printf("vp = %d\nvDims:\n", vp);
    int *vDims = (int*) malloc(vp*sizeof(int));
    for(i = 0 ; i < vp; i++){
        fscanf(file, "%d", &tmp_dim);
        printf("%d ", tmp_dim);
        vDims[i] = tmp_dim;
    }
    printf("\n");

    allocBlockMatrix(&matrix, m, n, vp, hp, vDims, hDims);


    float tmp;
    for(i = 0; i < matrix.m; i++)
    for(j = 0; j < matrix.n; j++)
    {
        fscanf(file, "%f", &tmp);
        matrix.mat[i][j] = tmp;
    }
    //printMatrix(&matrix);



    for(i = 0; i < matrix.m; i++){
        for(j = 0; j < matrix.n; j++)
            printf("%.2f ", idx(matrix, i, j));
        printf("\n");
    }
    printf("\n");
    return matrix;

	/*
    free(vDims); free(hDims);
    */

}

Matrix getIdxBlockMatrix(BlockMatrix bm, int linha, int coluna){
	int m, n, i, j, base_i, base_j; Matrix mat;
	m = bm.vDims[linha], n = bm.hDims[coluna], base_i = base_j = 0;
	allocMatrix(&mat, m, n);
	for(i=0; i<linha;  i++) base_i += bm.vDims[i];
	for(j=0; j<coluna; j++) base_j += bm.hDims[j];
	for(i=0; i < m; i++) for(j=0; j < n; j++)
        idx(mat,i,j) = idx(bm,i+base_i,j+base_j);
	return mat;
}


void printBlockMatrix(BlockMatrix* m){
    int i, j;
    for(i = 0; i < m->m; i++){
        for(j = 0; j < m->n; j++)
            printf("%.2f ", idxp(m, i, j));
        printf("\n");
    }
    printf("\n");
}

void setIdxBlockMatrix(BlockMatrix bm, int linha, int coluna, Matrix matrix){
	int m, n, i, j, base_i, base_j;
	m = bm.vDims[linha], n = bm.hDims[coluna];
	if(m == matrix.m && n == matrix.n){
        base_i = base_j = 0;
        for(i=0; i<linha;  i++) base_i += bm.vDims[i];
        for(j=0; j<coluna; j++) base_j += bm.hDims[j];
        for(i=0; i < m; i++) for(j=0; j < n; j++)
            idx(bm,i+base_i,j+base_j) = idx(matrix,i,j);
	}
	else printf("diferente\n");
}

#endif


int main(int argc, char**argv)
{
    char* filename = "mat1.txt", *filename2 = "mat2.txt";

    BlockMatrix bm = readBlockMatrix("block1.txt");

	Matrix m = getIdxBlockMatrix(bm, 0, 0);
	printMatrix(&m);
    freeMatrix(&m);

	m = getIdxBlockMatrix(bm, 0, 1);
	printMatrix(&m);
    freeMatrix(&m);

	m = getIdxBlockMatrix(bm, 1, 0);
	printMatrix(&m);
    freeMatrix(&m);

	m = getIdxBlockMatrix(bm, 1, 1);
	printMatrix(&m);
	freeMatrix(&m);

	allocMatrix(&m, 2, 2);
	printMatrix(&m);

	printBlockMatrix(&bm);
    setIdxBlockMatrix(bm, 0, 0, m);
    printBlockMatrix(&bm);
    setIdxBlockMatrix(bm, 1, 0, m);
    printBlockMatrix(&bm);
    freeMatrix(&m);

	freeBlockMatrix(&bm);

    /*
    Matrix A, B, R;

    A = readMatrix(filename);
    printMatrix(&A);


    B = readMatrix(filename2);
    printMatrix(&B);


    R = multiplyMatrix(&A, &B);
    printMatrix(&R);

    freeMatrix(&R);
    freeMatrix(&B);
    freeMatrix(&A);*/
    return 0;
}
