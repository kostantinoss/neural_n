#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define N 6000


int main() {

    srand(time(0));
    FILE *fp1;
    FILE *fp2;
    
    fp1 = fopen("dataset.txt", "w");
    fp2 = fopen("test_set.txt", "w");

    if (fp1 == NULL || fp2 == NULL) {
        printf("Null file pointer\n");    
        exit(0);
    }

    for (int i = 0; i < N; i++) {     
        if (i < N/2) {

            float x1 = ((float)rand())/(float)(RAND_MAX/4) - 2;
            float x2 = ((float)rand())/(float)(RAND_MAX/4) - 2;
            
            float possibility = ((float)rand())/(float)(RAND_MAX);
        
            if (pow(x1 - 1, 2.0) + pow(x2 - 1, 2.0) <= 0.49 ||
                     pow(x1 + 1, 2.0) + pow(x2 + 1, 2.0) <= 0.49) {               
                if (possibility > 0 && possibility <= 0.1) {
                    fprintf(fp1, "%.4f,%.4f,%s\n", x1, x2, "3");
                    printf("%.4f,%.4f\n", x1, x2);          
                }
                else {
                    fprintf(fp1, "%.4f,%.4f,%s\n", x1, x2, "2");
                }
            } else if (pow(x1 + 1, 2.0) + pow(x2 - 1, 2.0) <= 0.49 ||
                             pow(x1 - 1, 2.0) + pow(x2 + 1, 2.0) <= 0.49) {           
                if (possibility > 0 && possibility <= 0.1) {
                    fprintf(fp1, "%.4f,%.4f,%s\n", x1, x2, "2");          
                }
                else {
                    fprintf(fp1, "%.4f,%.4f,%s\n", x1, x2, "3");
                }
            } else {
                fprintf(fp1, "%.4f,%.4f,%s\n", x1, x2, "1");
            }
        }
        else {

            float x1 = ((float)rand())/(float)(RAND_MAX/4) - 2;
            float x2 = ((float)rand())/(float)(RAND_MAX/4) - 2;
        
            if (pow(x1 - 1, 2.0) + pow(x2 - 1, 2.0) <= 0.49 ||
                    pow(x1 + 1, 2.0) + pow(x2 + 1, 2.0) <= 0.49) {
                fprintf(fp2, "%.4f,%.4f,%s\n", x1, x2, "2");
            } else if (pow(x1 + 1, 2.0) + pow(x2 - 1, 2.0) <= 0.49 ||
                        pow(x1 - 1, 2.0) + pow(x2 + 1, 2.0) <= 0.49) {           
                fprintf(fp2, "%.4f,%.4f,%s\n", x1, x2, "3");
            } else {
                fprintf(fp2, "%.4f,%.4f,%s\n", x1, x2, "1");
            }
        }
    }
}