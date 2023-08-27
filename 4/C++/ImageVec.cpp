


#include <iostream>

#include "ImageVec.h"
#include "ImgMat.h"


using namespace std;

ImageVec::ImageVec(ImageVec &ConstrIm){
    this->channels = ConstrIm.channels;
    this->rows = ConstrIm.rows;
    this->cols = ConstrIm.cols;
    Img.resize(rows,vector<vector<unsigned int> >(cols,vector<unsigned int>(channels)));
    LOOP_THROUGH_IMG
            Img[i][j][k] = ConstrIm.Img[i][j][k];
    END_LOOP_THROUGH_IMG
}
ImageVec::ImageVec(vector<vector<vector<unsigned int>>> Img){
    this->rows = Img.size();
    this->cols = Img[0].size();
    this->channels = Img[0][0].size();
    this->Img = Img;
}
ImageVec::ImageVec(vector<vector<unsigned int>> Img){
    this->rows = Img.size();
    this->cols = Img[0].size();
    this->channels = 1;
    this->Img.resize(rows,vector<vector<unsigned int> >(cols,vector<unsigned int>(channels)));
    LOOP_THROUGH_ROW_COL
      this->Img[i][j][0] =Img[i][j];
    END_LOOP_THROUGH_ROW_COL
    
}
/// @brief Obtain ImageVec from a raw image from Path
/// @param s 
ImageVec::ImageVec(string s,int rows,int cols,int channels) { 
    this->rows = rows;
    this->cols = cols;
    this->channels= channels;

  Img.resize(rows,vector<vector<unsigned int> >(cols,vector<unsigned int>(channels)));
  unsigned char Imagedata[rows*cols*channels];
  FILE *file;
  // Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(s.c_str(),"rb"))) {
		cout << "Cannot open file: " << s <<endl;
		exit(1);
	}
  fread(Imagedata, sizeof(char), rows*cols*channels, file);
  unsigned int sz=0;
  LOOP_THROUGH_IMG
            Img[i][j][k] = Imagedata[sz++];
  END_LOOP_THROUGH_IMG
  fclose(file);

  return;
}
ImageVec::ImageVec(int rows,int cols,int channels) { 
    this->rows = rows;
    this->cols = cols;
    this->channels = channels;
    Img.resize(rows,vector<vector<unsigned int> >(cols,vector<unsigned int>(channels)));
  return;
}
void ImageVec::threshold(unsigned int value) { 
    LOOP_THROUGH_IMG            
        Img[i][j][k] = Img[i][j][k]>value?255:0;
    END_LOOP_THROUGH_IMG  
}
ImageVec ImageVec::RGBtoGRAY() { 
  vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(1)));
    LOOP_THROUGH_ROW_COL      
        unsigned int R_VAL,G_VAL,B_VAL,Y_VAL;
        R_VAL = Img[i][j][0];
        G_VAL = Img[i][j][1];
        B_VAL = Img[i][j][2];
        Y_VAL = round((0.2989 * double(R_VAL)) + (0.5870 * (double(G_VAL))) + (0.1140 * (double(B_VAL))));
      op_img[i][j][0] = Y_VAL;
    END_LOOP_THROUGH_ROW_COL  
    return ImageVec(op_img);
}
ImageVec ImageVec::RGBtoCMY() { 
  vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(3)));
    LOOP_THROUGH_IMG
        op_img[i][j][k] = 255 - Img[i][j][k];
    END_LOOP_THROUGH_IMG  
    return ImageVec(op_img);
}
ImageVec ImageVec::CMYtoRGB() { 
  vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(3)));
    LOOP_THROUGH_IMG
        op_img[i][j][k] = 255 - Img[i][j][k];
    END_LOOP_THROUGH_IMG  
    return ImageVec(op_img);
}
void ImageVec::RGBtoYUV() { 
    LOOP_THROUGH_ROW_COL      
        unsigned int R_VAL,G_VAL,B_VAL,Y_VAL,U_VAL,V_VAL;
        R_VAL = Img[i][j][0];
        G_VAL = Img[i][j][1];
        B_VAL = Img[i][j][2];
        Y_VAL = round((0.257 * double(R_VAL)) + (0.504 * (double(G_VAL))) + (0.098 * (double(B_VAL))) + 16);
        U_VAL = round(-(0.148 * double(R_VAL)) - (0.291 * (double(G_VAL))) + (0.439 * (double(B_VAL))) + 128);
        V_VAL = round((0.439 * double(R_VAL)) - (0.368 * (double(G_VAL))) - (0.071 * (double(B_VAL))) + 128);
        Img[i][j][0] = Y_VAL;
        Img[i][j][1] = U_VAL;
        Img[i][j][2] = V_VAL;
    END_LOOP_THROUGH_ROW_COL  
}
void ImageVec::YUVtoRGB() { 
    LOOP_THROUGH_ROW_COL      
         int R_VAL,G_VAL,B_VAL,Y_VAL,U_VAL,V_VAL;
        Y_VAL = Img[i][j][0];
        U_VAL = Img[i][j][1];
        V_VAL = Img[i][j][2];
        double test1 = (round((1.164 * double((double(Y_VAL) - 16)) + (1.596 * (double(double(V_VAL) - 128))) )));
        double test2 = round((1.164 * double(double(Y_VAL) - 16)) - (0.391 * (double(double(U_VAL) - 128))) - (0.813 * (double(double(V_VAL) - 128))) );
        double test3 = round((1.164 * double(double(Y_VAL) - 16)) + (2.018 * (double(double(U_VAL) - 128))) );
        R_VAL = (test1<0?0:test1>255?255:(unsigned int)test1);
        G_VAL =  (test2<0?0:test2>255?255:(unsigned int)test2);
        B_VAL =(test3<0?0:test3>255?255:(unsigned int)test3);;
        Img[i][j][0] = R_VAL;
        Img[i][j][1] = G_VAL;
        Img[i][j][2] = B_VAL;
    END_LOOP_THROUGH_ROW_COL  
}

