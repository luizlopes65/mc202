# include <stdio.h>

int main(){

    int contador_de_testes = 0;

    int num_meteoritos = 0;

    int x1, y1, x2, y2;

    while (1){
        
        scanf("%d %d %d %d", &x1 , &y1 , &x2 , &y2);

        contador_de_testes = contador_de_testes + 1;

        if (x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0)
            break;

        scanf("%d", &num_meteoritos);

        int contador_de_meteteoritos = 0;

        for (int i = 0; i < num_meteoritos; i++){
            int xm, ym; 

            scanf("%d %d", &xm , &ym);

            if (xm >= x1 && xm <= x2 && ym >= y2 && ym <= y1) {
                contador_de_meteteoritos = contador_de_meteteoritos + 1;
                };
        };

        printf("Teste %d\n", contador_de_testes);
        printf("%d\n", contador_de_meteteoritos);
        printf("\n");

    }

    return 0;
}