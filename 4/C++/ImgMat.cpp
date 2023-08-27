


#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include "ImgMat.h"

using namespace cv;
using namespace cv::ml;

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


ImgMat ImgMat::dft_predict(ImgMat test_data,vector<int> train_classes,vector<int> test_classes){
	ImgMat filtered_train_mat = this->select_col_mat({6,5,0});
	ImgMat filtered_test_mat = test_data.select_col_mat({6,5,0});
	cv::Mat cv_Mat(this->rows,3, CV_64F);
	for(int i=0;i<this->rows;i++){
		for(int j=0;j<3;j++){
			cv_Mat.at<double>(i,j) = filtered_train_mat.Mat[i][j];
		}
	}
	ofstream train_file("dft_train.txt");
	ofstream test_file("dft_test.txt");
	for(int i=0;i<filtered_train_mat.rows;i++){
			train_file<<train_classes[i]<<" ";
		for(int j=0;j<filtered_train_mat.cols;j++){
			train_file<<setprecision(10)<<filtered_train_mat.Mat[i][j]<<" ";
		}
		train_file<<endl;
	}
	for(int i=0;i<filtered_test_mat.rows;i++){
			test_file<<test_classes[i]<<" ";
		for(int j=0;j<filtered_test_mat.cols;j++){
			test_file<<setprecision(10)<<filtered_test_mat.Mat[i][j]<<" ";
		}
		test_file<<endl;
	}
	train_file.close();
	test_file.close();
	cv::Mat covar, mean_mat;
	calcCovarMatrix(cv_Mat, covar, mean_mat, cv::COVAR_NORMAL | cv::COVAR_ROWS);
	vector< vector<double>> covar_vec(covar.rows, vector<double> (covar.cols));
	for(int i=0;i<covar.rows;i++){
		for(int j=0;j<covar.cols;j++){
			covar_vec[i][j] = covar.at<double>(i,j) ;
		}
	}
	ImgMat covarMatInv = ImgMat(covar_vec).invertMat();
	double accuracy = 0;
	for(int i=0;i<filtered_test_mat.rows;i++){
		vector<double> distances;
		ImgMat single_test_sample= filtered_test_mat.row_mat(i);
		for(int j=0;j<filtered_train_mat.rows;j++){
			ImgMat single_train_sample= filtered_train_mat.row_mat(j);
			ImgMat subval = single_train_sample.MatSub(single_test_sample);
			ImgMat Opdist = subval.MatMul(covarMatInv).MatMul(subval.transpose());
			distances.push_back(sqrt(Opdist.Mat[0][0]));
		}
		int index=0;
		double minval = 100000;
		for(int j=0;j<distances.size();j++){
			if(distances[j]<minval){
				index  = j;
				minval = distances[j];
			}
		}
	cout<<"Lowest distance is to "<<index<<" th sample which has the class "<<train_classes[index]<<" and the actual class is "<<test_classes[i]<<endl;
	if(train_classes[index] == test_classes[i]){
		accuracy++;
	}
	}
	cout<<"Total accuracy with dft features = " << (accuracy/filtered_test_mat.rows)<<endl;

	return filtered_train_mat;
}









