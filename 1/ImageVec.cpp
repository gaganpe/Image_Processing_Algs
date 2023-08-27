


#include <iostream>

#include "ImageVec.h"


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
/*
//Currently only works with 3x3 filter
//Formula for size of output matrix obtained from:
//https://jamesmccaffrey.wordpress.com/2018/05/30/convolution-image-size-filter-size-padding-and-stride/
//Implemented similar to https://www.tensorflow.org/api_docs/python/tf/keras/layers/Conv2D API- Similar Parameters used because this function maybe used later
 vector<vector<vector<unsigned int >>>  ImageVec::convolve_2d(vector<vector<double>> filter,vector <int> stride, int padding_type,vector<unsigned int> InitPos){
  
  int filt_rows = filter.size();
  int filt_cols = filter[0].size();
  int init_row = InitPos[0],init_col = InitPos[1];
  if(padding_type = ODD_PAD){
  vector<vector<vector<unsigned int >>> Img_with_Pad = Img;
  //Copy the Second and the last second Column
  for(int i=0;i<Img.size();i++){
    vector<unsigned int > newvec_st,newvec_en;
    for(int j=0;j<3;j++){
      newvec_st.push_back(Img[i][1][j]);
      newvec_en.push_back(Img[i][Img[i].size()-2][j]);
    }
    Img_with_Pad[i].insert(Img_with_Pad[i].begin(),newvec_st);
    Img_with_Pad[i].push_back(newvec_en);
  }
  //Copy the Second Row
  vector<vector<unsigned int >> newvec_st_R,newvec_en_R;
  for(int i=0;i<Img[0].size();i++){
    vector<unsigned int > newvec_st,newvec_en;
    for(int j=0;j<3;j++){
      newvec_st.push_back(Img[0][i][j]);
      newvec_en.push_back(Img[Img.size()-2][i][j]);
    }
    newvec_st_R.push_back(newvec_st);
    newvec_en_R.push_back(newvec_en);
  }
  Img_with_Pad.insert(Img_with_Pad.begin(),newvec_st_R);
  Img_with_Pad.push_back(newvec_en_R);
  //Padding is done. Convolution 
  for(int i=init_row+1;i<Img_with_Pad.size();i=i+stride[0]){ 
    for(int j=init_col+1;j<Img_with_Pad[i].size();j=j+stride[1]){ 
     for(int k=0;k<Img_with_Pad[i][j].size();k++){
      if((i-(int(filt_rows/2))>0) && (j-(int(filt_cols/2))>0)){
        unsigned int val=0;
        for(int l=0;l<filter.size();l++){
          for(int m=0;l<filter[l].size();m++){
            val += filter[l][m] * Img_with_Pad[i + l - (int(filt_rows/2))][j + m - (int(filt_cols/2))][k];
            }
          }
          Img[i - int(filt_rows/2)][j - int(filt_cols/2)][k] = val;
      }
     }
    }
  }

  }
  return Img;
}
*/
ImageVec ImageVec::demosaic() { 
  //Bilinear Interpolation Masks;
 vector<vector<double>> G_at_R{{0, 0.25, 0}, {0.25, 0, 0.25},{0, 0.25, 0}};
 vector<vector<double>> B_at_R{{0.25, 0, 0.25}, {0, 0, 0},{0.25, 0, 0.25}};
 vector<vector<double>> R_at_B{{0.25, 0, 0.25}, {0, 0, 0},{0.25, 0, 0.25}};
 vector<vector<double>> G_at_B{{0, 0.25, 0}, {0.25, 0, 0.25},{0, 0.25, 0}};
 vector<vector<double>> R_at_G_odd{{0, 0, 0}, {0.5, 0, 0.5},{0, 0, 0}};
 vector<vector<double>> R_at_G_even{{0, 0.5, 0}, {0, 0, 0},{0, 0.5, 0}};
 vector<vector<double>> B_at_G_odd{{0, 0.5, 0}, {0, 0, 0},{0, 0.5, 0}};
 vector<vector<double>> B_at_G_even{{0, 0, 0}, {0.5, 0, 0.5},{0, 0, 0}};
 vector<vector<double>> zero_vec{{0,0,0},{0,0,0},{0,0,0}};
vector<int> stride_2_2 {2,2};
vector<unsigned int> G_at_R_in {0,1};
vector<vector<vector<unsigned int >>> Img_with_Pad = Img;
vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(3)));
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
  
  for(int i=0;i<(Img_with_Pad.size()-1);i++){ 
    for(int j=0;j<(Img_with_Pad[0].size()-1);j++){ 
      vector<vector<double>> R_at_G,B_at_G;
      if((i-(int(3/2))>=0) && (j-(int(3/2))>=0)){
        double val1=0,val2=0;
        unsigned int current_pix;
        if((i%2==0)&&(j%2==0) || (i%2==1) && (j%2==1)){
          current_pix = GREEN_CH;
          if(i%2==0){
            R_at_G = R_at_G_even;
            B_at_G = B_at_G_even;
          }
          else{
            R_at_G = R_at_G_odd;
            B_at_G = B_at_G_odd;
          }
        }
        if((i%2==1)&&(j%2==0) ){
          current_pix = RED_CH;
        }
        if((i%2==0)&&(j%2==1) ){
          current_pix = BLUE_CH;
        }
        switch(current_pix){
        case GREEN_CH:
          for(int l=0;l<3;l++){
            for(int m=0;m<3;m++){
              val1 += R_at_G[l][m] * Img_with_Pad[i + l - (int(3/2))][j + m - (int(3/2))][0];
              val2 += B_at_G[l][m] * Img_with_Pad[i + l - (int(3/2))][j + m - (int(3/2))][0];
              } 
            }

            op_img[i - int(3/2)][j - int(3/2)][0] = (val1);
            op_img[i - int(3/2)][j - int(3/2)][1] = Img_with_Pad[i][j][0];
            op_img[i - int(3/2)][j - int(3/2)][2] = (val2);
            break;
        case BLUE_CH:
          for(int l=0;l<3;l++){
            for(int m=0;m<3;m++){
              val1 += R_at_B[l][m] * Img_with_Pad[i + l - (int(3/2))][j + m - (int(3/2))][0];
              val2 += G_at_B[l][m] * Img_with_Pad[i + l - (int(3/2))][j + m - (int(3/2))][0];
              } 
            }

            op_img[i - int(3/2)][j - int(3/2)][0] = (val1);
            op_img[i - int(3/2)][j - int(3/2)][1] = (val2);
            op_img[i - int(3/2)][j - int(3/2)][2] = Img_with_Pad[i][j][0];
            break;
        default:
          for(int l=0;l<3;l++){
            for(int m=0;m<3;m++){
              val1 += B_at_R[l][m] * Img_with_Pad[i + l - (int(3/2))][j + m - (int(3/2))][0];
              val2 += G_at_R[l][m] * Img_with_Pad[i + l - (int(3/2))][j + m - (int(3/2))][0];
              } 
            }
            
            op_img[i - int(3/2)][j - int(3/2)][0] = Img_with_Pad[i][j][0];
            op_img[i - int(3/2)][j - int(3/2)][1] = (val2);
            op_img[i - int(3/2)][j - int(3/2)][2] = (val1);
            break;
        }
      }
     }
    }


    return ImageVec(op_img);
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
    vector<unsigned int > newvec_st(Img_with_Pad[0][0].size(),0);
    vector<unsigned int > newvec_en(Img_with_Pad[0][0].size(),0);
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
      vector<unsigned int > newvec_st(Img_with_Pad[0][0].size(),0);
    vector<unsigned int > newvec_en(Img_with_Pad[0][0].size(),0);
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

