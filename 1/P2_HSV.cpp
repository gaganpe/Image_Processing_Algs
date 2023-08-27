
#include <opencv2/opencv.hpp>
#include <iostream>
#include "ImageVec.h"
using namespace cv;
using namespace std;

int main(int argc, char *argv[]){
    ImageVec newImg(argv[1],atoi(argv[5]),atoi(argv[4]),C_RGB);
    ImageVec cleanImg(argv[2],atoi(argv[5]),atoi(argv[4]),C_RGB);
    
    //CLAHE
    
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

        Hval[i][j] = readval[0]*2;


        }
        
            
    }
    ImageVec writeImg(Hval);
    ImageVec hueImg = writeImg.conv2d_hue(atoi(argv[6]));
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
    cout<<"Mean PSNR = "<<finalImg.psnr_(cleanImg)<<endl;
     finalImg.writeToFile(argv[3]);

}