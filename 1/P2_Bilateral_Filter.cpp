
#include <iostream>
#include "ImageVec.h"


using namespace std;




int main(int argc, char *argv[]){
/*
    if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw Width Height" << endl;
		return 0;
	}*/
    ImageVec newImg(argv[1],atoi(argv[4]),atoi(argv[3]),C_GREY);
    ImageVec writeImg_bil(newImg);
    ImageVec writeImg_mean_fn = writeImg_bil.bilateral_filter(10,20,3);
    ImageVec cleanImg(argv[5],atoi(argv[4]),atoi(argv[3]),C_GREY);
    cout<<"Bilateral PSNR = "<<writeImg_mean_fn.psnr_(cleanImg)<<endl;
    writeImg_mean_fn.writeToFile(argv[2]);
    
    return 0;



}