ImageVec ImageVec::conv2d_hue(int mask_sz) { 
  //Bilinear Interpolation Masks;
int padding_sz = mask_sz/2;
vector<vector<vector<unsigned int >>> Img_with_Pad = Img;
vector<vector<vector<unsigned int>>> op_img(Img.size(), vector<vector<unsigned int>>(Img[0].size(), vector<unsigned int>(Img[0][0].size())));
  for(int i=0;i<Img_with_Pad.size();i++){
    for(int j=0;j<padding_sz;j++){
    vector<unsigned int > newvec_st(Img_with_Pad[0][0].size(),0);
    vector<unsigned int > newvec_en(Img_with_Pad[0][0].size(),0);
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
      vector<unsigned int > newvec_st(Img_with_Pad[0][0].size(),0);
    vector<unsigned int > newvec_en(Img_with_Pad[0][0].size(),0);
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
          double val1=0,val2=0;
          for(int l=0;l<mask_sz;l++){
            for(int m=0;m<mask_sz;m++){
              val1+=sin((PI_VAL*double(Img_with_Pad[i + l - (padding_sz)][j + m - (padding_sz)][k]))/180);
              val2+=cos((PI_VAL*double(Img_with_Pad[i + l - (padding_sz)][j + m - (padding_sz)][k]))/180);
              
              }
              }
              double val = ((180*atan2(val1,val2))/PI_VAL);
              val = val<0?val+360:val;
              op_img[i - padding_sz][j - padding_sz][k] = val/2; //OpenCV range is 0 to 180
            }
            
            
            
            
            }
        }
      }
     
    


    return ImageVec(op_img);
  }




