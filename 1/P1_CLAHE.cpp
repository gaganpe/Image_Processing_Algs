

#include <opencv2/opencv.hpp>
#include <iostream>
#include "ImageVec.h"

using namespace std;
using namespace cv;
ImageVec HistEqualization_Transfer_Fn(ImageVec newImg){
    vector <unsigned int> Histogram= newImg.HistGen();
    vector <unsigned int> mapping(256,0);
    vector<double> cdf(256,0);
	unsigned int data1=0;
    for(int i=0;i<Histogram.size();i++){
	data1+=Histogram[i];
        cdf[i] = (i>0?cdf[i-1]:0) + (double)Histogram[i]/((newImg.rows)*(newImg.cols));
        mapping[i] = cdf[i]*255;
    }
    ImageVec retval(newImg);
    retval.transfer_Img(mapping);
    return retval;

}

ImageVec HistEqualization_BF(ImageVec newImg){
    vector <unsigned int> Histogram= newImg.HistGen();
    unsigned int total_pixels = newImg.rows * newImg.cols;
    unsigned int pix_per_bucket =round(double(total_pixels)/256),current_bucket = 0;
    vector <vector<unsigned int>> mapping(256);
    vector <unsigned int> remaining_pix(256,pix_per_bucket);
    remaining_pix[255]-= (pix_per_bucket *256 -total_pixels);
	for(int i=0;i<Histogram.size();i++){
        unsigned int pix_required = Histogram[i];
        while(pix_required>0){
            if(remaining_pix[current_bucket]<pix_required){
                pix_required-= remaining_pix[current_bucket];
                mapping[i].push_back(current_bucket);
                mapping[i].push_back(remaining_pix[current_bucket]);
                remaining_pix[current_bucket++] = 0;
                
            }
            else{
                if(pix_required){
                remaining_pix[current_bucket] -= pix_required;
                mapping[i].push_back(current_bucket);
                mapping[i].push_back(pix_required);
                pix_required  = 0;
                }
            }
        }
    }
    ImageVec retval(newImg);
    retval.transfer_Img_BF(mapping);
    return retval;
}

int main(int argc, char *argv[]){
    if (argc < 7){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw Width Height" << endl;
		return 0;
	}

    ImageVec newImg(argv[1],atoi(argv[6]),atoi(argv[5]),C_RGB);
    ImageVec writeImg(newImg);
    //CLAHE
    Ptr<CLAHE> clahe = createCLAHE();
    writeImg.RGBtoYUV();
    
   Mat Y_mat(writeImg.rows,writeImg.cols,CV_8UC1),Yprime_mat( writeImg.rows, writeImg.cols,CV_8UC1);
    vector< vector<unsigned int>> Y_vec =  writeImg.readChannel(0);
    for (int i = 0; i <  writeImg.rows; i++) {
        for (int j = 0; j <  writeImg.cols; j++) {

             Y_mat.at<uchar>(i, j)=  (Y_vec[i][j]);
             //cout<<Y_mat.at<unsigned int>(i,j)<<endl;
        }
    }
    clahe->setClipLimit(4);
    Size gridSize(9, 9); 
    clahe->setTilesGridSize(gridSize);
    clahe->apply(Y_mat, Yprime_mat);
      vector< vector<unsigned int>> Yprime_vec( writeImg.rows, vector<unsigned int> ( writeImg.cols));
    for (int i = 0; i <  writeImg.rows; i++) {
        for (int j = 0; j <  writeImg.cols; j++) {
            Yprime_vec[i][j] = Yprime_mat.at<uchar>(i, j);
        }
    }
     writeImg.writeChannel(0,Yprime_vec);
    
     writeImg.YUVtoRGB();
     writeImg.writeToFile(argv[2]);
    //TF

    ImageVec writeImg_TF(newImg);
    writeImg_TF.RGBtoYUV();
    ImageVec Y_ImgVec(writeImg_TF.readChannel(0));
    ImageVec HistEq_Img = HistEqualization_Transfer_Fn(Y_ImgVec);
    writeImg_TF.writeChannel(0,HistEq_Img.readChannel(0));
    writeImg_TF.YUVtoRGB();
     writeImg_TF.writeToFile(argv[3]);

     //BF
     ImageVec writeImg_BF(newImg);
    writeImg_BF.RGBtoYUV();
    ImageVec Y_ImgVec_BF(writeImg_BF.readChannel(0));
    ImageVec HistEq_BF_Img = HistEqualization_BF(Y_ImgVec_BF);
    writeImg_BF.writeChannel(0,HistEq_BF_Img.readChannel(0));
    writeImg_BF.YUVtoRGB();
     writeImg_BF.writeToFile(argv[4]);
    return 0;



}