vector<vector<unsigned int>> ImageVec::readChannel(int channel) { 
  vector< vector<unsigned int>> retval(Img.size(), vector<unsigned int> (Img[0].size()));
    LOOP_THROUGH_ROW_COL      
        retval[i][j] = Img[i][j][channel];
    END_LOOP_THROUGH_ROW_COL  
    return retval;
}

void ImageVec::writeChannel(int channel,vector<vector<unsigned int>>Img_Chan) {
    LOOP_THROUGH_ROW_COL      
        Img[i][j][channel] =  Img_Chan[i][j];
    END_LOOP_THROUGH_ROW_COL  
    
}

void ImageVec::transfer_Img(vector <unsigned int> mapping) { 
    LOOP_THROUGH_IMG            
        Img[i][j][k] = mapping[Img[i][j][k]];
    END_LOOP_THROUGH_IMG  
}


void ImageVec::transfer_Img_BF(vector <vector<unsigned int>> mapping) {
    LOOP_THROUGH_IMG            
      vector <unsigned int> mapping_vec = mapping[Img[i][j][k]];
      unsigned int orig_value = Img[i][j][k];
      Img[i][j][k] = mapping_vec[0];

      mapping[orig_value][1] --;
      if(mapping[orig_value][1] == 0){
        mapping[orig_value].erase(mapping[orig_value].begin());
        mapping[orig_value].erase(mapping[orig_value].begin());
      }
    END_LOOP_THROUGH_IMG 
}
vector<unsigned int> ImageVec::HistGen() { 
    vector<unsigned int> retval(256,0);
    LOOP_THROUGH_IMG            
        retval[Img[i][j][k]]++;
    END_LOOP_THROUGH_IMG  
    return retval;
}
ImageVec ImageVec::dither_thresh(vector<vector<double>> thresh_mask) { 
  int row_ind = thresh_mask.size();
  int col_ind = thresh_mask[0].size();
  vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(1)));
    
    LOOP_THROUGH_IMG            
      op_img[i][j][k] = Img[i][j][k]>thresh_mask[i%row_ind][j%col_ind]?255:0;
    END_LOOP_THROUGH_IMG  
    return ImageVec(op_img);
}
ImageVec ImageVec::fixed_thresh() { 
  vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(1)));
    
    LOOP_THROUGH_IMG            
      op_img[i][j][k] = Img[i][j][k]>128?255:0;
    END_LOOP_THROUGH_IMG  
    return ImageVec(op_img);
}
ImageVec ImageVec::random_thresh() { 
  vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(1)));
    random_device rand_gen;
    mt19937 mtgen(rand_gen());
    uniform_int_distribution<> distributn(0,256);
    LOOP_THROUGH_IMG            
      op_img[i][j][k] = Img[i][j][k]>(distributn(mtgen))?255:0;
    END_LOOP_THROUGH_IMG  
    return ImageVec(op_img);
}
vector<vector<vector<int>>> ImageVec::PixelDif(ImageVec Image2) { 
    vector<vector<vector< int>>> retval(Img.size(), vector<vector<int>>(Img[0].size(), vector< int>(Img[0][0].size())));
    LOOP_THROUGH_IMG  
        int dif = int(this->Img[i][j][k]) - int(Image2.Img[i][j][k]) ;
        retval[i][j][k] = dif;
    END_LOOP_THROUGH_IMG  
    return retval;
}