ImgMat ImgMat::pca_predict(ImgMat test_data,vector<int> train_classes,vector<int> test_classes){

	cv::Mat cv_Mat(this->rows,this->cols, CV_64F);

	for(int i=0;i<this->rows;i++){
		for(int j=0;j<this->cols;j++){
			cv_Mat.at<double>(i,j) = this->Mat[i][j];

		}

	}
	cv::Mat cv_Mat_test(test_data.rows,test_data.cols, CV_64F);

	for(int i=0;i<test_data.rows;i++){
		for(int j=0;j<test_data.cols;j++){
			cv_Mat_test.at<double>(i,j) = test_data.Mat[i][j];
		}
	}
	cv::Mat mean;
	cv::reduce(cv_Mat, mean, 0, REDUCE_AVG);
	cv::Mat centered_data,centered_data_test;
	subtract(cv_Mat, repeat(mean, cv_Mat.rows, 1), centered_data);
	subtract(cv_Mat_test, repeat(mean, cv_Mat_test.rows, 1), centered_data_test);
	PCA pca(centered_data,cv::Mat(),PCA::DATA_AS_ROW,3);
	cv::Mat cv_transformed = pca.project(centered_data);
	cv::Mat cv_transformed_test = pca.project(centered_data_test);
	// Print the transformed data
	cv::Mat covar, mean_mat;
	calcCovarMatrix(cv_transformed, covar, mean_mat, cv::COVAR_NORMAL | cv::COVAR_ROWS);
	vector< vector<double>> new_Vec(this->rows, vector<double> (3));
	vector< vector<double>> covar_vec(covar.rows, vector<double> (covar.cols));
	vector< vector<double>> test_transformedVec(cv_transformed_test.rows, vector<double> (cv_transformed_test.cols));
	for(int i=0;i<this->rows;i++){
		for(int j=0;j<3;j++){
			new_Vec[i][j] = cv_transformed.at<double>(i,j) ;
		}
	}
	for(int i=0;i<covar.rows;i++){
		for(int j=0;j<covar.cols;j++){
			covar_vec[i][j] = covar.at<double>(i,j) ;
		}
	}
	for(int i=0;i<cv_transformed_test.rows;i++){
		for(int j=0;j<cv_transformed_test.cols;j++){
			test_transformedVec[i][j] = cv_transformed_test.at<double>(i,j) ;
		}
	}
	ImgMat covarMatInv = ImgMat(covar_vec).invertMat();
	ImgMat transformedMat(new_Vec);
	ImgMat test_transformedMat(test_transformedVec);

	ofstream train_file("pca_train.txt");
	ofstream test_file("pca_text.txt");
	for(int i=0;i<transformedMat.rows;i++){
			train_file<<train_classes[i]<<" ";
		for(int j=0;j<transformedMat.cols;j++){
			train_file<<setprecision(10)<<transformedMat.Mat[i][j]<<" ";
		}
		train_file<<endl;
	}
	for(int i=0;i<test_transformedMat.rows;i++){
			test_file<<test_classes[i]<<" ";
		for(int j=0;j<test_transformedMat.cols;j++){
			test_file<<setprecision(10)<<test_transformedMat.Mat[i][j]<<" ";
		}
		test_file<<endl;
	}
	train_file.close();
	test_file.close();


	double accuracy =0;
	for(int i=0;i<test_transformedMat.rows;i++){
		vector<double> distances;
		ImgMat single_test_sample= test_transformedMat.row_mat(i);
		for(int j=0;j<transformedMat.rows;j++){
			ImgMat single_train_sample= transformedMat.row_mat(j);
			ImgMat subval = single_train_sample.MatSub(single_test_sample);



			ImgMat Opdist = subval.MatMul(covarMatInv).MatMul(subval.transpose());
			distances.push_back(sqrt(Opdist.Mat[0][0]));
		}

		int index=0;
		double minval = 100000;
		for(int j=0;j<distances.size();j++){
			if(distances[j]<minval){
				index  = j;
				minval = distances[j];
			}
		}
		cout<<"Lowest distance is to "<<index<<" th sample which has the class "<<train_classes[index]<<" and the actual class is "<<test_classes[i]<<endl;
		if(train_classes[index] == test_classes[i]){
			accuracy++;
		}
	}
	cout<<"Total accuracy with pca features = " << (accuracy/test_transformedMat.rows)<<endl;
	return ImgMat(new_Vec);
}
ImgMat ImgMat::row_mat(int row){
	vector< vector<double>> new_Vec(1, vector<double> (this->cols));
	for(int i=0;i<this->cols;i++){
		new_Vec[0][i] = this->Mat[row][i];
	}
	return ImgMat(new_Vec);
}

ImgMat ImgMat::select_col_mat(vector<int> indexes){
	vector< vector<double>> new_Vec(this->rows, vector<double> (indexes.size()));
	for(int i=0;i<this->rows;i++){
		for(int j=0;j<indexes.size();j++){
			new_Vec[i][j] = this->Mat[i][indexes[j]];
		}
	}
	return ImgMat(new_Vec);
}
ImgMat ImgMat::transpose(){
	vector< vector<double>> transpose(this->cols, vector<double> (this->rows));
	for(int i=0;i<this->rows;i++){
		for(int j=0;j<this->cols;j++){
			transpose[j][i] = this->Mat[i][j];
		}
	}
	return ImgMat(transpose);
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

ImgMat ImgMat::MatSub(ImgMat second){
    vector< vector<double>> retval(this->rows, vector<double> (this->cols));
    if(this->cols!=second.cols || this->rows!=second.rows){
        cout<<"ERROR! cannot subtract!!"<<endl;
        return ImgMat(retval);
    }
    for(int i=0;i<retval.size();i++){
        for(int j=0;j<retval[0].size();j++){
     
                retval[i][j]=0;
     
        }
    }
    for(int i=0;i<retval.size();i++){
        for(int j=0;j<retval[0].size();j++){
     
                retval[i][j]=this->Mat[i][j] - second.Mat[i][j];
     
        }
    }
    return ImgMat(retval);

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
