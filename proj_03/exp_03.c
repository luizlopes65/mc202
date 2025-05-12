# include <stdio.h>
# include <math.h>

int main(){

    int n;

    scanf("%d", &n);

    int r = (int)ceil(sqrt(n));

    int V_vetor[n];

    int R_vetor[r];

    for (int i = 0; i < n; i++) {
        scanf("%d", &V_vetor[i]);
    }

    for (int j = 0; j < r; j++) {
        R_vetor[j] = 0;
    }

    for (int i=0; i < n; i++) {
        R_vetor[i / r] += V_vetor[i];
    }

    while (1) { 

        char tipo_op;

        int a, b;

        if (scanf(" %c %d %d", &tipo_op, &a, &b) != 3) {
            break; 
        }

        if (tipo_op == 'a'){
            R_vetor[a / r] = R_vetor[a / r] - V_vetor[a] + b;
            V_vetor[a] = b;
        };
        if (tipo_op == 's'){
            int soma = 0;
            int teste_inicio_bloco = a / r;
            int fim_bloco = b / r;

            if (teste_inicio_bloco == fim_bloco) {
                for (int k = a; k <= b; k++) {
                    soma += V_vetor[k];
                }
            } else {
                for (int k = a; k < (teste_inicio_bloco + 1) * r; k++) {
                    soma += V_vetor[k];
                }
                for (int k = teste_inicio_bloco + 1; k < fim_bloco; k++) {
                    soma += R_vetor[k];
                }
                for (int k = fim_bloco * r; k <= b; k++) {
                    soma += V_vetor[k];
                }
            }

            printf("%d\n", soma);
        }
    }

}
        