void ImageVec::writeToFile(string s) { 
    unsigned char Imagedata[rows*cols*channels];
    FILE * file;
	if (!(file=fopen(s.c_str(),"wb"))) {
		cout << "Cannot open file: " <<s << endl;
		exit(1);
	}
    unsigned int sz=0;
    LOOP_THROUGH_IMG            
        Imagedata[sz++] = Img[i][j][k];
    END_LOOP_THROUGH_IMG  
	fwrite(Imagedata, sizeof(unsigned char), rows*cols*channels, file);
	fclose(file);
}


void ImageVec::writeHist(string filename){
  
  if(this->Img[0][0].size() == 1){
    vector <unsigned int> Histogram_ori= HistGen();
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
      ImageVec newImg(this->readChannel(i));
      vector <unsigned int> Histogram_ori= newImg.HistGen();
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

double ImageVec::psnr_(ImageVec cleanImg) { 
  double val = 0;
LOOP_THROUGH_IMG
          val+=  (Img[i][j][k] - cleanImg.Img[i][j][k])*(Img[i][j][k] - cleanImg.Img[i][j][k]);
    END_LOOP_THROUGH_IMG
   double mse =  val/(Img[0].size()*Img.size()*Img[0][0].size());
   
   return 10*log10(double(255*255)/mse);
}
ImageVec ImageVec::bilateral_filter(double sigmac ,double sigmas,int mask_sz) { 
  //Bilinear Interpolation Masks;
int padding_sz = mask_sz/2;
vector<vector<vector<unsigned int >>> Img_with_Pad = Img;
vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(Img[0][0].size())));

  for(int i=0;i<Img_with_Pad.size();i++){
    for(int j=0;j<padding_sz;j++){
    vector<unsigned int > newvec_st(Img_with_Pad[0][0].size(),0);
    vector<unsigned int > newvec_en(Img_with_Pad[0][0].size(),0);
    Img_with_Pad[i].insert(Img_with_Pad[i].begin(),newvec_st);
    Img_with_Pad[i].push_back(newvec_en);
    }
  }
 for(int t=0;t<padding_sz;t++){
  vector<vector<unsigned int >> newvec_st_R,newvec_en_R;
  for(int i=0;i<Img_with_Pad[0].size();i++){
      vector<unsigned int > newvec_st(Img_with_Pad[0][0].size(),0);
    vector<unsigned int > newvec_en(Img_with_Pad[0][0].size(),0);
    newvec_st_R.push_back(newvec_st);
    newvec_en_R.push_back(newvec_en);
  }
  Img_with_Pad.insert(Img_with_Pad.begin(),newvec_st_R);
  Img_with_Pad.push_back(newvec_en_R);
  //Padding is done. Convolution 
 }
  for(int i=0;i<(Img_with_Pad.size()-padding_sz);i++){ 
    for(int j=0;j<(Img_with_Pad[0].size()-padding_sz);j++){ 
      
      if((i-(padding_sz)>=0) && (j-(padding_sz)>=0)){
        
        
        for(int k = 0;k<Img_with_Pad[0][0].size();k++){
          double val1=0;
          unsigned int center_pix_val = Img_with_Pad[i][j][k];
          double weight_sum = 0;
          for(int l=0;l<mask_sz;l++){
            for(int m=0;m<mask_sz;m++){
              double pixel_dif = double(Img_with_Pad[i + l - (padding_sz)][j + m - (padding_sz)][k]) - double(center_pix_val); 
              double weight = 0;
              int x_dist = m - mask_sz/2;
            int y_dist = l -  mask_sz/2;

            double dist_comp = -1*double(x_dist*x_dist + y_dist*y_dist)/(2*sigmac*sigmac);
            double int_comp = -(1*double(pixel_dif*pixel_dif)/(2* sigmas*sigmas));
            weight = exp(dist_comp + int_comp);
              val1 += weight * (double)Img_with_Pad[i + l - (padding_sz)][j + m - (padding_sz)][k];
              weight_sum+=weight;
              }
              }
              val1 = val1/weight_sum; 
              op_img[i - padding_sz][j - padding_sz][k] = (val1);
            }
            
            
            
            
            }
        }
      }
    return ImageVec(op_img);
  }
