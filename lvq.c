#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define inputs 2
#define num_of_means 5
#define dataset_length 600
#define EPOCH_MAX 1000
#define h0 0.01

float data[dataset_length][2];
float weights[num_of_means][2];
int clusters[num_of_means][600];
float h = h0;


float distance(float x1, float y1, float x2, float y2)
{
    return sqrt(pow((x2 - x1), 2.0) + pow((y2 - y1), 2.0));
}

void to_file(float centers[][2]) 
{
    FILE *fp1;

    fp1 = fopen("centers.txt", "w");

    for (int i = 0; i < num_of_means; i ++) {
        fprintf(fp1, "%lf,%lf\n", centers[i][0], centers[i][1]);
    }

    fclose(fp1);
}

int winner_neuron(float center[][2], float data[][2], int n) 
{
    float d;
    int winner;
    float min = 1000.0;

    for (int i = 0; i < num_of_means; i ++) {
        d = distance(center[i][0], center[i][1], data[n][0], data[n][1]);

        if (d < min) {
            min = d;
            winner = i;
        }
    }

    return winner;
}

void init_weights(float stream[][2], int n)
{ 
    int i;  // index for elements in stream
   
    for (i = 0; i < num_of_means; i++) {
        weights[i][0] = stream[i][0];
        weights[i][1] = stream[i][1];
    }
  
    srand(time(0)); 
  
    for (; i < n; i++) { 
        int j = rand() % (i+1); 
        //printf("j: %d\n", j);
        if (j < num_of_means) {
            weights[j][0] = stream[i][0];
            weights[j][1] = stream[i][1];
        }
    } 
  
    //printf("Following are k randomly selected items \n"); 

    //for (int i = 0; i < num_of_means; i++)
    //    printf("%lf, %lf\n", weights[i][0], weights[i][1]); 
} 

void update_weights(int winner, float x[], int k) {

    for (int j = 0; j < k; j ++) {
        weights[winner][j] += h * (x[j] - weights[winner][j]);
    }

}


void load_data() 
{
    FILE *fp;
    double x1, x2;
    fp = fopen("clusters.txt", "r");

    for (int i = 0; i < dataset_length; i++) {
        fscanf(fp, "%lf,%lf", &x1, &x2);
        data[i][0] = (float)x1;
        data[i][1] = (float)x2;

        //printf("%lf, %lf\n", data[i][0], data[i][1]);
    }

    fclose(fp);
}

int change(float m1[][2], float m0[][2]) 
{
    float d;
    int count = 0;
    
    for (int i = 0; i <num_of_means; i ++) {
        
        d = distance(m1[i][0], m1[i][1], m0[i][0], m0[i][1]);
        //printf("dist: %lf\n", d);
        
        if (d <= 0.001) {
            count ++;
        }  
    }

    if ( count == num_of_means ) {
        return 0;
    }

    return 1;
}

float error(int clusters[][600], float data[][2], float center[][2]) 
{

    int index;
    float sum1 = 0;
    float sum2;

    for (int i = 0; i < num_of_means; i ++) {
        
        int len = clusters[i][0];
        sum1 = 0;
        
        for (int j = 1; j <= len; j ++) {
            index = clusters[i][j];
            sum1 += distance(center[i][0], center[i][1], data[index][0], data[index][1]);
        }

        sum2 += sum1;
    }

    return sum2;

}

void classify(float data[][2], float center[][2]) 
{
    int temp_cluster;
    int next;
    float dist;    
    float min = 1000.0;

    for (int i = 0; i < dataset_length; i++) {
            
        min = 1000.0;

        for (int j = 0; j < num_of_means; j++) {
            dist = distance(center[j][0], center[j][1], data[i][0], data[i][1]);

            if (dist < min) {
                min = dist;
                temp_cluster = j;
            }
        }

        //cluster[i] = temp_cluster;
        next = clusters[temp_cluster][0] + 1;
        clusters[temp_cluster][next] = i;
        clusters[temp_cluster][0] ++;
    }    
}

int main() 
{
    int winner;
    float min_error = 1000.0;
    float err;
    float weights0[num_of_means][2];
    float min_centers[num_of_means][2];
    int min_k;

    
    load_data();

    for (int k = 0; k < 5; k ++) {
        printf("k = %d\n", k);
        
        init_weights(data, dataset_length);
        printf("centers: ");
        for (int i = 0; i < num_of_means; i ++) {
            printf("*%lf,%lf   ", min_centers[i][0], min_centers[i][1]);
        }
        printf("\n");
        h = h0;
        
        for (int i  = 0; i < num_of_means; i++) {
            weights0[i][0] = weights[i][0];
            weights0[i][1] = weights[i][1];
            //printf("")
        }
        

        for (int epoch = 0; epoch < EPOCH_MAX; epoch ++) {

            for (int i = 0; i < num_of_means; i ++) {
                clusters[i][0] = 0;
            }

            //printf("epoch: %d\t ", epoch);
            for (int n = 0; n < dataset_length; n ++) {
                winner = winner_neuron(weights, data, n);
                update_weights(winner, data[n], inputs);    
            }

            classify(data, weights);
            
            if (change(weights, weights0) == 0) {
                goto exit;
            }

            err = error(clusters, data, weights);
            printf("epoch: %d  error: %lf  centers: ", epoch, err);
            printf("centers: ");
            for (int i = 0; i < num_of_means; i ++) {
                printf("%lf,%lf   ", weights[i][0], weights[i][1]);
            }
            printf("\n");

            for( int i  = 0; i < num_of_means; i++) {
                weights0[i][0] = weights[i][0];
                weights0[i][1] = weights[i][1];
            }

            h = 0.95 * h;
        
        }
    exit:
        //printf("exited: \n");

        if (err < min_error) {
            min_error = err;

            for (int i = 0; i < num_of_means; i ++) {
                min_centers[i][0] = weights[i][0];
                min_centers[i][1] = weights[i][1];
                min_k = k;
            }
        }

    }

    printf("\n");
    printf("k: %d  error: %lf  centers: ", min_k, min_error);
    printf("centers: ");
    for (int i = 0; i < num_of_means; i ++) {
        printf("%lf,%lf   ", min_centers[i][0], min_centers[i][1]);
    }
    printf("\n");

    to_file(min_centers);
    execl("/usr/bin/python3", "python3", "plot.py", "clusters.txt", "centers.txt", NULL);

}