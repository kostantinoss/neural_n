#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>


#define num_of_means 5
#define dataset_length 600 
#define EPOCH_MAX 100


float center[num_of_means][2];
float data[dataset_length][2];
int clusterss[num_of_means][600];

int cluster[dataset_length];

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

void selectKItems(float stream[][2], int n, float reservoir[][2], int k) 
{ 
    int i;  // index for elements in stream
   
    for (i = 0; i < k; i++) {
        reservoir[i][0] = stream[i][0];
        reservoir[i][1] = stream[i][1];
    }
  
    srand(time(0)); 
  
    for (; i < n; i++) { 
        int j = rand() % (i+1); 
   
        if (j < k) {
            reservoir[j][0] = stream[i][0];
            reservoir[j][1] = stream[i][1];
        }
    } 
  
    //printf("Following are k randomly selected items \n"); 

    //for (int i = 0; i < k; i++)
     //   printf("%lf, %lf\n", center[i][0], center[i][1]); 
} 

float distance(float x1, float y1, float x2, float y2)
{
    return sqrt(pow((x2 - x1), 2.0) + pow((y2 - y1), 2.0));
}

void to_file() {

    FILE *fp1, *fp2;
    fp2 = fopen("clusters_updated.txt", "w");
    fp1 = fopen("centers.txt", "w");

    for (int i = 0; i < dataset_length; i++) {
        fprintf(fp2, "%lf,%lf,%d\n", data[i][0], data[i][1], cluster[i]);

    }

    for (int i = 0; i < num_of_means; i ++) {
        fprintf(fp1, "%lf,%lf\n", center[i][0], center[i][1]);
    }

    fclose(fp2);
    fclose(fp1);
}

void calculate_centers() {

    int index;
    float sumx, sumy;

    for (int i = 0; i < num_of_means; i++) {
        
        int len = clusterss[i][0];
        sumx = 0.0;
        sumy = 0.0;

        for (int j = 1; j <= len; j ++) {
            index = clusterss[i][j];
            //printf("%d\n", index);
            sumx += data[index][0];
            sumy += data[index][1];
        }

        center[i][0] = (1.0/((float)len)) * sumx;
        center[i][1] = (1.0/((float)len)) * sumy;

        //printf("%lf, %lf\n", center[i][0], center[i][1]);
    }
    
}

void classify() {

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

        cluster[i] = temp_cluster;
        next = clusterss[temp_cluster][0] + 1;
        clusterss[temp_cluster][next] = i;
        clusterss[temp_cluster][0] ++;
    }    
}

int change(float m1[][2], float m0[][2]) {

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

float error(int clusterss[][600]) {

    int index;
    float sum1 = 0;
    float sum2;

    for (int i = 0; i < num_of_means; i ++) {
        
        int len = clusterss[i][0];
        sum1 = 0;
        
        for (int j = 1; j <= len; j ++) {
            index = clusterss[i][j];
            sum1 += distance(center[i][0], center[i][1], data[index][0], data[index][1]);
        }

        sum2 += sum1;
    }

    return sum2;

}

int main() 
{
    float means0[num_of_means][2];
    load_data();
    selectKItems(data, dataset_length, center, num_of_means);
    int status;
    


    for( int i  = 0; i < num_of_means; i++) {
            means0[i][0] = center[i][0];
            means0[i][1] = center[i][1];
    }

    for (int epoch = 0; epoch < EPOCH_MAX; epoch++) {
        
        for (int i = 0; i < num_of_means; i ++) {
            clusterss[i][0] = 0;
        }
        
        classify();
        calculate_centers();

        if (change(center, means0) == 0) {
            break;
        }

        printf("epoch:%d  error:%lf  centers: ", epoch, error(clusterss));
        for (int i = 0; i < num_of_means; i ++) {
            printf("%lf,%lf  ", center[i][0], center[i][1]);
        }
        printf("\n");

        for( int i  = 0; i < num_of_means; i++) {
            means0[i][0] = center[i][0];
            means0[i][1] = center[i][1];
        }

    }

    to_file();

    execl("/usr/bin/python3", "python3", "plot.py", "clusters.txt", "centers.txt", NULL);
}