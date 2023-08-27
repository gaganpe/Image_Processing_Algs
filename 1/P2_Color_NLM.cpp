
#include <iostream>
#include "ImageVec.h"
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;


ImageVec nlm_colour_image(ImageVec noisyImg){
        
   ImageVec OpImg(noisyImg.rows,noisyImg.cols,noisyImg.channels); 
    //CLAHE
for(int i=0;i<3;i++){
   Mat noisy_mat(noisyImg.rows,noisyImg.cols,CV_8UC1),denoised_mat( noisyImg.rows,noisyImg.cols,CV_8UC1);
    vector< vector<unsigned int>> Y_vec =  noisyImg.readChannel(i);
    for (int i = 0; i <  noisyImg.rows; i++) {
        for (int j = 0; j <  noisyImg.cols; j++) {

             noisy_mat.at<uchar>(i, j)=  (Y_vec[i][j]);
             //cout<<Y_mat.at<unsigned int>(i,j)<<endl;
        }
    }
    fastNlMeansDenoising(noisy_mat, denoised_mat, 50,21,7);
      vector< vector<unsigned int>> Yprime_vec( noisyImg.rows, vector<unsigned int> ( noisyImg.cols));
    for (int i = 0; i <  noisyImg.rows; i++) {
        for (int j = 0; j <  noisyImg.cols; j++) {
            Yprime_vec[i][j] = denoised_mat.at<uchar>(i, j);
            

        }
    }
    OpImg.writeChannel(i,Yprime_vec);
}
    
   return OpImg; 

}


int main(int argc, char *argv[]){
/*
    if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw Width Height" << endl;
		return 0;
	}*/
    ImageVec newImg(argv[2],atoi(argv[5]),atoi(argv[4]),C_RGB);
    ImageVec noisyImg(argv[1],atoi(argv[5]),atoi(argv[4]),C_RGB);
    ImageVec firstFilter = noisyImg.median_filter(3);
    ImageVec writeImg_mean_fn = nlm_colour_image(firstFilter);
    writeImg_mean_fn.writeToFile(argv[3]);
    /*vector <vector <vector<int>>> firstdif = newImg.PixelDif(noisyImg);
    writeHist(firstdif,"Pepper_diff.txt");
    newImg.writeHist("Pepper_orig.txt");
    newImg.writeHist("pepper_noisy1.txt");*/
    
    
    return 0;



}