vector<vector <vector <double>>> ImageVec::sobel_filter() { 
  int mask_sz = 3;
  vector<vector<double>> mask_X{{-1, 0, 1}, {-2, 0, 2},{-1, 0, 1}};
  vector<vector<double>> mask_Y{{1, 2, 1}, {0, 0, 0},{-1, -2, -1}};

  //Bilinear Interpolation Masks;
int padding_sz = mask_sz/2;
vector<vector<vector<unsigned int >>> Img_with_Pad = Img;
vector<vector<vector<double>>> op_img(Img.size(), vector<vector<double>>(Img[0].size(), vector<double>(2)));
  //Copy the Second and the last second Column
for(int i=0;i<Img_with_Pad.size();i++){
    vector<unsigned int > newvec_st,newvec_en;
    for(int j=0;j<Img_with_Pad[0][0].size();j++){
      newvec_st.push_back(Img_with_Pad[i][1][j]);
      newvec_en.push_back(Img_with_Pad[i][Img_with_Pad[i].size()-2][j]);
    }
    Img_with_Pad[i].insert(Img_with_Pad[i].begin(),newvec_st);
    Img_with_Pad[i].push_back(newvec_en);
  }
  //Copy the Second Row
  vector<vector<unsigned int >> newvec_st_R,newvec_en_R;
  for(int i=0;i<Img_with_Pad[0].size();i++){
    vector<unsigned int > newvec_st,newvec_en;
    for(int j=0;j<Img_with_Pad[0][0].size();j++){
      newvec_st.push_back(Img_with_Pad[1][i][j]);
      newvec_en.push_back(Img_with_Pad[Img_with_Pad.size()-2][i][j]);
    }
    newvec_st_R.push_back(newvec_st);
    newvec_en_R.push_back(newvec_en);
  }
  Img_with_Pad.insert(Img_with_Pad.begin(),newvec_st_R);
  Img_with_Pad.push_back(newvec_en_R);
  //Padding is done. Convolution 
 

  for(int i=0;i<(Img_with_Pad.size()-padding_sz);i++){ 
    for(int j=0;j<(Img_with_Pad[0].size()-padding_sz);j++){ 
      int p = 0;
      if((i-(padding_sz)>=0) && (j-(padding_sz)>=0)){  
              double valx=0,valy=0;
            for(int l=0;l<mask_sz;l++){
            for(int m=0;m<mask_sz;m++){
  
              valx += (mask_X[l][m] * Img_with_Pad[i + l - (padding_sz)][j + m - (padding_sz)][0]);
              valy += (mask_Y[l][m] * Img_with_Pad[i + l - (padding_sz)][j + m - (padding_sz)][0]);
            }
          }
           
             
              op_img[i - padding_sz][j - padding_sz][0] = (valx);
              op_img[i - padding_sz][j - padding_sz][1] = (valy);
            
            }
        }
      }
    
    return (op_img);
  }
ImageVec ImageVec::median_filter(int mask_sz) { 
  //Bilinear Interpolation Masks;
int padding_sz = mask_sz/2;
vector<vector<vector<unsigned int >>> Img_with_Pad = Img;
vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(Img[0][0].size())));
  //Copy the Second and the last second Column
  
  for(int j=0;j<padding_sz;j++){
  for(int i=0;i<Img_with_Pad.size();i++){
    vector<unsigned int > newvec_st,newvec_en;
    for(int j=0;j<Img_with_Pad[0][0].size();j++){
      newvec_st.push_back(Img_with_Pad[i][0][j]);
      newvec_en.push_back(Img_with_Pad[i][Img_with_Pad[i].size()-1][j]);
    }
    Img_with_Pad[i].insert(Img_with_Pad[i].begin(),newvec_st);
    Img_with_Pad[i].push_back(newvec_en);
  }
  }
 for(int t=0;t<padding_sz;t++){
  vector<vector<unsigned int >> newvec_st_R,newvec_en_R;
  for(int i=0;i<Img_with_Pad[0].size();i++){
      vector<unsigned int > newvec_st;
    vector<unsigned int > newvec_en;
      for(int j=0;j<Img_with_Pad[0][0].size();j++){
      newvec_st.push_back(Img_with_Pad[0][i][j]);
      newvec_en.push_back(Img_with_Pad[Img_with_Pad.size()-1][i][j]);
    }
    newvec_st_R.push_back(newvec_st);
    newvec_en_R.push_back(newvec_en);
  }
  
  Img_with_Pad.insert(Img_with_Pad.begin(),newvec_st_R);
  Img_with_Pad.push_back(newvec_en_R);
  //Padding is done. Convolution 
 }
 

  for(int i=0;i<(Img_with_Pad.size()-padding_sz);i++){ 
    for(int j=0;j<(Img_with_Pad[0].size()-padding_sz);j++){ 
      
      if((i-(padding_sz)>=0) && (j-(padding_sz)>=0)){
        
        
        for(int k = 0;k<Img_with_Pad[0][0].size();k++){
          
              vector<int> vec;
            for(int l=0;l<mask_sz;l++){
            for(int m=0;m<mask_sz;m++){
              
              vec.push_back(Img_with_Pad[i + l - (padding_sz)][j + m - (padding_sz)][k]);
              
              }
            }
            sort(vec.begin(),vec.end());
             
              op_img[i - padding_sz][j - padding_sz][k] = vec[((mask_sz*mask_sz)/2)];
            }
            }
        }
      }
    
    return ImageVec(op_img);
  }
