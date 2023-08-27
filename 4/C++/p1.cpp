

#include <iostream>
#include <opencv2/opencv.hpp>
#include <iomanip>
#include "ImageVec.h"
#include <dirent.h>
#include <cstring>

using namespace std;
using namespace cv;



vector<vector<double>> tensor_prod(vector <double> onedvec1,vector <double> onedvec2);
vector<vector<vector<double>>> law_filter_gen(){
	vector <double> L5 = {1,4,6,4,1};
	vector <double> E5 = {-1,-2,0,2,1};
	vector <double> S5 = {-1,0,2,0,-1};
	vector <double> W5 = {-1,2,0,-2,1};
	vector <double> R5 = {1,-4,6,-4,1};
	vector<vector<double>> vec_2d;
	vec_2d.push_back(L5);
	vec_2d.push_back(E5);
	vec_2d.push_back(S5);
	vec_2d.push_back(W5);
	vec_2d.push_back(R5);
	vector<vector<vector<double>>> retval;
	for (int i = 0; i < vec_2d.size(); i++){
		for (int j = 0; j < vec_2d.size(); j++){
			retval.push_back(tensor_prod(vec_2d[i],vec_2d[j]));
		}
	}
	/*for (int i = 0; i < retval.size(); i++){
		for (int j = 0; j < retval[i].size(); j++){
			for (int k = 0; k < retval[i][j].size(); k++){
				cout<<retval[i][j][k]<<" ";
			}
			cout<<endl;
		}
			cout<<endl;
			cout<<endl;
	}*/
	return retval;
}




vector<vector<double>> tensor_prod(vector <double> onedvec1,vector <double> onedvec2){
	vector< vector<double>> retval(onedvec1.size(), vector<double> (onedvec2.size()));
	for (int i = 0; i < onedvec1.size(); i++){
		for (int j = 0; j < onedvec2.size(); j++){
			retval[i][j] = onedvec1[i]*onedvec2[j];
		}
	}
	return retval;
}
vector<double> avg_nrg(vector<vector<vector<double>>> Img) { 
	vector<double> retval;
	for(int k=1;k<Img[0][0].size();k++){
		double avg_energy = 0;
		LOOP_THROUGH_ROW_COL
			avg_energy+= (Img[i][j][k] * Img[i][j][k]);
		END_LOOP_THROUGH_ROW_COL
		avg_energy = avg_energy/(Img.size() * Img[0].size());
		retval.push_back(avg_energy);
	}

	return retval;
}

