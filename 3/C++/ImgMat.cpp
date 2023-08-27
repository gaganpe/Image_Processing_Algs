


#include <opencv2/opencv.hpp>
#include <iostream>
#include "ImgMat.h"

using namespace cv;

ImgMat ImgMat::invertMat (){
    
     cv::Mat cv_Mat(this->rows,this->cols, CV_64F);
     
    for(int i=0;i<this->rows;i++){
        for(int j=0;j<this->cols;j++){
             cv_Mat.at<double>(i,j) = this->Mat[i][j];
     
        }
     
    }
    
    
    
    cv::Mat cv_Matinv(this->cols, this->rows, CV_64F);
    invert(cv_Mat,cv_Matinv,DECOMP_SVD);
    vector< vector<double>> new_Vec(this->cols, vector<double> (this->rows));
    for(int i=0;i<this->cols;i++){
        for(int j=0;j<this->rows;j++){
              new_Vec[i][j] = cv_Matinv.at<double>(i,j) ;
        }
    }
    return ImgMat(new_Vec);
}

ImgMat ImgMat::cv_solve (ImgMat rval){
    
     cv::Mat cv_l_Mat(this->rows,this->cols, CV_64F);
     cv::Mat cv_r_Mat(rval.rows,rval.cols, CV_64F);
     
    for(int i=0;i<this->rows;i++){
        for(int j=0;j<this->cols;j++){
             cv_l_Mat.at<double>(i,j) = this->Mat[i][j];
        }
     
    }
    for(int i=0;i<rval.rows;i++){
        for(int j=0;j<rval.cols;j++){
             cv_r_Mat.at<double>(i,j) = rval.Mat[i][j];
        }
     
    }
    cv::Mat op;
    solve(cv_l_Mat,cv_r_Mat,op,DECOMP_NORMAL);
    vector< vector<double>> new_Vec(3, vector<double> (3));
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            
              new_Vec[i][j] = op.at<double>(j+(3*i),0) ;
        }
    }
    return ImgMat(new_Vec);
}

void ImgMat::printval(){
    cout<<"---------------------"<<endl;
    for(int i=0;i<this->rows;i++){
        for(int j=0;j<this->cols;j++){
             cout<<this->Mat[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"---------------------"<<endl;
    return;
}

ImgMat::ImgMat(vector<vector<double>> Mat){
    this->rows = Mat.size();
    this->cols = Mat[0].size();
    this->Mat.resize(this->rows, vector<double>(this->cols));
    for(int i=0;i<this->rows;i++){
        for(int j=0;j<this->cols;j++){
             this->Mat[i][j] = Mat[i][j];
        }
    }
    return;
}


ImgMat::ImgMat(int rows,int cols){
    this->Mat.resize(rows, vector<double>(cols));
    this->rows = rows;
    this->cols = cols;
    return;
}


void ImgMat::sixelemextend(){
    this->rows = 6;
    this->cols = Mat[0].size();
    vector<vector<double>> initval = this->Mat;
    this->Mat.resize(this->rows, vector<double>(this->cols));
for(int i=0;i<initval[0].size();i++){
    double x_val = initval[0][i];
    double y_val = initval[1][i];

    this->Mat[0][i] = 1;
    this->Mat[1][i] = x_val;
    this->Mat[2][i] = y_val;
    this->Mat[3][i] = x_val*x_val;
    this->Mat[4][i] = x_val*y_val;
    this->Mat[5][i] = y_val*y_val;

}

}

ImgMat ImgMat::MatMul (ImgMat second){
    vector< vector<double>> retval(this->rows, vector<double> (second.cols));
    if(this->cols!=second.rows){
        cout<<"ERROR! cannot multiply"<<endl;
        return ImgMat(retval);
    }
    for(int i=0;i<retval.size();i++){
        for(int j=0;j<retval[0].size();j++){
     
                retval[i][j]=0;
     
        }
    }
    
    for(int i=0;i<retval.size();i++){
        for(int j=0;j<retval[0].size();j++){
            for(int k=0;k<this->cols;k++){
                retval[i][j]+=(this->Mat[i][k]*second.Mat[k][j]);
            }
        }
    }
    
    return ImgMat(retval);

}
