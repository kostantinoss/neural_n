#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define EXIT 0
#define TRAINING 1
#define CLASSIFICATION 2
#define ACTIVATION_H2 0         // 0: logistic, 1: tanh
#define H1 10                   // hidden layer 1
#define H2 10                   // hidden layer 2
#define IL 2                    // input layer
#define OL 3                    // output layer
#define dataset_length 3000
#define EPOCH_MAX 10000
#define batch_size 1
#define L (dataset_length/batch_size)   // Number of mini batches
#define h 0.01                         // learning rate


float **weights1;
float **weights2;
float **weights3;
float e[OL];           
float delta3[OL];      
float delta2[H2];
float delta1[H1];
float **expected;
float **expected_test;

float y_1[H1];
float y_2[H2];
float u1[H1];
float u2[H2];
float uout[OL];
float yout[OL];

float bias1[H1];
float bias2[H2];
float bias3[OL];

float input_data[dataset_length][2];
float test_data[dataset_length][2];
int expected_category_test[6000];

int correct = 0;
int false_p = 0;
int batch_start;

float class[OL][OL] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};


float set_random(float a, float b) {
    return ((float)rand())/(float)(RAND_MAX/abs(a - b)) + a;
}

float sigmoid_logistic(float x) {
    if(1 + exp((float) - x) <= 0){
        printf("Lllllllllllllllllllllll\n");
        exit(0);
    }
    return 1 / (1 + exp((float) - x));
}

int prediction(float *out, int len) {
    int pos = 0;
    float max = -1.0;
    for (int i = 0; i < len; i++) {
        if (out[i] > max) {
            max = out[i];
            pos = i + 1;
        }
    }

    return pos;
}

void update_results(int n) {

    int pred = prediction(yout, OL);
    int expe = 0;

    if (expected[n][0] == 1.0){
        expe = 1;
        //printf("%d",1);
    }
    else if(expected[n][1] == 1.0){
        expe = 2;
        ///printf("%d",2);
    }else if(expected[n][2] == 1.0){
        expe = 3;
        //printf("%d",3);
    }

    if (expe == prediction(yout, OL)){
        correct += 1;
    }
    else{
        false_p += 1;
    }


    
}
void load_data() {
    
    FILE *fp1, *fp2;
    expected = malloc(dataset_length * sizeof(float*));
    expected_test = malloc(dataset_length * sizeof(float*));
    double x1, x2, x3, x4;
    int cat1, cat2;
    
    fp1 = fopen("dataset.txt", "r");
    fp2 = fopen("test_set.txt", "r");
    
    // loading dataset from file to 2D array input_data[x1][x2]
    // and 1D vector category category[C1 or C2 or C3]
    for (int i = 0; i < dataset_length; i++) {
        fscanf(fp1, "%lf,%lf,%d", &x1, &x2, &cat1);
        input_data[i][0] = (float)x1;
        input_data[i][1] = (float)x2;
        
        switch (cat1)
        {
            case 1:
                expected[i] = class[(int)cat1 - 1];
                break;
            case 2:
                expected[i] = class[(int)cat1 - 1];
                break;
            case 3:
                expected[i] = class[(int)cat1 - 1];
                break;
        }

        fscanf(fp2, "%lf,%lf,%d", &x3, &x4, &cat2);
        test_data[i][0] = (float)x3;
        test_data[i][1] = (float)x4;
        
        switch (cat2)
        {
            case 1: 
                expected_test[i] = class[(int)cat2 - 1];
                break;
            case 2: 
                expected_test[i] = class[(int)cat2 - 1];
                break;
            case 3:
                expected_test[i] = class[(int)cat2 - 1];
                break;
        }
    }

    fclose(fp1);
    fclose(fp2);
}