int main(int argc, char *argv[])
{
	if (argc < 5)
	{
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw Width Height" << endl;
		return 0;
	}


	//const char* dir_path = "/home/vm2204/Downloads/HW4/data/EE569_2023Spring_HW4_materials/train/";
	const char* dir_path = argv[1];
	string dir_path_str = dir_path;
	if (dir_path_str.back() != '/') {
		dir_path_str+='/';
	}

	DIR* dir = opendir(dir_path);
	if (dir == nullptr) {
		cout << "Failed to open directory: " <<endl;
		return 1;
	}
	vector<string> filenames;
	dirent* entry;
	while ((entry = readdir(dir)) != nullptr) {
		if (entry->d_type == DT_REG) {
			string new_str = entry->d_name;
			string substr = ".raw";
			if (new_str.size() >= substr.size() && new_str.substr(new_str.size() - substr.size()) == substr) {
				filenames.push_back(new_str);
			}
		}
	}
	closedir(dir);
	vector <double> sample_mean(24,0); 
	vector <int> classes(filenames.size(),0); 
	vector< vector<double>> sample_mean_per_class(4, vector<double> (24));
	vector< vector<double>> samples(filenames.size(), vector<double> (24));
	int sample_size = 0;
	vector<int> sample_size_per_class(4,0);
	ofstream myfile("energies_train.txt");
	for(int i=0;i<filenames.size();i++){
		ImageVec newImg(dir_path_str+filenames[i], atoi(argv[4]), atoi(argv[3]), C_GREY);
		vector<vector<vector<double>>> laws_filters = law_filter_gen();
		vector<vector<vector<double>>> op_Img = newImg.laws_filter_gray(laws_filters,5); 
		vector<double> total_nrg = avg_nrg(op_Img) ; 
		string brick_str = "brick";
		string grass_str = "grass";
		string rice_str = "rice";
		string stones_str = "stones";
		int text_class = 0;
		if (filenames[i].find(brick_str) != std::string::npos) {
			text_class = 1;
		}
		if (filenames[i].find(grass_str) != std::string::npos) {
			text_class = 2;
		}
		if (filenames[i].find(rice_str) != std::string::npos) {
			text_class = 3;
		}
		if (filenames[i].find(stones_str) != std::string::npos) {
			text_class = 4;
		}
		sample_size_per_class[text_class-1]++;
		classes[i] = text_class;
		if(myfile.is_open()){
			myfile<<text_class<<" ";
			for(int j=0;j<total_nrg.size();j++){
				
				sample_mean_per_class[text_class-1][j]+=total_nrg[j];
				sample_mean[j]+=total_nrg[j];
				samples[i][j] = total_nrg[j];
				myfile<<setprecision(10)<<total_nrg[j]<<" ";
			}
			myfile<<endl;
		}
		else{
			cout<<"File is not opened!"<<endl;
		}
				sample_size++;

	}
	for(int i=0;i<sample_mean.size();i++){
		sample_mean[i] = sample_mean[i]/sample_size;
		for(int j=0;j<sample_mean_per_class.size();j++){
			sample_mean_per_class[j][i] = sample_mean_per_class[j][i]/sample_size_per_class[j]; 
		}
	}
	vector<double> intra_class_var(24);
	vector<double> inter_class_var(24);
	for(int i=0;i<filenames.size();i++){
		for(int j=0;j<24;j++){
			intra_class_var[j] += (samples[i][j] - sample_mean_per_class[classes[i]-1][j])*(samples[i][j] - sample_mean_per_class[classes[i]-1][j]);
		}	
	}

	for(int i=0;i<24;i++){
		for(int j=0;j<4;j++){
			inter_class_var[i] += (sample_size_per_class[j]*(sample_mean[i] - sample_mean_per_class[j][i])*(sample_mean[i] - sample_mean_per_class[j][i]));
		}
	}	
	for(int i=0;i<24;i++){
			cout<<"For feature number "<<(i);
			cout<<" Discriminant Power =  "<<(inter_class_var[i]/intra_class_var[i])<<endl;
	}	
	myfile.close();

	//-------------------------------------------------Read Test Values---------------------------
	//const char* dir_path_test = "/home/vm2204/Downloads/HW4/data/EE569_2023Spring_HW4_materials/test/";
	const char* dir_path_test = argv[2];
	 dir_path_str = dir_path_test;
	if (dir_path_str.back() != '/') {
		dir_path_str+='/';
	}

	dir = opendir(dir_path_test);
	if (dir == nullptr) {
		cout << "Failed to open directory: " <<endl;
		return 1;
	}
	vector<string> filenames_test;
	while ((entry = readdir(dir)) != nullptr) {
		if (entry->d_type == DT_REG) {
			string new_str = entry->d_name;
			string substr = ".raw";
			if (new_str.size() >= substr.size() && new_str.substr(new_str.size() - substr.size()) == substr) {
				filenames_test.push_back(new_str);
			}
		}
	}
	closedir(dir);
	vector<int> test_classes = {2,4,4,1,3,2,1,3,3,1,4,2};
	vector <int> classes_test(filenames_test.size(),0); 
	vector< vector<double>> samples_test(filenames_test.size(), vector<double> (24));
	ofstream myfile_test("energies_test.txt");
	for(int i=0;i<filenames_test.size();i++){
		string just_filename = filenames_test[i];
		string removal = ".raw";
		  size_t pos = just_filename.find(removal);
		  just_filename.erase(pos, removal.length());

		ImageVec newImg(dir_path_str+filenames_test[i], atoi(argv[4]), atoi(argv[3]), C_GREY);
		int text_class = 0;
		vector<vector<vector<double>>> laws_filters = law_filter_gen();
		vector<vector<vector<double>>> op_Img = newImg.laws_filter_gray(laws_filters,5); 
		vector<double> total_nrg = avg_nrg(op_Img) ; 
		text_class = test_classes[atoi(just_filename.c_str()) - 1];
		sample_size_per_class[text_class-1]++;
		classes_test[i] = text_class;
		if(myfile_test.is_open()){
			myfile_test<<text_class<<" ";
			for(int j=0;j<total_nrg.size();j++){
				myfile_test<<setprecision(10)<<total_nrg[j]<<" ";
				samples_test[i][j] = total_nrg[j];
			}
			myfile_test<<endl;
		}
		else{
			cout<<"File is not opened!"<<endl;
		}

	}
    	ImgMat sample_mat = ImgMat(samples);
    	ImgMat sample_test_mat = ImgMat(samples_test);
	ImgMat pca_op_mat = sample_mat.pca_predict(sample_test_mat,classes,classes_test);
	ImgMat nothing = sample_mat.dft_predict(sample_test_mat,classes,classes_test);
	return 0;
}
