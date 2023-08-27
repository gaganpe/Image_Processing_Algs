
#include <opencv2/opencv.hpp>
#include <iostream>
#include "ImageVec.h"
using namespace cv;
using namespace std;


vector<vector<double>> mean_Weight_gen(int filter_sz){
    vector<vector<double>> retval;
    for(int i=0;i<filter_sz;i++){
        vector<double> new_vec;
        for(int j=0;j<filter_sz;j++){
            double val = 1/double(filter_sz*filter_sz);
            new_vec.push_back(val);
        }
        retval.push_back(new_vec);
    }
    return retval;

}


int main(int argc, char *argv[]){
    ImageVec newImg(argv[1],atoi(argv[4]),atoi(argv[3]),C_RGB);
    
   Mat noisy_mat(newImg.rows,newImg.cols,CV_8UC3),denoised_mat( newImg.rows,newImg.cols,CV_8UC3),final_Img( newImg.rows,newImg.cols,CV_8UC3);
    
    for (int i = 0; i <  newImg.rows; i++) {
        for (int j = 0; j <  newImg.cols; j++) {
            Vec3b writeval;
            writeval[0] = newImg.Img[i][j][0];
            writeval[1] = newImg.Img[i][j][1];
            writeval[2] = newImg.Img[i][j][2];
             noisy_mat.at<Vec3b>(i, j)= writeval;
             //cout<<Y_mat.at<unsigned int>(i,j)<<endl;
        }
    }
    cvtColor(noisy_mat,denoised_mat, COLOR_RGB2HSV);
    
      vector< vector<unsigned int>> Hval( newImg.rows, vector<unsigned int> ( newImg.cols));
      
    for (int i = 0; i <  newImg.rows; i++) {
        for (int j = 0; j <  newImg.cols; j++) {
            Vec3b readval =  denoised_mat.at<Vec3b>(i, j);

        Hval[i][j] = readval[0];
    
            

        }
        
            
    }
    ImageVec writeImg(Hval);
    ImageVec hueImg = writeImg.conv2d_allch(mean_Weight_gen(atoi(argv[5])));
    for (int i = 0; i <  newImg.rows; i++) {
        for (int j = 0; j <  newImg.cols; j++) {
            Vec3b readval =  denoised_mat.at<Vec3b>(i, j);
            Vec3b writeval;
            writeval[0] = hueImg.Img[i][j][0];
            writeval[1] = readval[1];
            writeval[2] = readval[2];
             denoised_mat.at<Vec3b>(i, j)= writeval;
             
        }
    }
    
    cvtColor(denoised_mat,final_Img, COLOR_HSV2RGB);
 
      vector<vector<vector<unsigned int>>> rgb_Img(newImg.rows, vector<vector<unsigned int>>(newImg.cols, vector<unsigned int>(3)));
    for (int i = 0; i <  newImg.rows; i++) {
        for (int j = 0; j <  newImg.cols; j++) {
            Vec3b readval =  final_Img.at<Vec3b>(i, j);
        rgb_Img[i][j][0] = readval[0];
        rgb_Img[i][j][1] = readval[1];
        rgb_Img[i][j][2] = readval[2];
            

        }
        
            
    }
    ImageVec finalImg(rgb_Img);
     finalImg.writeToFile(argv[2]);

}