void init() {

    //e = (float*)malloc(OL * sizeof(float));
    //delta1 = (float*)malloc(H1 * sizeof(float));
    //delta2 = (float*)malloc(H2 * sizeof(float));
    //delta3 = (float*)malloc(OL * sizeof(float));

    memset(delta1, 0, H1 * sizeof(float));
    memset(delta2, 0, H2 * sizeof(float));
    memset(delta3, 0, OL * sizeof(float));

    // initialization of weights between input and 1st hidden leyer
    weights1 = malloc(H1 * sizeof(float*));
    for (int j = 0; j < H1; j++) {
        bias1[j] = set_random(0.0, 1.0);
        weights1[j] = malloc(IL * sizeof(float));
        for (int i = 0; i < IL; i++) {
            weights1[j][i] = set_random(0.0, 1.0);
        } 
    }

    // init of weights between 1st and 2nd hidden layers
    weights2 = malloc(H2 * sizeof(float*));
    
    for (int j = 0; j < H2; j++) {      
        bias2[j] = set_random(0.0, 1.0);   
        weights2[j] = malloc(H1 * sizeof(float));   
        for (int i = 0; i < H1; i++) {
            weights2[j][i] = set_random(0.0, 1.0);
        } 
    }

    // init of weights between 2nd hidden layer and output layer
    weights3 = malloc(OL * sizeof(float*));
    
    for (int j = 0; j < OL; j++) {
        bias3[j] = set_random(0.0, 1.0);
        weights3[j] = malloc(H2 * sizeof(float));
        for (int i = 0; i < H2; i++) {
            weights3[j][i] = set_random(0.0, 1.0);
        } 
    }

}

void forward_propagation(float *input) {

    float max = 0.0;
    int pos = 0;

    // computation of input and aoutput of the 1st hidden layer
    for (int j = 0; j < H1; j++) {
        u1[j] = bias1[j];
        for (int i = 0; i < IL; i++) {
            u1[j] += weights1[j][i] * input[i];
            
        }
        
        //printf("\n\n");
        y_1[j] = sigmoid_logistic(u1[j]);
        
    }

    // computation of input and aoutput of the 2nd hidden layer
    for (int j = 0; j < H2; j++) {
        u2[j] = bias2[j];
        for (int i = 0; i < H1; i++) {
            u2[j] += weights2[j][i] * y_1[i];
        }

        switch (ACTIVATION_H2)
        {
        case 0:
            y_2[j] = sigmoid_logistic(u2[j]);          
            break;
        
        case 1:
            y_2[j] = tanh(u2[j]);
            break;
        }
        
    }


    // computation of input and output of the output layer
    for (int j = 0; j < OL; j++) {
        uout[j] = bias3[j];
        for (int i = 0; i < H2; i++) {
            uout[j] += weights3[j][i] * y_2[i];
        }
        //printf("+ %lf\n",uout[j]);
        
        yout[j] = sigmoid_logistic(uout[j]);
        //printf("--> %lf\n", output[j]);
        //printf("%lf\t%lf\n", expected[n][], predicted[j]);
    }
}

void backpropagation(float *expected, float *predicted, int n) {

    float derivative = 0;
    float sum = 0;
    //printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // prediction error
    for (int j = 0; j < OL; j++) {
        e[j] = expected[j] - predicted[j];
        //printf("%lf\t%lf\t%lf\n", expected[j], predicted[j], e[j]);
    }
   // printf("\n");

    // output layer nodes...
    for (int j = 0; j < OL; j++) {
        derivative = sigmoid_logistic(uout[j]) * (1 - sigmoid_logistic(uout[j]));
        delta3[j] =  e[j] * derivative;
        //printf("delta3 = %lf\n", delta3[j]);
    }

    // hidden layer 2 nodes...
    for (int j = 0; j < H2; j++) {
        sum = 0;

        switch (ACTIVATION_H2)
        {
        case 0:   
            derivative = sigmoid_logistic(u2[j]) * (1 - sigmoid_logistic(u2[j]));      
            break;
        
        case 1:
            derivative = 1 - pow(tanh(u2[j]), 2.0);      
            break;
        }
        
        for (int k = 0; k < OL; k++) {
            sum += delta3[k] * weights3[k][j];
        }
        delta2[j] = derivative * sum;
       // printf("delta2 = %lf\n", delta2[j]);
    }
    
    // hidden layer 1
    for (int j = 0; j < H1; j++) {
        sum = 0;
        derivative = sigmoid_logistic(u1[j]) * (1 - sigmoid_logistic(u1[j]));
        for (int k = 0; k < H2; k++) {
            sum += delta2[k] * weights2[k][j];
        }
        delta1[j] = derivative * sum;
        //printf("delta1 = %lf\n", delta1[j]);
    }
    
}

