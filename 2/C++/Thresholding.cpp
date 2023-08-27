

#include <iostream>
#include "ImageVec.h"
#include <fstream>
#include <random>

using namespace std;

vector<vector<int>> gen_dithering(int size){
   vector<vector<int>> I2{{1,2}, {3,0}};
   
   int loop_size = int(log2(size))-1;
   vector<vector<int>> retval = I2,orig = I2;
   for(int i=0;i<loop_size;i++){
    vector <vector <int>> new_ret(orig.size()*2, vector<int>(orig[0].size()*2));
   
    for(int j=0;j<orig.size();j++){
        int x_index = 0;
        int y_index = 0;
      for(int k = 0;k<orig[0].size();k++){
        int orig_size = orig[0].size();
    
        
        new_ret[j][k] = 4*orig[j][k]+1;
        new_ret[j][(k)+orig_size] = 4*orig[j][k]+2;
        new_ret[j+orig_size][(k)] = 4*orig[j][k]+3;
        new_ret[(j)+orig_size][(k)+orig_size] = 4*orig[j][k];
      }
    }
    orig = new_ret;
    retval = new_ret;
   }
   return retval;

}
vector<vector<double>> gen_thresh(vector<vector<int>> dither_mat){
    vector <vector <double>> new_ret(dither_mat.size(), vector<double>(dither_mat[0].size()));
    for(int i=0;i<dither_mat.size();i++){
        for(int j=0;j<dither_mat[0].size();j++){
            new_ret[i][j] = (double(double(dither_mat[i][j])) + 0.5)*double(255)/double(dither_mat.size()*dither_mat.size());
        }
    }
    return new_ret;


}
void writeImat(vector<vector<int>> thresh_mat,string filename){
      ofstream myfile(filename);
        if(myfile.is_open()){
        for(int i=0;i< thresh_mat.size();i++){
            for(int j=0;j< thresh_mat[0].size();j++){
            myfile<<(thresh_mat[i][j])<< " ";
            }
            myfile<<endl;
        }
        myfile.close();
        }
        else{
                    cout<<"File is not opened!"<<endl;
        }
        return;
    
}
void writeThresh(vector<vector<double>> thresh_mat,string filename){
      ofstream myfile(filename);
        if(myfile.is_open()){
        for(int i=0;i< thresh_mat.size();i++){
            for(int j=0;j< thresh_mat[0].size();j++){
            myfile<<floor(thresh_mat[i][j])<< " ";
            }
            myfile<<endl;
        }
        myfile.close();
        }
        else{
                    cout<<"File is not opened!"<<endl;
        }
        return;
    
}


int main(int argc, char *argv[]){

    if (argc < 6){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image_XGradient.raw output_image_YGradient.raw output_image_Sobel_Op.raw Width Height" << endl;
		return 0;
	}
    ImageVec newImg(argv[1],atoi(argv[8]),atoi(argv[7]),C_GREY);
    
     ImageVec fixed_thresh_op = newImg.fixed_thresh();
     ImageVec random_thresh_op = newImg.random_thresh();

    fixed_thresh_op.writeToFile(argv[2]);
    random_thresh_op.writeToFile(argv[3]);
    vector<vector<double>> dither_mat = gen_thresh(gen_dithering(2));
     writeThresh(dither_mat,"I2.txt");
    
    
    ImageVec I4op = newImg.dither_thresh((dither_mat));
    I4op.writeToFile(argv[4]);
    vector<vector<double>> dither_mat1 = gen_thresh(gen_dithering(8));
    writeImat(gen_dithering(8),"I8_val.txt");
    writeImat(gen_dithering(32),"I32_val.txt");
    writeThresh(dither_mat1,"I8.txt");
    
    ImageVec I8op = newImg.dither_thresh((dither_mat1));
    I8op.writeToFile(argv[5]);
    // vector<vector<int>> dither_mat2 = gen_dithering(16);
    // for(int i=0;i<dither_mat2.size();i++){
    //     for(int j=0;j<dither_mat2.size();j++){
    //         cout<<dither_mat2[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    // ImageVec I16op = newImg.dither_thresh(gen_thresh(dither_mat2));
    // I16op.writeToFile("I16.raw");
    vector<vector<double>> dither_mat3 = gen_thresh(gen_dithering(32));
    writeThresh(dither_mat3,"I32.txt");
    ImageVec I32op = newImg.dither_thresh((dither_mat3));
    I32op.writeToFile(argv[6]);

    
    
    return 0;



}
