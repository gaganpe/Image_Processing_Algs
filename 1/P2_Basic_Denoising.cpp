
#include <iostream>
#include "ImageVec.h"


using namespace std;



vector<vector<double>> Gaussian_Weight_gen(double sigma,int filter_sz){
    vector<vector<double>> retval;
    for(int i=0;i<filter_sz;i++){
        vector<double> new_vec;
        for(int j=0;j<filter_sz;j++){
            int x_dist = j - filter_sz/2;
            int y_dist = i - filter_sz/2;
            double val = (sqrt(7)/(sqrt(2*22)*sigma))*exp(-1*double(x_dist*x_dist + y_dist*y_dist)/(2*sigma));
            new_vec.push_back(val);
        }
        retval.push_back(new_vec);
    }
    return retval;

}
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

    if (argc < 10){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw Width Height" << endl;
		return 0;
	}
    ImageVec newImg(argv[1],atoi(argv[5]),atoi(argv[4]),C_GREY);
    ImageVec writeImg_mean(newImg);
    ImageVec writeImg_mean_fn = writeImg_mean.conv2d_allch(mean_Weight_gen(atoi(argv[6])));
    ImageVec cleanImg(argv[9],atoi(argv[5]),atoi(argv[4]),C_GREY);
    
    ImageVec writeImg_Gaus(newImg);
    ImageVec writeImg_Gaus_fin =  writeImg_Gaus.conv2d_allch(Gaussian_Weight_gen(stod(argv[8]),atoi(argv[7])));
    cout<<"Mean PSNR = "<<writeImg_mean_fn.psnr_(cleanImg)<<"Gaussian PSNR = "<<writeImg_Gaus_fin.psnr_(cleanImg)<<endl;
    writeImg_mean_fn.writeToFile(argv[2]);
    writeImg_Gaus_fin.writeToFile(argv[3]);
    return 0;



}
