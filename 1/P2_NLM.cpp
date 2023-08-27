
#include <opencv2/opencv.hpp>
#include <iostream>
#include "ImageVec.h"
using namespace cv;
using namespace std;

int main(int argc, char *argv[]){
    ImageVec newImg(argv[1],atoi(argv[4]),atoi(argv[3]),C_GREY);
    
    //CLAHE
    
   Mat noisy_mat(newImg.rows,newImg.cols,CV_8UC1),denoised_mat( newImg.rows,newImg.cols,CV_8UC1);
    vector< vector<unsigned int>> Y_vec =  newImg.readChannel(0);
    for (int i = 0; i <  newImg.rows; i++) {
        for (int j = 0; j <  newImg.cols; j++) {

             noisy_mat.at<uchar>(i, j)=  (Y_vec[i][j]);
             //cout<<Y_mat.at<unsigned int>(i,j)<<endl;
        }
    }
    fastNlMeansDenoising(noisy_mat, denoised_mat, atoi(argv[5]),atoi(argv[6]),atoi(argv[7]));
      vector< vector<unsigned int>> Yprime_vec( newImg.rows, vector<unsigned int> ( newImg.cols));
    for (int i = 0; i <  newImg.rows; i++) {
        for (int j = 0; j <  newImg.cols; j++) {
            Yprime_vec[i][j] = denoised_mat.at<uchar>(i, j);
            

        }
            
    }
    ImageVec writeImg(Yprime_vec);
     writeImg.writeToFile(argv[2]);

}