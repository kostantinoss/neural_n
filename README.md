### Semester project for undergraduate course:

### MYE035 Computational Intelligence
[Department of Computer Science & Engineering, University of Ioannina](https://www.cs.uoi.gr/?lang=en)

## Technologies
  * C
  * GCC-9
  
  
## About the the datasets
Construct datasets for the following problems:

#### S1) _Classification problem_:

First, we habe to generate 6000 random examples ( points (x1, x2) in 2d space) within the rectangle bounded by [-2, 2]x[-2, 2]. The first 3000 points will form the training set and the rest, the test set. Each example of the training set can be classified in one of the following three categories:
  * If (x1 -1)^2 +(x2 -1)^2 <=0.49, then (x1 ,x2 ) is classified in category C2,
  * If (x1 +1)^2 +(x2 +1)^2 <=0.49, then (x1 ,x2 ) is classified in category C2
  * If (x1 +1)^2 +(x2 -1)^2 <=0.49, then (x1 ,x2 ) is classified in category C3,
  * If (x1 -1) 2 +(x2 +1)^2 <=0.49, then (x1 ,x2 ) is classified in category C3,
  * else in C1.

Next, we are going to add some noise only to the training set using the following process: for each example of the set that belongs to C1 or C2 with a probability of 0.1, swap it’s category with C1.

The above procedure is implemented in *generate.c*


#### S2) _Clustering problem (five classes, 600 examples)_:
For the clustering problem, we generate the datapoints that we are going to use (points (x1, x2) in 2d space) as follows:
  1. 100 points bounded by a circle with center (0, 0) and radius 0.3,
  2. 100 points bounded by the rectangle [-1.1, -0.5]x[0.5, 1.1],
  3. 100 points bounded by the rectangle [-1.1, -0.5]x[-0.5, -1.1],
  4. 100 points bounded by the rectangle [0.5, 1.1]x[-1.1, -0.5],
  5. 100 points bounded by the rectangle [0.5, 1.1]x[1.1, 0.5],
  6. 100 points bounded by the rectangle [-1, 1]x[-1, 1]
  
The above is implemented in *generate2.c*

## Algorithms implemented

#### P1) A classification program based on the Multilayer Perceptron (MLP) with two hidden layers of neurons having:
- on the 1st hidden layer → logistic function,
- on the 2nd hidden layer → hyperbolic tangent or relu.
- on the output layer →  logistic function.

The program implements a gradient descent training algorithm and weights updating using batches of L examples (mini-batches) assuming that there are N examples in total. 
The above is implemented in *mlp.c*


#### P2) Clustering program with M centers based on the K-means algorithm.

The position of each center is chosen randomly from the whole set of examples.
The above is implemented in *kmeans.c*

#### P3) Clustering program with M centers based on the LVQ algorithm for clustering.

The learning rate, starting from a fixed value (e.g n = 0.1), is formulated by a declining function (e.g n(t+1) = 0.95).
The above is implemented in *lvq.c*


## Usage

Before runing the mlp or the clustering programs, you have to generate the required datasets. 
This is simply done by compiling (i used gcc) and running the excecutables.
* generate.c generates the training and test set tha mlp.c uses
* generate2.c generates the training and test set tha kmeans.c and lvq.c use

The next step is to compile and run the programs. Simple as that

## TO-DO
- [ ] Organize files
- [ ] Add a Makefile
