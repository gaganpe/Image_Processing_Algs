
#include <iostream>
#include "ImageVec.h"


using namespace std;



int main(int argc, char *argv[]){
    if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw Width Height" << endl;
		return 0;
	}
    ImageVec newImg = ImageVec(argv[1],atoi(argv[4]),atoi(argv[3]),C_GREY);
    
    ImageVec writeImg = newImg.demosaic();
    writeImg.writeToFile(argv[2]);
    return 0;



}
