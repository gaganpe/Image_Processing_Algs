

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
		ImgMat MatSub(ImgMat second);
		ImgMat dft_predict(ImgMat test_data,vector<int> train_classes,vector<int> test_classes);
		ImgMat pca_predict(ImgMat test_data,vector<int> train_classes,vector<int> test_classes);
		ImgMat select_col_mat(vector<int> indexes);
		void sixelemextend();
		ImgMat(int rows,int cols);
		void printval();
		ImgMat row_mat(int row);
		ImgMat cv_solve (ImgMat rval);
		ImgMat transpose();
		int rows,cols;


};


#endif
