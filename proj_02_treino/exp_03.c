# include <stdio.h>

int main() {

    int contador_de_testes = 0;

    int num_depositos = 0;

    while (1){
        scanf("%d", &num_depositos);

        contador_de_testes = contador_de_testes + 1;

        if (num_depositos == 0){
            break;
        }
            

        int conta_j = 0;
        int conta_z = 0;

        printf("Teste %d\n", contador_de_testes);

        for (int i = 0; i < num_depositos; i++){
            
            int diff;
            int j, z; 

            scanf("%d %d", &j , &z);

            conta_j = conta_j + j;
            conta_z = conta_z + z;

            diff = conta_j - conta_z;

            printf("%d\n", diff);

        };
        printf("\n");
    }

    return 0;
}
