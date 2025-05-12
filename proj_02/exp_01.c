# include <stdio.h>

int main() {
    int numero, frequencia;
    char caractere;

    while (1) { 

        if (scanf(" (%d,%d,%c)", &numero, &frequencia, &caractere) != 3) {
            break; 
        }

        printf("%4d |", numero);

        for (int i = 0; i < frequencia; i++) {
            printf("%c", caractere);
        }

        printf(" %d\n", frequencia);
    }

    

    return 0;
}