ImageVec ImageVec::binarize() {
ImageVec Op_Img(*this);
int max_int = 0;
LOOP_THROUGH_IMG
max_int = (this->Img[i][j][k]>max_int)?this->Img[i][j][k]:max_int;
END_LOOP_THROUGH_IMG
LOOP_THROUGH_IMG
Op_Img.Img[i][j][k] = (this->Img[i][j][k]>(max_int/2))?1:0;
END_LOOP_THROUGH_IMG

return Op_Img;

}
ImageVec ImageVec::rev_binarize() {
ImageVec Op_Img(*this);
LOOP_THROUGH_IMG
Op_Img.Img[i][j][k] = (this->Img[i][j][k])?255:0;
END_LOOP_THROUGH_IMG

return Op_Img;

} 
ImageVec ImageVec::thinning(unordered_set <string> cond_mat,vector<vector<vector<char>>> uncond_mat,string filename){
  //Bilinear Interpolation Masks;
  string subStr = filename.substr(0, filename.size() - 4);
    
vector<vector<vector<unsigned int>>> mark_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(Img[0][0].size())));
  //Copy the Second and the last second Column
int count=1;
int iter_ = 0;
    
  ImageVec bin_img = this->binarize();
  vector<vector<vector<unsigned int >>> thinned_Img = bin_img.Img;
  int abc =0;
 while(count!=0){
LOOP_THROUGH_ROW_COL
  mark_img[i][j][0] = 0;
END_LOOP_THROUGH_ROW_COL
if(iter_%5==0 && iter_){
  string op_name = subStr + to_string(iter_)+ ".raw";
//ImageVec(thinned_Img).rev_binarize().writeToFile(op_name);
}
iter_++;
  count = 0;
  for(int i=1;i<(this->rows)-1;i++){ 
    for(int j=1;j<(this->cols)-1;j++){ 
            string comp;
    
            for(int l=0;l<3;l++){
            for(int m=0;m<3;m++){
              comp+=to_string(thinned_Img[i + l - (1)][j + m - (1)][0]);
              }
            }

            if(cond_mat.find(comp)!=cond_mat.end()){
              mark_img[i][j][0] = 1;
            }
      }
  }

  for(int i=1;i<(this->rows)-1;i++){ 
    for(int j=1;j<(this->cols)-1;j++){ 
            string comp;
            if(mark_img[i][j][0]==0){
              continue;
            }
            vector<vector<unsigned int>> test_mat;

            for(int l=0;l<3;l++){
                vector<unsigned int> test_vec;
            for(int m=0;m<3;m++){
                test_vec.push_back(mark_img[i + l - (1)][j + m - (1)][0]);
              }
              test_mat.push_back(test_vec);
            }
            int op=0;
          for(int l = 0;l<uncond_mat.size();l++){
            if(cond_mat_comp(test_mat,uncond_mat[l])){
              op=1;
              break;
            }
            }
          if(!op){
            
                count++;
            
            thinned_Img[i][j][0] = 0;
            
          }
            
      }
  }
  cout<<"Count = "<<count<<endl;
    
    
 }
    
    return ImageVec(thinned_Img).rev_binarize();
  }
