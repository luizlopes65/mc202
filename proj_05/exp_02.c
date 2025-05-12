#include <stdio.h>
#include <math.h>
#include <float.h>

void media(int num_indices, int lista_de_indices[], int matriz[][3], double *media_r, double *media_g, double *media_b) {
    int soma_r = 0, soma_g = 0, soma_b = 0;

    for (int i = 0; i < num_indices; i++) {
        int idx = lista_de_indices[i];
        soma_r += matriz[idx][0];
        soma_g += matriz[idx][1];
        soma_b += matriz[idx][2];
    }

    *media_r = soma_r / (double)num_indices;
    *media_g = soma_g / (double)num_indices;
    *media_b = soma_b / (double)num_indices;
}


double transferencia(int num_indices, int lista_de_indices[], int roupas[][3]) {
    double media_r, media_g, media_b;
    media(num_indices, lista_de_indices, roupas, &media_r, &media_g, &media_b);

    double soma = 0.0;
    for (int i = 0; i < num_indices; i++) {
        int idx = lista_de_indices[i];
        soma += pow(roupas[idx][0] - media_r, 2) + pow(roupas[idx][1] - media_g, 2) + pow(roupas[idx][2] - media_b, 2);
    }
    return soma;
}

void prod_vetorial(const double a[3], const double b[3], double result[3]) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

double prod_interno(double a[3], double b[3]){
    int resultado = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    return resultado;
}

int main(){
    int num_roupas, num_lote;
    scanf("%d %d", &num_roupas, &num_lote);

    int roupas[num_roupas][3];
    for (int i = 0; i < num_roupas; i++) {
        scanf("%d %d %d", &roupas[i][0], &roupas[i][1], &roupas[i][2]);
    }

    if (num_lote == 1){
        double media_r, media_g, media_b;
        int lista_idxs[num_roupas];
        for (int i = 0; i < num_roupas; i++) {
            lista_idxs[i] = i;
        }
        media(num_roupas, lista_idxs, roupas, &media_r, &media_g, &media_b);
        double transf = 0.0;
        for (int i = 0; i < num_roupas; i++) {
            transf += pow(roupas[i][0] - media_r, 2) + pow(roupas[i][1] - media_g, 2) + pow(roupas[i][2] - media_b, 2);
        }
        printf("%.6f\n", transf);
    } else {
        double menor_transf = DBL_MAX;
        if (num_roupas == 1) menor_transf = 0;

        for (int i = 0; i < num_roupas - 2; i++) {
            for (int j = i + 1; j < num_roupas - 1; j++) {
                for (int k = j + 1; k < num_roupas; k++) {

                    double A[3] = {roupas[i][0], roupas[i][1], roupas[i][2]};
                    double B[3] = {roupas[j][0], roupas[j][1], roupas[j][2]};
                    double C[3] = {roupas[k][0], roupas[k][1], roupas[k][2]};

                    double AB[3] = {B[0] - A[0], B[1] - A[1], B[2] - A[2]};
                    double AC[3] = {C[0] - A[0], C[1] - A[1], C[2] - A[2]};

                    double normal[3];
                    prod_vetorial(AB, AC, normal);

                    int combinacoes[8][3] = {
                                {0, 0, 0},
                                {0, 0, 1},
                                {0, 1, 0},
                                {0, 1, 1},
                                {1, 0, 0},
                                {1, 0, 1},
                                {1, 1, 0},
                                {1, 1, 1}
                            };

                            for (int comb = 0; comb < 8; comb++) {
                                int lado1[num_roupas], lado2[num_roupas];
                                int count1 = 0, count2 = 0;

                                for (int u = 0; u < num_roupas; u++) {
                                    if (u == i || u == j || u == k){
                                        continue;
                                        } 

                                    double P[3] = {roupas[u][0] - A[0], roupas[u][1] - A[1], roupas[u][2] - A[2]};
                                    
                                    double orient = prod_interno(normal, P);

                                    if (orient >= 0)
                                        lado1[count1++] = u;
                                    else
                                        lado2[count2++] = u;
                                }

                                if (combinacoes[comb][0] == 1) {
                                    lado1[count1++] = i;
                                } else {
                                    lado2[count2++] = i;
                                    };
                                if (combinacoes[comb][1] == 1){
                                    lado1[count1++] = j;
                                } else {
                                    lado2[count2++] = j;
                                    }
                                if (combinacoes[comb][2] == 1) {
                                    lado1[count1++] = k;} 
                                    else {
                                    lado2[count2++] = k;
                                    }

                                double transf1 = transferencia(count1, lado1, roupas);
                                double transf2 = transferencia(count2, lado2, roupas);
                                double transf_total = transf1 + transf2;

                                if (transf_total < menor_transf) {
                                    menor_transf = transf_total;
                                }
                            }
                    }
                }
            }
            printf("%.6f\n", menor_transf);
        }
        return 0;
    }

