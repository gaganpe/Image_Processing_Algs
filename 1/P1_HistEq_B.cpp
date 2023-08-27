

#include <iostream>
#include "ImageVec.h"


using namespace std;

ImageVec HistEqualization_Transfer_Fn(ImageVec newImg){
    vector <unsigned int> Histogram= newImg.HistGen();
    unsigned int total_pixels = newImg.rows * newImg.cols;
    unsigned int pix_per_bucket =round(double(total_pixels)/256),current_bucket = 0;
    vector <vector<unsigned int>> mapping(256);
    vector <unsigned int> remaining_pix(256,pix_per_bucket);
    remaining_pix[255]-= (pix_per_bucket *256 -total_pixels);
	for(int i=0;i<Histogram.size();i++){
        unsigned int pix_required = Histogram[i];
        while(pix_required>0){
            if(remaining_pix[current_bucket] ==0){
                    current_bucket++;
            }
            else if(remaining_pix[current_bucket]<pix_required){
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

if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw Width Height" << endl;
		return 0;
	}
    ImageVec newImg(argv[1],atoi(argv[4]),atoi(argv[3]),C_GREY);
    
    ImageVec writeImg = HistEqualization_Transfer_Fn(newImg);
    vector <unsigned int> Histogram= writeImg.HistGen();

    writeImg.writeToFile(argv[2]);
    writeImg.writeHist("op_bf.txt");
    return 0;



}