int cond_mat_comp(vector<vector<unsigned int>> test_mat,vector <vector<char>> uncond_mat){
  int orval=0,orset=0;
        for(int i=0;i<uncond_mat.size();i++){
          for(int j=0;j<uncond_mat[i].size();j++){
            switch(uncond_mat[i][j]){
              case ('0'):
                if(test_mat[i][j]){
                  return 0;
                }
                break;
              case ('M'):
                if(!test_mat[i][j]){
                  return 0;
                }
                break;
              case ('A'):
                if(test_mat[i][j]){
                  orval++;
                }
                  orset = 1;
                break;
            case ('B'):
                if(test_mat[i][j]){
                  orval++;
                }
                   orset = 1;
                break;
              case ('C'):
                if(test_mat[i][j]){
                  orval++;
                }
                   orset = 1;
                break;
               default:
                break;

            }
        

          }
        }
        if(orset && orval<1){
          return 0;
        }
        
        return 1;



}
ImageVec ImageVec::conv2d_allch(vector<vector<double>> mask) { 
	//Bilinear Interpolation Masks;
	int padding_sz = mask.size()/2;
	vector<vector<vector<unsigned int >>> Img_with_Pad = Img;
	vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(Img[0][0].size())));
	double weight_sum = 0;
	for(int i=0;i<mask.size();i++){
		for(int j=0;j<mask[0].size();j++){
			weight_sum+=mask[i][j];
		}
	}
	for(int i=0;i<Img_with_Pad.size();i++){
		for(int j=0;j<padding_sz;j++){
			vector<unsigned int> newvec_st(Img_with_Pad[0][0].size(),0);
			vector<unsigned int> newvec_en(Img_with_Pad[0][0].size(),0);
			/*  for(int j=0;j<Img_with_Pad[0][0].size();j++){
			    newvec_st.push_back(Img_with_Pad[i][1][j]);
			    newvec_en.push_back(Img_with_Pad[i][Img_with_Pad[i].size()-2][j]);
			    }*/
			Img_with_Pad[i].insert(Img_with_Pad[i].begin(),newvec_st);
			Img_with_Pad[i].push_back(newvec_en);
		}
	}
	for(int t=0;t<padding_sz;t++){
		vector<vector<unsigned int >> newvec_st_R,newvec_en_R;
		for(int i=0;i<Img_with_Pad[0].size();i++){
			vector<unsigned int> newvec_st(Img_with_Pad[0][0].size(),0);
			vector<unsigned int> newvec_en(Img_with_Pad[0][0].size(),0);
			/* for(int j=0;j<Img_with_Pad[0][0].size();j++){
			   newvec_st.push_back(Img_with_Pad[1][i][j]);
			   newvec_en.push_back(Img_with_Pad[Img_with_Pad.size()-2][i][j]);
			   }*/
			newvec_st_R.push_back(newvec_st);
			newvec_en_R.push_back(newvec_en);
		}
		Img_with_Pad.insert(Img_with_Pad.begin(),newvec_st_R);
		Img_with_Pad.push_back(newvec_en_R);
		//Padding is done. Convolution 
	}
	for(int i=0;i<(Img_with_Pad.size()-padding_sz);i++){ 
		for(int j=0;j<(Img_with_Pad[0].size()-padding_sz);j++){ 

			if((i-(padding_sz)>=0) && (j-(padding_sz)>=0)){

				for(int k = 0;k<Img_with_Pad[0][0].size();k++){
					double val1=0;
					for(int l=0;l<mask.size();l++){
						for(int m=0;m<mask.size();m++){
							val1 += mask[l][m] * Img_with_Pad[i + l - (padding_sz)][j + m - (padding_sz)][k];
						}
					}
					val1 = val1/weight_sum; 
					op_img[i - padding_sz][j - padding_sz][k] = (val1);
				}




			}
		}
	}




	return ImageVec(op_img);
  }



