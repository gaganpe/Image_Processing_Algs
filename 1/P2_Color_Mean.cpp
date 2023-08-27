
#include <iostream>
#include "ImageVec.h"
using namespace std;
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
    
    
    
                if (argc < 6){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw Width Height" << endl;
		return 0;
	}
    ImageVec newImg(argv[1],atoi(argv[5]),atoi(argv[4]),C_RGB);
    ImageVec writeImg_mean(newImg);
    ImageVec writeImg_mean_fn = writeImg_mean.conv2d_allch(mean_Weight_gen(atoi(argv[6])));
    ImageVec cleanImg(argv[2],atoi(argv[5]),atoi(argv[4]),C_RGB);

    cout<<"Mean PSNR = "<<writeImg_mean_fn.psnr_(cleanImg)<<endl;
            
    
    
     writeImg_mean_fn.writeToFile(argv[3]);

}