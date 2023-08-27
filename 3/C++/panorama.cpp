

#include <opencv2/opencv.hpp>

#include <iostream>
#include "ImageVec.h"
#include "ImgMat.h"

using namespace std;
using namespace cv;
double rows = 0, cols = 0;

Mat rawtoOCV_Mat(ImageVec newImg)
{
    Mat retval(newImg.rows, newImg.cols, CV_8UC3);
    vector<vector<vector<unsigned int>>> Img = newImg.Img;
    LOOP_THROUGH_ROW_COL
    Vec3b writeval;
    writeval[2] = newImg.Img[i][j][0];
    writeval[1] = newImg.Img[i][j][1];
    writeval[0] = newImg.Img[i][j][2];
    retval.at<Vec3b>(i, j) = writeval;
    END_LOOP_THROUGH_ROW_COL

    return retval;
}

ImageVec CanvasCreate(ImageVec Img1, ImageVec Img2, ImageVec Img3, int retcanvas)
{
    ImageVec left = Img1, mid = Img2, right = Img3;
    ImageVec OpCanvas = ImageVec(left.rows + int(left.rows / 4) * 2, left.cols * 3 + int(left.cols / 4) * 4, 3);
    ImageVec LCanvas = ImageVec(left.rows + int(left.rows / 4) * 2, left.cols * 3 + int(left.cols / 4) * 4, 3);
    ImageVec RCanvas = ImageVec(left.rows + int(left.rows / 4) * 2, left.cols * 3 + int(left.cols / 4) * 4, 3);
    ImageVec CCanvas = ImageVec(left.rows + int(left.rows / 4) * 2, left.cols * 3 + int(left.cols / 4) * 4, 3);
    cout<<"Rows = "<<left.rows + int(left.rows / 4) * 2<<" Cols = "<<left.cols * 3 + int(left.cols / 4) * 4<<endl;
    int row_st_index = left.rows / 4;
    vector<int> col_st_index = {(left.cols / 4), (left.cols / 2) + left.cols, ((3 * left.cols) / 4) + 2 * left.cols};
    for (int i = 0; i < left.rows; i++)
    {
        for (int j = 0; j < left.cols; j++)
        {
            for (int k = 0; k < 3; k++)
            {

                ImageVec *Iter_ = &Img1;
                if (k == 1)
                {
                    Iter_ = &Img2;
                }
                if (k == 2)
                {
                    Iter_ = &Img3;
                }
                int col_start = col_st_index[k];
                if (retcanvas == 0)
                {
                    OpCanvas.Img[row_st_index + i][col_start + j][0] = Iter_->Img[i][j][0];
                    OpCanvas.Img[row_st_index + i][col_start + j][1] = Iter_->Img[i][j][1];
                    OpCanvas.Img[row_st_index + i][col_start + j][2] = Iter_->Img[i][j][2];
                }
                if (k == 0 && retcanvas == 1)
                {
                    LCanvas.Img[row_st_index + i][col_start + j][0] = Iter_->Img[i][j][0];
                    LCanvas.Img[row_st_index + i][col_start + j][1] = Iter_->Img[i][j][1];
                    LCanvas.Img[row_st_index + i][col_start + j][2] = Iter_->Img[i][j][2];
                }
                else if (k == 1 && retcanvas == 2)
                {
                    CCanvas.Img[row_st_index + i][col_start + j][0] = Iter_->Img[i][j][0];
                    CCanvas.Img[row_st_index + i][col_start + j][1] = Iter_->Img[i][j][1];
                    CCanvas.Img[row_st_index + i][col_start + j][2] = Iter_->Img[i][j][2];
                }
                else if (k == 2 && retcanvas == 3)
                {
                    RCanvas.Img[row_st_index + i][col_start + j][0] = Iter_->Img[i][j][0];
                    RCanvas.Img[row_st_index + i][col_start + j][1] = Iter_->Img[i][j][1];
                    RCanvas.Img[row_st_index + i][col_start + j][2] = Iter_->Img[i][j][2];
                }
                else
                {
                }
            }
        }
    }

    if (retcanvas == 0)
        return OpCanvas;
    if (retcanvas == 1)
        return LCanvas;
    if (retcanvas == 2)
        return CCanvas;
    if (retcanvas == 3)
    {
        cout << "Ever Here?" << endl;
        return RCanvas;
    }

    return OpCanvas;
}