ImageVec ImageVec::error_diffu(vector<vector<double>> mask) { 
	double mask_sum = 0;
	for(int i=0;i<mask.size();i++){
		for(int j=0;j<mask[0].size();j++){
			// cout<<mask[i][j]<<" ";
			mask_sum+= mask[i][j];
		}

	}
	for(int i=0;i<mask.size();i++){
		for(int j=0;j<mask[0].size();j++){

			mask[i][j] = mask[i][j]/mask_sum;
			//    cout<<mask[i][j]<<" ";
		}

	}
	//Bilinear Interpolation Masks;
	static int test1 = 0;
	int padding_sz = mask.size()/2;

	vector<vector<vector<double>>> Img_with_Pad(Img.size(), vector<vector<double>>(Img[0].size(), vector<double>(Img[0][0].size())));
	vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(Img[0][0].size())));

	for(int i=0;i<Img_with_Pad.size();i++){
		for(int j=0;j<padding_sz;j++){
			vector<double > newvec_st(Img_with_Pad[0][0].size(),0);
			vector<double > newvec_en(Img_with_Pad[0][0].size(),0);
			/*  for(int j=0;j<Img_with_Pad[0][0].size();j++){
			    newvec_st.push_back(Img_with_Pad[i][1][j]);
			    newvec_en.push_back(Img_with_Pad[i][Img_with_Pad[i].size()-2][j]);
			    }*/
			Img_with_Pad[i].insert(Img_with_Pad[i].begin(),newvec_st);
			Img_with_Pad[i].push_back(newvec_en);
		}
	}
	for(int t=0;t<padding_sz;t++){
		vector<vector<double >> newvec_st_R,newvec_en_R;
		for(int i=0;i<Img_with_Pad[0].size();i++){
			vector<double > newvec_st(Img_with_Pad[0][0].size(),0);
			vector<double > newvec_en(Img_with_Pad[0][0].size(),0);
			/* for(int j=0;j<Img_with_Pad[0][0].size();j++){
			   newvec_st.push_back(Img_with_Pad[1][i][j]);
			   newvec_en.push_back(Img_with_Pad[Img_with_Pad.size()-2][i][j]);
			   }*/
			newvec_st_R.push_back(newvec_st);
			newvec_en_R.push_back(newvec_en);
		}
		Img_with_Pad.insert(Img_with_Pad.begin(),newvec_st_R);
		Img_with_Pad.push_back(newvec_en_R);
		//Padding is done. Convolution 
	}
	LOOP_THROUGH_IMG     
		Img_with_Pad[i+padding_sz][j+padding_sz][k]= Img[i][j][k];
	END_LOOP_THROUGH_IMG

		for(int i=0;i<(Img_with_Pad.size()-padding_sz);i++){ 
			//     if(i<2){
			// cout<<endl;
			//     }

			for(int j=0;j<(Img_with_Pad[0].size()-padding_sz);j++){ 
				int n = j;
				if(i%2==1){
					n = Img_with_Pad[0].size()-padding_sz - j-1;
				}
				// if(i<2){
				//   cout<<n<< " ";
				// }

				if((i-(padding_sz)>=0) && (n-(padding_sz)>=0)){
					for(int k = 0; k<Img_with_Pad[0][0].size();k++){
						double before = Img_with_Pad[i][n][k];
						double error_val=Img_with_Pad[i][n][k] - ( Img_with_Pad[i][n][k]>127?255:0);
						Img_with_Pad[i][n][k] = Img_with_Pad[i][n][k]>127?255:0;
						for(int l=0;l<mask.size();l++){
							for(int m=0;m<mask.size();m++){
								int o = m;
								if(i%2==1){
									o = mask[0].size() - m-1;
								}

								Img_with_Pad[i + l - (padding_sz)][n + m - (padding_sz)][k] += mask[l][o] * error_val;

							}

						}
						op_img[i - padding_sz][n - padding_sz][k] = Img_with_Pad[i][n][k];
					}   
				}

			}
		}




	return ImageVec(op_img);
  }

/*ImageVec ImageVec::shape_transform(vector<ImgMat> transformMat) { 
  vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(Img[0][0].size())));
  double rows = Img.size();
  LOOP_THROUGH_IMG
      
      vector< vector<double>> op_loc(2, vector<double> (1));
    op_loc[0][0] = (j);
    op_loc[1][0] = CART(i);
    ImgMat op_Mat = ImgMat(op_loc);
    op_Mat.sixelemextend();
    ImgMat ip_xy(2,1);
    
    if(j>i){
      ip_xy = transformMat[0].MatMul(op_Mat);
     
    }
    else if((i+j)<Img.size()){
      ip_xy = transformMat[1].MatMul(op_Mat);
      
     
    }
    else{
      ip_xy = transformMat[2].MatMul(op_Mat);

    }
    
    double x_val = ip_xy.Mat[0][0];
    double y_val = CART(ip_xy.Mat[1][0]);
    op_img[i][j][k] = this->bilinear_interpolate(y_val,x_val,k);




  END_LOOP_THROUGH_IMG
  return ImageVec(op_img);


}
*/
/*ImageVec ImageVec::homography_transform(ImgMat transformMat) { 
  vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(Img[0][0].size())));
  double rows = Img.size();
  LOOP_THROUGH_IMG
      
      vector< vector<double>> op_loc(3, vector<double> (1));
    op_loc[0][0] = (j);
    op_loc[1][0] = CART(i);
    op_loc[2][0] = 1;
    ImgMat op_Mat = ImgMat(op_loc);

    ImgMat ip_xy(3,1);
    ip_xy = transformMat.MatMul(op_Mat); 
    double x_val = ip_xy.Mat[0][0]/ip_xy.Mat[2][0];
    double y_val = CART(ip_xy.Mat[1][0]/ip_xy.Mat[2][0]);
    op_img[i][j][k] = this->bilinear_interpolate(y_val,x_val,k);
  END_LOOP_THROUGH_IMG
  return ImageVec(op_img);


}*/