void update_weights(int n) {

    //printf("updating wights /n");
    // update weights and biases
    // 3rd layer
    for (int j = 0; j < OL; j++) {
        for (int i = 0; i < H2; i++){
            weights3[j][i] += h * delta3[j] * y_2[i];
            
        }
        bias3[j] += h * delta3[j];
    }

    // 2nd layer
    for (int j = 0; j < H2; j++) {
        for (int i = 0; i < H1; i++){
            weights2[j][i] += h * delta2[j] * y_1[i];
            
        }
        bias2[j] += h * delta2[j];
    }

    // 1st layer
    for (int j = 0; j < H1; j++) {
        for (int i = 0; i < IL; i++){
            weights1[j][i] += h * delta1[j] * input_data[n][i];
            
        }
        bias1[j] += h * delta1[j];
    }
}

int main() {

    float sum1, sum2;
    float mean_err0 = 0;
    float mean_err1 = 0;
    char yes;
    int job = 1;
    
    srand(time(0));

    load_data();
    //exit(0);
    init();

    /*
    //while (1){
        printf(" # 0 : EXIT...\n");
        printf(" # 1 : Start training...\n");
        printf(" # 2 : Test network...\n");
        printf("=>");
        //scanf("%d", &job);
        
        if (job == EXIT) {
            exit(0);
        }

        if (job == TRAINING) {
            for (int epoch = 0; epoch < EPOCH_MAX; epoch++) {   
                correct = 0;
                false_p = 0;
                sum1 = 0;
                sum2 = 0;  
                printf("------------------------ epoch:%d --------------------- \n", epoch);

                memset(delta1, 0, H1 * sizeof(float));
                memset(delta2, 0, H2 * sizeof(float));
                memset(delta3, 0, OL * sizeof(float));

                int n = 0;
                for (int batch = 0; batch < L; batch++) {
                    for (int i = 0; i < batch_size; i ++) {
                        if (n == dataset_length) {
                            break;
                        }
                        //printf("batch:%d  n = %d  ",batch, n);
                        
                        forward_propagation(input_data[n]);
                        update_results(n);
                        ///* -------------------------------------------- 
                        // innrer loop for computaion of mean square error.
                        for (int j = 0; j < OL; j++) {
                            sum1 += pow(e[j], 2.0);
                            //printf("yout = %lf\n", yout[j]);
                            
                        }
                        //printf("error = %0.6f\n",sum1/(float)3);
                        sum2 += sum1;
                        ///* ---------------------------------------------
                        //print_res();
                        backpropagation(expected[n], yout);
                        //print_pr();
                        n++;
                    }
                    //update_weights(n);
                }
                mean_err1 = (1.0/(float)(2 * dataset_length)) * sum2;
                printf("correct = %d,  false = %d   ", correct, false_p);
                printf("mean error = %lf\n", mean_err1);
            }
            
            //printf("\n");
        }

        if (job == CLASSIFICATION) {
            // todo...
        }
        
    //}
    
    
    */

    
    for (int epoch = 0; epoch < EPOCH_MAX; epoch++) {
        correct = 0;
        false_p = 0;
        sum1 = 0;
        sum2 = 0;
        printf("+ epoch %d: ", epoch);
        for (int n = 0; n < dataset_length; n++) {
            forward_propagation(input_data[n]);
            update_results(n);
            //
            //compute_error(e, expected[expected_category[n] - 1], yout, OL, n);
            backpropagation(expected[n], yout, n);
            update_weights(n);

            // mean error over each epoch 
            for (int j = 0; j < OL; j++) {
                sum1 += pow(e[j], 2.0);
                
            }
            sum2 += sum1;
        }

        mean_err1 = (1.0/(float)(2 * dataset_length)) * sum2;
        printf("correct = %d,  false = %d   ", correct, false_p);
        printf("mean error = %lf, change = %lf\n", mean_err1, mean_err0 - mean_err1);
        
        if (epoch >= 10000) {
            if (mean_err0 - mean_err1 < 0.0001 && mean_err0 - mean_err1 >= 0)
            {
                break;
            }
            
        }
        
        mean_err0 = mean_err1;
        mean_err1 = 0;
        
    }

    free(weights1);
    free(weights2);
    free(weights3);
}