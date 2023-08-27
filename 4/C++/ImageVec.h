

#ifndef IMAGEVEC_H
#define IMAGEVEC_H
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
#include <unordered_set>
#include "ImgMat.h"
#define C_GREY 1
#define C_RGB 3

#define ODD_PAD 2
#define RED_CH 0
#define GREEN_CH 1
#define BLUE_CH 2

#define  PI_VAL 3.14159265


#define LOOP_THROUGH_IMG \
for(int i=0;i<Img.size();i++){ \
    for(int j=0;j<Img[i].size();j++){ \
     for(int k=0;k<Img[i][j].size();k++){

#define LOOP_THROUGH_ROW_COL \
for(int i=0;i<Img.size();i++){ \
    for(int j=0;j<Img[i].size();j++){ 
#define END_LOOP_THROUGH_IMG } } }
#define END_LOOP_THROUGH_ROW_COL } } 



using namespace std;
class ImageVec {
  public:
    ImageVec(string s,int rows,int cols,int channels);
    ImageVec(ImageVec &ConstrIm);
    ImageVec(int rows,int cols,int channels);
    ImageVec(vector<vector<unsigned int>> Img);
    ImageVec(vector<vector<vector<unsigned int>>> Img);
    ImageVec binarize();
    vector<vector <vector <double>>> sobel_filter() ;
    //vector<vector<vector<unsigned int >>>  ImageVec::convolve_2d(vector<vector<double>> filter,vector <int> stride, int padding_type,vector<unsigned int> InitPos);
    void threshold(unsigned int value);
    
    vector<unsigned int> HistGen();
    ImageVec conv2d_allch(vector<vector<double>> mask) ;
    double psnr_(ImageVec cleanImg);
    void writeToFile(string s);
    void transfer_Img(vector <unsigned int> mapping) ;
    ImageVec RGBtoGRAY() ; 
    void RGBtoYUV() ;
    void YUVtoRGB() ;
    ImageVec RGBtoCMY();
    ImageVec CMYtoRGB();
    vector<vector<vector<int>>> PixelDif(ImageVec Image2) ;
    vector<vector<unsigned int>> readChannel(int channel) ;
    void writeChannel(int channel,vector<vector<unsigned int>>channel_data) ;
    ImageVec rev_binarize() ;
    void writeHist(string filename);
    ImageVec bilateral_filter(double sigmac ,double simgas,int mask_sz) ;
    //ImageVec shape_transform(vector<ImgMat> transformMat) ;
//    ImageVec homography_transform(ImgMat transformMat) ;
    ImageVec median_filter(int mask_sz);
    ImageVec fixed_thresh() ;
    ImageVec random_thresh() ;
    ImageVec dither_thresh(vector<vector<double>> thresh_mask) ; 
    ImageVec error_diffu(vector<vector<double>> mask) ;
    vector<vector<vector<unsigned int> > > Img;
    void transfer_Img_BF(vector <vector<unsigned int>> mapping) ;
    unsigned int bilinear_interpolate(double x,double y,int channel) ;
    ImageVec thinning(unordered_set <string> cond_mat,vector<vector<vector<char>>> uncond_mat,string filename);
    vector<vector<vector<double>>> laws_filter_gray(vector<vector<vector<double>>> masks,int sz);
    
    int rows;
    int cols;
    int channels;
  

};

int cond_mat_comp(vector<vector<unsigned int>> test_mat,vector <vector<char>> uncond_mat);
#endif   /*IMAGEVEC_H */