unsigned int ImageVec::bilinear_interpolate(double x,double y,int channel) { 
    int prev_x = x;
    int next_x = x+1;
    int prev_y = y;
    int next_y = y+1;
    double delta_x = x - int(x);
    delta_x = delta_x<0?(-1*delta_x):delta_x;
    double delta_y = y - int(y);
    delta_y = delta_y<0?(-1*delta_y):delta_y;
    
    unsigned int top_left = (prev_x>-1 && prev_y>-1 && prev_x<this->rows && prev_y<this->cols)?(this->Img[prev_x][prev_y][channel]):0;
    unsigned int top_right = (next_x>-1 && prev_y>-1 && next_x<this->rows && prev_y<this->cols)?(this->Img[next_x][prev_y][channel]):0;
    unsigned int bottom_left = (prev_x>-1 && next_y>-1 && prev_x<this->rows && next_y<this->cols)?(this->Img[prev_x][next_y][channel]):0;
    unsigned int bottom_right = (next_x>-1 && next_y>-1 && next_x<this->rows && next_y<this->cols)?(this->Img[next_x][next_y][channel]):0;

double inter_val = (1-delta_x)*(1-delta_y)*(double(top_left)) + (delta_x)*(delta_y)*(double(bottom_right)) + (1-delta_x)*(delta_y)*(double(bottom_left)) + (delta_x)*(1-delta_y) * (double(top_right));
return inter_val;


    
    


}



vector<vector<vector<double>>> ImageVec::laws_filter_gray(vector<vector<vector<double>>> masks,int sz) { 
	int padding_sz = sz/2;
	vector<vector<vector<unsigned int >>> Img_with_Pad = Img;
	vector<vector<vector<double>>> op_img(Img.size(), vector<vector<double>>(Img[0].size(), vector<double>(masks.size())));
	/*	for(int i=0;i<mask.size();i++){
		for(int j=0;j<mask[0].size();j++){
		weight_sum+=mask[i][j];
		}
		}*/
	for(int j=0;j<padding_sz;j++){
		for(int i=0;i<Img_with_Pad.size();i++){
			vector<unsigned int> newvec_st;
			vector<unsigned int> newvec_en;
			for(int k=0;k<Img_with_Pad[0][0].size();k++){
				newvec_st.push_back(Img_with_Pad[i][1+(2*j)][k]);
				newvec_en.push_back(Img_with_Pad[i][Img_with_Pad[i].size()-2-(2*j)][k]);
			}
			Img_with_Pad[i].insert(Img_with_Pad[i].begin(),newvec_st);
			Img_with_Pad[i].push_back(newvec_en);
		}
	}
	for(int t=0;t<padding_sz;t++){
		vector<vector<unsigned int >> newvec_st_R,newvec_en_R;
		for(int i=0;i<Img_with_Pad[0].size();i++){
			vector<unsigned int> newvec_st;
			vector<unsigned int> newvec_en;
			for(int j=0;j<Img_with_Pad[0][0].size();j++){
				newvec_st.push_back(Img_with_Pad[1+(2*t)][i][j]);
				newvec_en.push_back(Img_with_Pad[Img_with_Pad.size()-2-(2*t)][i][j]);
			}
			newvec_st_R.push_back(newvec_st);
			newvec_en_R.push_back(newvec_en);
		}
		Img_with_Pad.insert(Img_with_Pad.begin(),newvec_st_R);
		Img_with_Pad.push_back(newvec_en_R);
	}
	ImageVec test_Img = ImageVec(Img_with_Pad);
	for(int i=0;i<(Img_with_Pad.size()-padding_sz);i++){ 
		for(int j=0;j<(Img_with_Pad[0].size()-padding_sz);j++){ 
			if((i-(padding_sz)>=0) && (j-(padding_sz)>=0)){
				for(int n = 0;n<masks.size();n++){
					for(int k = 0;k<Img_with_Pad[0][0].size();k++){
						double val1=0;
						double mask_sum = 0;
						for(int l=0;l<masks[0].size();l++){
							for(int m=0;m<masks[0].size();m++){
								val1 += masks[n][l][m] * Img_with_Pad[i + l - (padding_sz)][j + m - (padding_sz)][k];
								mask_sum+=masks[n][l][m];
							}
						}
						if(!mask_sum){
							op_img[i - padding_sz][j - padding_sz][n] = (val1);
						}
						else{

							op_img[i - padding_sz][j - padding_sz][n] = (val1);
							//op_img[i - padding_sz][j - padding_sz][n] = (val1)/mask_sum;
						}

					}
				}
			}
		}
	}
	return (op_img);
  }




