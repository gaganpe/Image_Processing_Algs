

#include <iostream>
#include "ImageVec.h"
#include <fstream>

using namespace std;
void writeMapping(string filename,vector<unsigned int>cdf_m);
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
    writeMapping("tf.txt",mapping);
    ImageVec retval(newImg);
    retval.transfer_Img(mapping);
    return retval;

}
void writeMapping(string filename,vector<unsigned int>cdf_m){
  ofstream myfile(filename);
        if(myfile.is_open()){
        for(int i=0;i<cdf_m.size();i++){
            myfile<<cdf_m[i]<< endl;
        }
        myfile.close();
        }
        else{
                    cout<<"File is not opened!"<<endl;
        }
}


int main(int argc, char *argv[]){

    if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw Width Height" << endl;
		return 0;
	}
ImageVec newImg(argv[1],atoi(argv[4]),atoi(argv[3]),C_GREY);
    newImg.writeHist("ori.txt");
    
    ImageVec writeImg = HistEqualization_Transfer_Fn(newImg);


    writeImg.writeToFile(argv[2]);
    writeImg.writeHist("op.txt");
    return 0;



}
