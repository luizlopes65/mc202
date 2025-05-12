#include <stdio.h>
#include <math.h>

int main() {
    int num_buracos;
    double x_r, y_r, x_g, y_g, z_g;

    while (1) { 

        scanf("%d", &num_buracos);

        if (num_buracos < 0) 
            break; 

        scanf("%lf %lf %lf %lf %lf", &x_r, &y_r, &x_g, &y_g, &z_g);
        
        int flag = 0;

        for (int i = 0; i < num_buracos; i++) {
            float x_b, y_b;

            scanf("%f %f", &x_b, &y_b);

            float dist_r = sqrt((x_b - x_r) * (x_b - x_r) + (y_b - y_r) * (y_b - y_r));
            float dist_g = sqrt((x_b - x_g) * (x_b - x_g) + (y_b - y_g) * (y_b - y_g) + z_g * z_g);

            if (flag == 0){
                if (2 * dist_r <= dist_g) {
                printf("O rato pode escapar pelo buraco (%.3lf,%.3lf).\n", x_b, y_b);
                flag = 1;
            }
            }
        }

        if (flag == 0) {
            printf("O rato nao pode escapar.\n");
        }
    }

    return 0;
}
