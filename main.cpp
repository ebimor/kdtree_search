#include <iostream>
#include "kdtree.h"
#include "utility.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main(int argc,char* argv[])
{
    vector<vector<double>> dataset;
    timer t;

    int WIDTH, HEIGHT;
    cout<<"Enter WIDTH of your the 2D space"<<endl;
    cin>>WIDTH;

    cout<<"Enter HEIGHT of your the 2D space"<<endl;
    cin>>HEIGHT;

    int D;
    cout<<"Enter number of samples to take from your space"<<endl;
    cin>>D;

    for(int i = 0; i < D; i++){
        dataset.push_back({generate_random(0, WIDTH), generate_random(0, HEIGHT), generate_random(0.0, 2*M_PI)});
    }
    printf("%d points with dimension (x, y, theta) were generated \n", D);

    double cx, cy, theta;

    cout<<"Enter X coordinate of your point"<<endl;
    cin>>cx;

    cout<<"Enter Y coordinate of your point"<<endl;
    cin>>cy;

    cout<<"Enter Theta value of your point"<<endl;
    cin>>theta;
    vector<double> point = {cx, cy, theta};

    int N;
    cout<<"One more question, how many nearest neighbor should we find?"<<endl;
    cin>>N;



    Kdtree::KdtreePtr tree = make_shared<Kdtree>(Kdtree(dataset));

    vector<vector<double>> res, res_b;
    t.tic();
    res = tree->searchNNearest(point, N);
    double execution_time = t.toc();
    printf("Nearest neighbor search with Kdtree took %f seconds\n", execution_time);

    sort(res.begin(), res.end(), [&point](const vector<double>& a, vector<double>& b){
        return Kdtree::euclideanDist(a, point)<Kdtree::euclideanDist(b, point);
    });

    for(int i = 0; i < res.size(); i++)
      printf("nearest point %d is: (%f, %f, %f)\n",i, res[i][0],res[i][1],res[i][2]);

    t.tic();
    res_b = Kdtree::bruteForceNNearest(dataset, point, N);
    execution_time = t.toc();
    printf("Bruteforce nearest neighbor search method took %f seconds\n", execution_time);

    for(int i = 0; i < N; i++)
      printf("bruteforce nearest point %d is: (%f, %f, %f)\n",i, res_b[i][0],res_b[i][1],res_b[i][2]);

    bool equal = true;
    for(int i = 0; i < N; i++){
      for(int j = 0; j < 3; j++){
        if(res_b[i][j]!=res[i][j]){
          equal = false;
        }
      }
    }
    if(!equal)
      printf("ERROR: Outputs are not equal!!!!\n");
    else
      printf("Bruteforce and K-d tree search produce the same result!!\n");

    export_to_image(dataset, point, res);

    return 0;
}

//
// const double WIDTH = 1000;
// const double HEIGHT = 1000;
//
// int main(){
//
//         vector<vector<double> > dataset;
//         timer t;
//
//         for(int i = 0; i < 10000; i++) {
//                 dataset.push_back({generate_random(0, WIDTH), generate_random(0, HEIGHT), generate_random(0.0, 2*M_PI)});
//         }
//
//         Kdtree::KdtreePtr tree = make_shared<Kdtree>(Kdtree(dataset));
//         vector<double> point = {250.0, 320.0, 1.28};
//
//         vector<vector<double> > res, res_b;
//         int N = 10;
//         t.tic();
//         res = tree->searchNNearest(point, N);
//         double execution_time = t.toc();
//         cout<<"Nearest neighbor search with Kdtree took "<<execution_time<<" seconds"<<endl;
//
//         sort(res.begin(), res.end(), [&point](const vector<double>& a, vector<double>& b){
//                 return euclideanDist(a, point)<euclideanDist(b, point);
//         });
//
//         for(int i = 0; i < res.size(); i++)
//                 cout<<"nearest point "<<i<<" is: "<<res[i][0]<<" "<<res[i][1]<<" "<<res[i][2]<<endl;
//
//         t.tic();
//         res_b = bruteForceNNearest(dataset, point, N);
//         execution_time = t.toc();
//         cout<<"Bruteforce nearest neighbor search method took "<<execution_time<<" seconds"<<endl;
//
//         for(int i = 0; i < N; i++)
//                 cout<<"bruteforce nearest point "<<i<<" is: "<<res_b[i][0]<<" "<<res_b[i][1]<<" "<<res_b[i][2]<<endl;
//
//         bool equal = true;
//         for(int i = 0; i < N; i++) {
//                 for(int j = 0; j < 3; j++) {
//                         if(res_b[i][j]!=res[i][j]) {
//                                 equal = false;
//                         }
//                 }
//         }
//         if(!equal)
//                 printf("Outputs are not equal!!!!\n");
//         else
//                 printf("Check!!\n");
//
//         export_to_image(dataset, point, res);
//
//         return 0;
// }