ImgMat h_calc(vector<vector<int>> xyxdyd, vector<int> l_start, vector<int> r_start)
{

    vector<vector<double>> retval(9, vector<double>(9));
    double old_rows = rows;
    rows = rows + (rows / 2);
    for (int i = 0; i < 4; i++)
    {
        cout<<"Init value = "<<xyxdyd[0][2 * i]<<" added with "<<l_start[0]<<endl;
        cout<<"Init y value = "<<xyxdyd[1][2 * i]<<" added with "<<l_start[1]<<endl;
        double x1 = int((xyxdyd[0][2 * i]) + l_start[0]);
        double y1 = int(CART(xyxdyd[1][2 * i] + l_start[1]));
        double x2 = int(xyxdyd[0][(2 * i) + 1] + r_start[0]);
        double y2 = int(CART(xyxdyd[1][(2 * i) + 1] + r_start[1]));

        // H values
        retval[2 * i] = {-1 * x1, -1 * y1, -1, 0, 0, 0, x1 * x2, y1 * x2, x2};
        retval[(2 * i) + 1] = {0, 0, 0, -1 * x1, -1 * y1, -1, x1 * y2, y1 * y2, y2};
    }
    retval[8] = {0, 0, 0, 0, 0, 0, 0, 0, 1};
    vector<vector<double>> bmat(9, vector<double>(1));
    for (int i = 0; i < 8; i++)
    {
        bmat[i][0] = 0;
    }
    retval[8] = {0, 0, 0, 0, 0, 0, 0, 0, 1};
    bmat[8][0] = 1;
    ImgMat lhs_mat = ImgMat(retval);
    lhs_mat.printval();

    ImgMat bm = ImgMat(bmat);
    ImgMat op = lhs_mat.cv_solve(bm);

    rows = old_rows;

    return op;
}

