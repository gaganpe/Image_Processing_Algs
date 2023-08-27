

#include <iostream>
#include "ImageVec.h"
#include <fstream>
#include <random>

using namespace std;

vector<vector<double>> floyd_steinberg = {{0,0,0},{0,0,7},{3,5,1}};
vector<vector<double>> jjn = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,7,5},{3,5,7,5,3},{1,3,5,3,1}};
vector<vector<double>> stucki = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,8,4},{2,4,8,4,2},{1,2,4,2,1}};

int main(int argc, char *argv[]){

    if (argc < 7){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image_XGradient.raw output_image_YGradient.raw output_image_Sobel_Op.raw Width Height" << endl;
		return 0;
	}
    ImageVec newImg(argv[1],atoi(argv[6]),atoi(argv[5]),C_GREY);
    
    ImageVec floyd_steinberg_op = newImg.error_diffu(floyd_steinberg);
    ImageVec jjn_op = newImg.error_diffu(jjn);
    ImageVec stucki_op = newImg.error_diffu(stucki);
    floyd_steinberg_op.writeToFile(argv[2]);
    jjn_op.writeToFile(argv[3]);
    stucki_op.writeToFile(argv[4]);

    
    
    return 0;



}
