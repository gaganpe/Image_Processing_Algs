

#include <opencv2/opencv.hpp>
#include <iostream>
#include "ImageVec.h"

using namespace std;
using namespace cv;
ImageVec HistEqualization_Transfer_Fn(ImageVec newImg)
{
    vector<unsigned int> Histogram = newImg.HistGen();
    vector<unsigned int> mapping(256, 0);
    vector<double> cdf(256, 0);
    unsigned int data1 = 0;
    for (int i = 0; i < Histogram.size(); i++)
    {
        data1 += Histogram[i];
        cdf[i] = (i > 0 ? cdf[i - 1] : 0) + (double)Histogram[i] / ((newImg.rows) * (newImg.cols));
        mapping[i] = cdf[i] * 255;
    }
    ImageVec retval(newImg);
    retval.transfer_Img(mapping);
    return retval;
}

ImageVec HistEqualization_BF(ImageVec newImg)
{
    vector<unsigned int> Histogram = newImg.HistGen();
    unsigned int total_pixels = newImg.rows * newImg.cols;
    unsigned int pix_per_bucket = round(double(total_pixels) / 256), current_bucket = 0;
    vector<vector<unsigned int>> mapping(256);
    vector<unsigned int> remaining_pix(256, pix_per_bucket);
    remaining_pix[255] -= (pix_per_bucket * 256 - total_pixels);
    for (int i = 0; i < Histogram.size(); i++)
    {
        unsigned int pix_required = Histogram[i];
        while (pix_required > 0)
        {
            if (remaining_pix[current_bucket] < pix_required)
            {
                pix_required -= remaining_pix[current_bucket];
                mapping[i].push_back(current_bucket);
                mapping[i].push_back(remaining_pix[current_bucket]);
                remaining_pix[current_bucket++] = 0;
            }
            else
            {
                if (pix_required)
                {
                    remaining_pix[current_bucket] -= pix_required;
                    mapping[i].push_back(current_bucket);
                    mapping[i].push_back(pix_required);
                    pix_required = 0;
                }
            }
        }
    }
    ImageVec retval(newImg);
    retval.transfer_Img_BF(mapping);
    return retval;
}

int main(int argc, char *argv[])
{
    if (argc < 7)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw Width Height" << endl;
        return 0;
    }

    ImageVec newImg(argv[1], atoi(argv[4]), atoi(argv[3]), C_RGB);
    ImageVec writeImg = newImg.RGBtoGRAY();

    Mat  gray_Mat(newImg.rows, newImg.cols, CV_8UC1), final_Mat(newImg.rows, newImg.cols, CV_8UC1);

    
    
for (int i = 0; i < newImg.rows; i++)
    {
        for (int j = 0; j < newImg.cols; j++)
        {
            gray_Mat.at<uchar>(i, j) = writeImg.Img[i][j][0];
            
            // cout<<Y_mat.at<unsigned int>(i,j)<<endl;
        }
    }
    Canny(gray_Mat, final_Mat, atoi(argv[5]), atoi(argv[6]), 3);
    
    vector<vector<vector<unsigned int>>> op_Img(newImg.rows, vector<vector<unsigned int>>(newImg.cols, vector<unsigned int>(1)));
    for (int i = 0; i < newImg.rows; i++)
    {
        for (int j = 0; j < newImg.cols; j++)
        {
            op_Img[i][j][0] = final_Mat.at<uchar>(i, j) == 0?255:0;
            
        }
    }
    ImageVec finalImg(op_Img);
    finalImg.writeToFile(argv[2]);
    return 0;
}
