
#include <iostream>
#include "ImageVec.h"
#include <fstream>
#include <random>

using namespace std;

vector<vector<double>> floyd_steinberg = {{0,0,0},{0,0,7},{3,5,1}};
vector<vector<double>> jjn = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,7,5},{3,5,7,5,3},{1,3,5,3,1}};
vector<vector<double>> stucki = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,8,4},{2,4,8,4,2},{1,2,4,2,1}};

int main(int argc, char *argv[]){

    if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image_XGradient.raw output_image_YGradient.raw output_image_Sobel_Op.raw Width Height" << endl;
		return 0;
	}
    ImageVec newImg(argv[1],atoi(argv[4]),atoi(argv[3]),C_RGB);
    ImageVec convImg= newImg.RGBtoCMY();
    // cout<<"w"<<endl;
    ImageVec floyd_steinberg_op = convImg.error_diffu(floyd_steinberg).CMYtoRGB();
    // cout<<"w2"<<endl;
    // ImageVec jjn_op = convImg.error_diffu(jjn).CMYtoRGB();
    // ImageVec stucki_op = convImg.error_diffu(stucki).CMYtoRGB();
    // cout<<"w3"<<endl;
    floyd_steinberg_op.writeToFile(argv[2]);
    // jjn_op.writeToFile("jjn_op.raw");
    // stucki_op.writeToFile("stucki_op.raw");

    
    
    return 0;



}