vector<vector<int>> ocv_calculation(ImageVec FirstImg, ImageVec SecondImg, int match)
{

    Mat img1 = rawtoOCV_Mat(FirstImg);
    Mat img2 = rawtoOCV_Mat(SecondImg);

    // Convert to grayscale
    Mat gray1, gray2;
    cvtColor(img1, gray1, COLOR_BGR2GRAY);
    cvtColor(img2, gray2, COLOR_BGR2GRAY);
    Ptr<SIFT> sift = SIFT::create();

    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;

    sift->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
    sift->detectAndCompute(img2, noArray(), keypoints2, descriptors2);

    BFMatcher matcher(NORM_L2);
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    Mat img_matches;

    vector<vector<DMatch>> knn_matches;
    matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);

    int vec_val = 0;
    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.7f;
    std::vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);

        }
    }

    //     std::vector<DMatch> final_matches = {good_matches[122],good_matches[60],good_matches[110],good_matches[360]}; Left and centre
    std::vector<DMatch> final_matches;
    // Filter matched points in top half of image
    int height = img1.rows, a = 0;
    vector<Point2f> pts1, pts2;
    for (size_t i = 0; i < good_matches.size(); i++)
    {
        Point2f pt1 = keypoints1[good_matches[i].queryIdx].pt;
        Point2f pt2 = keypoints2[good_matches[i].trainIdx].pt;

        if (pt1.y < 5 * height / 10 && pt2.y < 5 * height / 10)
        {

            pts1.push_back(pt1);
            pts2.push_back(pt2);

            final_matches.push_back(good_matches[i]);
            /*if(match){
                drawMatches( img1, keypoints1, img2, keypoints2, {good_matches[i]}, img_matches, Scalar::all(-1),
            Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
            String title = "Matches_" + to_string(i);
         imshow(title, img_matches);
     waitKey(0);
     destroyAllWindows();

            }*/
            
            
        }
/*        if(a < 2){
            if(!match){
                drawMatches( img1, keypoints1, img2, keypoints2, {good_matches[122], good_matches[60], good_matches[110], good_matches[360]}, img_matches, Scalar::all(-1),
                Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
                String title = "Matches";
                imshow(title, img_matches);
                waitKey(0);
                destroyAllWindows();
            } 
            else{
                    drawMatches( img1, keypoints1, img2, keypoints2, {good_matches[130], good_matches[9], good_matches[85], good_matches[152]}, img_matches, Scalar::all(-1),
                Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
                String title = "Matches";
                imshow(title, img_matches);
                waitKey(0);
                destroyAllWindows();
            }
            a ++;
        }*/
    }

    if (!match)
    {
        final_matches = {good_matches[122], good_matches[60], good_matches[110], good_matches[360]};
        // final_matches = {good_matches[122],good_matches[60],good_matches[110],good_matches[360]};
    }
    else
    {
        final_matches = {good_matches[130], good_matches[9], good_matches[85], good_matches[50]};
    }
    vector<vector<int>> retval(2, vector<int>(8));

    for (size_t i = 0; i < final_matches.size(); i++)
    {
        Point2f pt2f1 = keypoints1[final_matches[i].queryIdx].pt;
        Point2f pt2f2 = keypoints2[final_matches[i].trainIdx].pt;
            retval[0][2 * i] = pt2f1.x;
            retval[1][2 * i] = pt2f1.y;
            retval[0][(2 * i) + 1] = pt2f2.x;
            retval[1][(2 * i) + 1] = pt2f2.y;


 
    }
    return retval;

    // Draw matched keypoints on image

    // Display image
}
ImageVec Combine_Op(ImageVec Left,ImageVec Mid,ImageVec Right){
    ImageVec retval(Mid);
    vector<vector<vector<unsigned int>>> Img = Left.Img;
    LOOP_THROUGH_IMG
        double left_val = Left.Img[i][j][k];
        double mid_val = Mid.Img[i][j][k];
        double right_val = Right.Img[i][j][k];
        int count = 0;
        if(left_val)
            count++;
        if(mid_val)
            count++;
        if(right_val)
            count++;
        retval.Img[i][j][k] = count!=0?(left_val+mid_val+right_val)/count:0;
    END_LOOP_THROUGH_IMG
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

    ImageVec LeftImg(argv[1], atoi(argv[6]), atoi(argv[5]), C_RGB);
    ImageVec CentreImg(argv[2], atoi(argv[6]), atoi(argv[5]), C_RGB);
    ImageVec RightImg(argv[3], atoi(argv[6]), atoi(argv[5]), C_RGB);
    rows = atoi(argv[6]);
    cols = atoi(argv[5]);

    ImageVec CanvasIp = CanvasCreate(LeftImg, CentreImg, RightImg, 0);
    ImageVec LCanvasIp = CanvasCreate(LeftImg, CentreImg, RightImg, 1);
    ImageVec CCanvasIp = CanvasCreate(LeftImg, CentreImg, RightImg, 2);
    ImageVec RCanvasIp = CanvasCreate(LeftImg, CentreImg, RightImg, 3);
   /* CanvasIp.writeToFile("Total.raw");
    CCanvasIp.writeToFile("Mid.raw");
    RCanvasIp.writeToFile("Right.raw");*/
    vector<vector<int>> lc_pts = ocv_calculation(LeftImg, CentreImg, 0);
    vector<vector<int>> cr_pts = ocv_calculation(RightImg, CentreImg, 1);
    vector<int> left_start = {int(cols / 4), int(rows / 4)};
    vector<int> mid_start = {int((cols / 2) + cols), int(rows / 4)};
    vector<int> right_start = {int(((3 * cols) / 4) + 2 * cols), int(rows / 4)};
    ImgMat h_left = h_calc(lc_pts, left_start, mid_start);
    ImgMat h_right = h_calc(cr_pts, right_start, mid_start);
    ImgMat h_left_inv = h_left.invertMat();
    ImgMat h_right_inv = h_right.invertMat();
    ImageVec LCanvasOp = LCanvasIp.homography_transform(h_left_inv);
    ImageVec RCanvasOp = RCanvasIp.homography_transform(h_right_inv);
  /*  LCanvasOp.writeToFile("Left_op.raw");
    RCanvasOp.writeToFile("Right_op.raw");*/
    ImageVec output = Combine_Op(LCanvasOp,CCanvasIp,RCanvasOp);
    output.writeToFile(argv[4]);

    return 0;
}
