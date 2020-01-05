#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define N 100


int main() {

srand(time(0));

    FILE *fp1;

    fp1 = fopen("clusters.txt", "w");

    if (fp1 == NULL) {
        printf("Null file pointer\n");    
        exit(0);
    }

    for (int i = 0; i < N; i++) {     

        double x1 = ((double)rand())/(double)(RAND_MAX) - 0.5;
        double x2 = ((double)rand())/(double)(RAND_MAX) - 0.5;
        
        double circle = pow(x1, 2.0) + pow(x2, 2.0);

        if (circle <= pow(0.3, 2.0)) {
            fprintf(fp1, "%.6f,%.6f\n", x1, x2);
        }
    }

    for (int i = 0; i < N; i++) {     

        double x1 = ((double)rand())/(double)(RAND_MAX/0.6) - 1.1;
        double x2 = ((double)rand())/(double)(RAND_MAX/0.6) + 0.5;
        
        fprintf(fp1, "%.6f,%.6f\n", x1, x2);
    }

    for (int i = 0; i < N; i++) {     

        double x1 = ((double)rand())/(double)(RAND_MAX/0.6) - 1.1;
        double x2 = ((double)rand())/(double)(RAND_MAX/0.6) - 1.1;
        
        fprintf(fp1, "%.6f,%.6f\n", x1, x2);
    }

    for (int i = 0; i < N; i++) {     

        double x1 = ((double)rand())/(double)(RAND_MAX/0.6) + 0.5;
        double x2 = ((double)rand())/(double)(RAND_MAX/0.6) - 1.1;
        
        fprintf(fp1, "%.6f,%.6f\n", x1, x2);
    }

    for (int i = 0; i < N; i++) {     

        double x1 = ((double)rand())/(double)(RAND_MAX/0.6) + 0.5;
        double x2 = ((double)rand())/(double)(RAND_MAX/0.6) + 0.5;
        
        fprintf(fp1, "%.6f,%.6f\n", x1, x2);
    }

    for (int i = 0; i < N; i++) {     

        double x1 = ((double)rand())/(double)(RAND_MAX/2) - 1.0;
        double x2 = ((double)rand())/(double)(RAND_MAX/2) - 1.0;
        
        fprintf(fp1, "%.6f,%.6f\n", x1, x2);
    }

}

