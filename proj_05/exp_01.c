#include <stdio.h>
#include <math.h>

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

double calcular_transferencia_total(int* lista_de_idxs_prim, int* lista_de_idxs_seg, int num_prim, int num_seg, int roupas[][3]) {
    double media_r_prim, media_g_prim, media_b_prim;
    media(num_prim, lista_de_idxs_prim, roupas, &media_r_prim, &media_g_prim, &media_b_prim);

    double transf_prim = 0.0;
    for (int i = 0; i < num_prim; i++) {
        int idx = lista_de_idxs_prim[i];
        transf_prim += pow(roupas[idx][0] - media_r_prim, 2) + pow(roupas[idx][1] - media_g_prim, 2) + pow(roupas[idx][2] - media_b_prim, 2);
    }

    double media_r_seg, media_g_seg, media_b_seg;
    media(num_seg, lista_de_idxs_seg, roupas, &media_r_seg, &media_g_seg, &media_b_seg);

    double transf_seg = 0.0;
    for (int i = 0; i < num_seg; i++) {
        int idx = lista_de_idxs_seg[i];
        transf_seg += pow(roupas[idx][0] - media_r_seg, 2) + pow(roupas[idx][1] - media_g_seg, 2) + pow(roupas[idx][2] - media_b_seg, 2);
    }

    return transf_prim + transf_seg;
}

void copia_array(int destino[], int origem[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        destino[i] = origem[i];
    }
}

void troca(int arr1[], int idx1, int arr2[], int idx2) {
    int temp = arr1[idx1];
    arr1[idx1] = arr2[idx2];
    arr2[idx2] = temp;
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
    } else{
        int pos = 0; 

        int num_part = pow(2 , num_roupas) - 1;

        double vetor_de_transf[num_part];

        for(int max_idx=1; max_idx < num_roupas; max_idx++){

            int lista_de_idxs_prim[max_idx];

            for (int i = 0; i < max_idx; i++) {
                lista_de_idxs_prim[i] = i;
            }

            int lista_de_idxs_seg[num_roupas - max_idx];
            
            for (int j = 0; j < num_roupas - max_idx; j++) {
                lista_de_idxs_seg[j] = max_idx + j;
            }

            double transf = calcular_transferencia_total(
                lista_de_idxs_prim,
                lista_de_idxs_seg,
                max_idx,
                num_roupas - max_idx,
                roupas
            );

            printf("Partição %d:\n  Prim: ", pos);
            for (int i = 0; i < max_idx; i++) printf("%d ", lista_de_idxs_prim[i]);
                printf("\n  Seg: ");
            for (int i = 0; i < num_roupas - max_idx; i++) printf("%d ", lista_de_idxs_seg[i]);
                printf("\n  Transf: %.6f\n", transf);

            vetor_de_transf[pos++] = transf;
            
            for (int k = 0; k < num_roupas - max_idx; k++){
                int lista_de_idxs_alt_prim[max_idx];
                copia_array(lista_de_idxs_alt_prim, lista_de_idxs_prim, max_idx);
                int lista_de_idxs_alt_seg[num_roupas - max_idx];
                copia_array(lista_de_idxs_alt_seg, lista_de_idxs_seg, num_roupas - max_idx);
                troca(lista_de_idxs_alt_prim, max_idx - 1, lista_de_idxs_alt_seg, k);

                double transf_alt = calcular_transferencia_total(
                lista_de_idxs_alt_prim,
                lista_de_idxs_alt_seg,
                max_idx,
                num_roupas - max_idx,
                roupas
                );

                printf("Troca %d:\n  Prim: ", pos);
                for (int i = 0; i < max_idx; i++) printf("%d ", lista_de_idxs_alt_prim[i]);
                    printf("\n  Seg: ");
                for (int i = 0; i < num_roupas - max_idx; i++) printf("%d ", lista_de_idxs_alt_seg[i]);
                    printf("\n  Transf: %.6f\n", transf_alt);

                vetor_de_transf[pos++] = transf_alt;
                } 
            }
            

        double menor = vetor_de_transf[0];
        for (int i = 1; i < pos; i++) {
            if (vetor_de_transf[i] < menor) {
                menor = vetor_de_transf[i];
            }
        }
        printf("%.6f\n", menor);
    }
}