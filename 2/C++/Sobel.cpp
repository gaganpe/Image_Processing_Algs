
#include <iostream>
#include "ImageVec.h"
#include <fstream>

using namespace std;

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
ImageVec gradNorm(vector<vector<vector<double>>> Img,int channel){
    vector<vector<vector< unsigned int>>> retImg(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(1)));
    double maxval = -256,minval = 256;
    LOOP_THROUGH_ROW_COL 
    if(maxval<Img[i][j][channel]){
        maxval = Img[i][j][channel];
    }
    if(minval>Img[i][j][channel]){
        minval = Img[i][j][channel];
    }
    END_LOOP_THROUGH_ROW_COL  
    
    LOOP_THROUGH_ROW_COL 
    retImg[i][j][0] = ((Img[i][j][channel] - minval)/(maxval - minval))*255;
    END_LOOP_THROUGH_ROW_COL  
    return ImageVec(retImg);

}

ImageVec sobelMag(vector<vector<vector<double>>> Img,double thresh,string writeloc){
    vector<vector<vector< unsigned int>>> retImg(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(1)));
    double maxval = -256,minval = 256;
    LOOP_THROUGH_ROW_COL 
        retImg[i][j][0] = sqrt((Img[i][j][0]* Img[i][j][0])+ (Img[i][j][1]* Img[i][j][1]));
    END_LOOP_THROUGH_ROW_COL  
     
    LOOP_THROUGH_ROW_COL 
     if(maxval<retImg[i][j][0]){
        maxval = retImg[i][j][0];
    }
    if(minval>retImg[i][j][0]){
        minval = retImg[i][j][0];
    }
    END_LOOP_THROUGH_ROW_COL
     LOOP_THROUGH_ROW_COL 
    retImg[i][j][0] = ((retImg[i][j][0] - minval)/(maxval - minval))*255;
    END_LOOP_THROUGH_ROW_COL    
    ImageVec finalImg =  ImageVec(retImg);
    finalImg.writeToFile(writeloc);
     vector <unsigned int> Histogram= finalImg.HistGen();
    vector <unsigned int> mapping(256,0);
    vector<double> cdf(256,0);
	unsigned int data1=0;
    for(int i=0;i<Histogram.size();i++){
	data1+=Histogram[i];
        cdf[i] = (i>0?cdf[i-1]:0) + (double)Histogram[i]/((finalImg.rows)*(finalImg.cols));
        if(cdf[i]<thresh){
            mapping[i] = 255;
        }
        else{
            mapping[i] = 0;
        }
    }
    finalImg.transfer_Img(mapping);
    return finalImg; 


}

int main(int argc, char *argv[]){

    if (argc < 7){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image_XGradient.raw output_image_YGradient.raw output_image_Sobel_Op.raw Width Height" << endl;
		return 0;
	}
    ImageVec newImg(argv[1],atoi(argv[7]),atoi(argv[6]),C_RGB);
    ImageVec grayImg = newImg.RGBtoGRAY();
    vector<vector<vector<double>>> gradMaps = grayImg.sobel_filter();
    ImageVec gradX = gradNorm(gradMaps,0),gradY = gradNorm(gradMaps,1);

    gradX.writeToFile(argv[2]);
    gradY.writeToFile(argv[3]);
    ImageVec finalOp = sobelMag(gradMaps,0.96,argv[5]);
    finalOp.writeToFile(argv[4]);
    

    return 0;



}
