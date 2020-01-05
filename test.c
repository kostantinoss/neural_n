#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

float *array;

float set_random(float a, float b) {

    return ((float)rand())/(float)(RAND_MAX/abs(a - b)) + a;
}

void change(float *a) {
    for (int i = 0; i < 4; i++) {
        a[i] = 0.24;

        printf("%0.6f\n",array[i]);
    }
}

int main() {

    array = (float*)malloc(4 * sizeof(float));
    memset(array, 0, 4 * sizeof(float));

    change(array);
    for (int i = 0; i < 4; i++) {


        printf("%0.6f\n",array[i]);
    }

    free(array);
}