
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

vector<vector<vector< int>>> readChannel(vector<vector<vector<int>>> Img,int channel) { 
vector<vector<vector< int>>> retval(Img.size(), vector<vector<int>>(Img[0].size(), vector< int>(1)));
    LOOP_THROUGH_ROW_COL      
        retval[i][j][0] = Img[i][j][channel];
    END_LOOP_THROUGH_ROW_COL  
    return retval;
}
vector<unsigned int> HistGen(vector<vector<vector<int>>> Img) { 
    vector<unsigned int> retval(256*2-1,0);
    LOOP_THROUGH_IMG
        retval[Img[i][j][k]+255]++;
     END_LOOP_THROUGH_IMG
    return retval;
}
void writeHist(vector<vector<vector<int>>> image,string filename){
  
  if(image[0][0].size() == 1){
    vector <unsigned int> Histogram_ori= HistGen(image);
        ofstream myfile(filename);
        if(myfile.is_open()){
        for(int i=0;i<Histogram_ori.size();i++){
            myfile<<Histogram_ori[i]<< endl;
        }
        myfile.close();
        }
        else{
                    cout<<"File is not opened!"<<endl;
        }
  }
  else{
    vector <vector<unsigned int>> final_hist;
    for(int i=0;i<3;i++){
      vector<vector<vector<int>>> retval = readChannel(image,i);
      vector <unsigned int> Histogram_ori= HistGen(retval);
      final_hist.push_back(Histogram_ori);
    }
    ofstream myfile(filename);
        if(myfile.is_open()){
        for(int i=0;i<final_hist[0].size();i++){
            myfile<<final_hist[0][i]<< " ";
            myfile<<final_hist[1][i]<< " ";
            myfile<<final_hist[2][i]<< endl;
        }
        myfile.close();
        }
        else{
                    cout<<"File is not opened!"<<endl;
        }
    
  }
            
}


int main(int argc, char *argv[]){
/*
    if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw Width Height" << endl;
		return 0;
	}*/
    ImageVec newImg(argv[2],atoi(argv[5]),atoi(argv[4]),C_RGB);
    ImageVec noisyImg(argv[1],atoi(argv[5]),atoi(argv[4]),C_RGB);
    ImageVec firstFilter = noisyImg.median_filter(3);
    ImageVec writeImg_mean_fn = firstFilter.bilateral_filter(10,20,3);
    writeImg_mean_fn.writeToFile(argv[3]);
    vector <vector <vector<int>>> firstdif = newImg.PixelDif(noisyImg);
    writeHist(firstdif,"Pepper_diff.txt");
    newImg.writeHist("Pepper_orig.txt");
    newImg.writeHist("pepper_noisy1.txt");
    
    
    
    return 0;



}
