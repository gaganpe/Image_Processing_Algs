

#ifndef IMGMAT_H
#define IMGMAT_H
#include <stdio.h>
#include <stdlib.h>
#include<algorithm>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <queue>
#include <random>
#include <map>
#include <limits.h>

#define CART(x) rows - (x) -1



using namespace std;
class ImgMat {
  public:
    ImgMat invertMat ();
    ImgMat MatMul (ImgMat second);
    vector<vector<double>> Mat;
    ImgMat(vector<vector<double>> Mat);
    void sixelemextend();
    ImgMat(int rows,int cols);
    void printval();
    ImgMat cv_solve (ImgMat rval);
    int rows,cols;


};


#endif
