
#include <opencv2/opencv.hpp>
#include <iostream>
#include "ImageVec.h"
#include "ImgMat.h"

using namespace std;
using namespace cv;

vector< vector<double>> uvextend(vector< vector<double>> initval){
vector< vector<double>> retval(6, vector<double> (initval[0].size()));
for(int i=0;i<initval[0].size();i++){
    double x_val = initval[0][i];
    double y_val = initval[1][i];

    retval[0][i] = 1;
    retval[1][i] = x_val;
    retval[2][i] = y_val;
    retval[3][i] = x_val*x_val;
    retval[4][i] = x_val*y_val;
    retval[5][i] = y_val*y_val;

}

return retval;
}


int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw Width Height" << endl;
        return 0;
    }

    ImageVec newImg(argv[1], atoi(argv[5]), atoi(argv[4]), C_RGB);
    
    double rows = atoi(argv[5]);
    double cols = atoi(argv[4]);


    //--------------------------------------_TOP RIGHT--------------------------------------------------------
    //top Right Triangle u,v
    vector< vector<double>> new_Vec(2, vector<double> (6));
    new_Vec[0][0] = 0;
    new_Vec[1][0] = CART(0);
    new_Vec[1][1] = int(CART((rows-1)/4));
    new_Vec[0][1] = int((cols-1)/4);
    new_Vec[1][2] = int(CART((rows-1)/2));
    new_Vec[0][2] = int((cols-1)/2);
    new_Vec[1][3] = int(CART(((3*(rows-1))/4)));
    new_Vec[0][3] = int((3*(cols-1))/4);
    new_Vec[1][4] = int(CART(rows-1));
    new_Vec[0][4] = cols-1;

    new_Vec[0][5] = int(((cols-1)/sqrt(2)));
    new_Vec[1][5] = int(CART((rows - 1) -((rows-1)/sqrt(2))));
    double test1 = new_Vec[0][5],test2 = new_Vec[1][5];
    
    
    ImgMat orig = ImgMat(new_Vec);
    orig.sixelemextend();
    orig.printval();
    
    ImgMat matval = orig.invertMat();
    matval.printval();

    //top Triangle x,y    
    vector< vector<double>> xy_vec(2, vector<double> (6));
    xy_vec[0][0] = 0;
    xy_vec[1][0] = CART(0);
    xy_vec[1][1] = int(CART((rows-1)/4));
    xy_vec[0][1] = int((cols-1)/4);
    xy_vec[1][2] = int(CART((rows-1)/2));
    xy_vec[0][2] = int((cols-1)/2);
    xy_vec[1][3] = int(CART((3*(rows-1))/4));
    xy_vec[0][3] = int((3*(cols-1))/4);
    xy_vec[1][4] = int(CART(rows-1));
    xy_vec[0][4] = int(cols-1);

    xy_vec[0][5] = cols-1;
    xy_vec[1][5] = CART(0);
    ImgMat xyMat(xy_vec);
    xyMat.printval();
    
    ImgMat finalMat = xyMat.MatMul(matval);
    
    vector< vector<double>> testvec(2, vector<double> (1));
    testvec[0][0] = 231;
    testvec[1][0] = 95;
    
    ImgMat tM = ImgMat(testvec);
    tM.sixelemextend();
    
    ImgMat testVal = finalMat.MatMul(tM);
    
    //--------------------------------------_TOP LEFT--------------------------------------------------------
//Top Left Triangle u,v
    vector< vector<double>> tl_uv_vec(2, vector<double> (6));
    tl_uv_vec[0][0] = 0;
    tl_uv_vec[1][0] = CART(0);
    tl_uv_vec[1][1] = CART((rows-1)/4);
    tl_uv_vec[0][1] = (cols-1)/4;
    tl_uv_vec[1][2] = CART((rows-1)/2);
    tl_uv_vec[0][2] = (cols-1)/2;
    tl_uv_vec[1][3] = CART(((3*(rows-1))/4));
    tl_uv_vec[0][3] = (cols-1)/4;
    tl_uv_vec[1][4] = CART(rows-1);
    tl_uv_vec[0][4] = 0;
    tl_uv_vec[1][5] = CART((rows-1)/2);
    tl_uv_vec[0][5] = 64;
    double test1_tl = tl_uv_vec[0][5],test2_tl = tl_uv_vec[1][5];
    
     ImgMat orig_tl_uv = ImgMat(tl_uv_vec);
     orig_tl_uv.sixelemextend();
     
    
    ImgMat invert_tl_uv = orig_tl_uv.invertMat();

     //top Left Triangle x,y    
    vector< vector<double>> tl_xy_vec(2, vector<double> (6));
    tl_xy_vec[0][0] = 0;
    tl_xy_vec[1][0] = CART(0);
    tl_xy_vec[1][1] = CART((rows-1)/4);
    tl_xy_vec[0][1] = (cols-1)/4;
    tl_xy_vec[1][2] = CART((rows-1)/2);
    tl_xy_vec[0][2] = (cols-1)/2;
    tl_xy_vec[1][3] = CART((3*(rows-1))/4);
    tl_xy_vec[0][3] =(cols-1)/4;
    tl_xy_vec[1][4] = CART(rows-1);
    tl_xy_vec[0][4] = 0;

    tl_xy_vec[0][5] = 0;
    tl_xy_vec[1][5] = CART(((rows-1)/2));
    ImgMat tl_xyMat(tl_xy_vec);
    
    ImgMat tl_finalMat = tl_xyMat.MatMul(invert_tl_uv);
    vector< vector<double>> testvec_tl(2, vector<double> (1));
    testvec_tl[0][0] = test1_tl;
    testvec_tl[1][0] = test2_tl;
    ImgMat tM_tl = ImgMat(testvec_tl);
    tM_tl.sixelemextend();
    ImgMat testVal_tl = tl_finalMat.MatMul(tM_tl);



    //--------------------------------------BOTTOM LEFT--------------------------------------------------------
//Bottom Left Triangle u,v
    vector< vector<double>> bl_uv_vec(2, vector<double> (6));
    bl_uv_vec[0][0] = 0;
    bl_uv_vec[1][0] = CART(rows-1);
    bl_uv_vec[1][1] = CART(3*(rows-1)/4);
    bl_uv_vec[0][1] = (cols-1)/4;
    bl_uv_vec[1][2] = CART((rows-1)/2);
    bl_uv_vec[0][2] = (cols-1)/2;
    bl_uv_vec[1][3] = CART(((3*(rows-1))/4));
    bl_uv_vec[0][3] = ((3*(cols-1))/4);
    bl_uv_vec[1][4] = CART(rows-1);
    bl_uv_vec[0][4] = cols-1;
    bl_uv_vec[1][5] = CART(rows-1-64);
    bl_uv_vec[0][5] = (cols-1)/2;
    double test1_bl = bl_uv_vec[0][5],test2_bl = bl_uv_vec[1][5];
    
     ImgMat orig_bl_uv = ImgMat(bl_uv_vec);
     orig_bl_uv.sixelemextend();
   
    ImgMat invert_bl_uv = orig_bl_uv.invertMat();

     //top Left Triangle x,y    
    vector< vector<double>> bl_xy_vec(2, vector<double> (6));
    bl_xy_vec[0][0] = 0;
    bl_xy_vec[1][0] = CART(rows-1);
    bl_xy_vec[1][1] = CART(3*(rows-1)/4);
    bl_xy_vec[0][1] = (cols-1)/4;
    bl_xy_vec[1][2] = CART((rows-1)/2);
    bl_xy_vec[0][2] = (cols-1)/2;
    bl_xy_vec[1][3] = CART((3*(rows-1))/4);
    bl_xy_vec[0][3] = ((3*(cols-1))/4);
    bl_xy_vec[1][4] = CART(rows-1);
    bl_xy_vec[0][4] = cols-1;

    bl_xy_vec[0][5] = (cols-1)/2;
    bl_xy_vec[1][5] = CART(rows-1);
    ImgMat bl_xyMat(bl_xy_vec);
    
    ImgMat bl_finalMat = bl_xyMat.MatMul(invert_bl_uv);
    vector< vector<double>> testvec_bl(2, vector<double> (1));
    testvec_bl[0][0] = test1_bl;
    testvec_bl[1][0] = test2_bl;
    ImgMat tM_bl = ImgMat(testvec_bl);
    tM_bl.sixelemextend();
    ImgMat testVal_bl = bl_finalMat.MatMul(tM_bl);
    
    vector<ImgMat> transform_mats;
    transform_mats.push_back(finalMat);
    finalMat.printval();
    transform_mats.push_back(tl_finalMat);
    transform_mats.push_back(bl_finalMat);
    ImageVec retval = newImg.shape_transform(transform_mats);
    retval.writeToFile(argv[2]);


    //---------------------------------------------------------------------Reverse Mapping---------------------------------------------------------------

    //--------------------------------------_TOP RIGHT--------------------------------------------------------
    //top Right Triangle u,v
    vector< vector<double>> tr_op_vec(2, vector<double> (6));
    tr_op_vec[0][0] = 0;
    tr_op_vec[1][0] = CART(0);
    tr_op_vec[1][1] = int(CART((rows-1)/4));
    tr_op_vec[0][1] = int((cols-1));
    tr_op_vec[1][2] = int(CART((rows-1)/2));
    tr_op_vec[0][2] = int((cols-1)/2);
    tr_op_vec[1][3] = int(CART((0)));
    tr_op_vec[0][3] = int((3*(cols-1))/4);
    tr_op_vec[1][4] = int(CART(rows-1));
    tr_op_vec[0][4] = cols-1;
    tr_op_vec[0][5] = cols-1;
    tr_op_vec[1][5] = CART(0);
   
/*    tr_op_vec[1][1] = int(CART((rows-1)/4));
    tr_op_vec[0][1] = int((cols-1)/4);
    tr_op_vec[1][2] = int(CART((rows-1)/2));
    tr_op_vec[0][2] = int((cols-1)/2);
    tr_op_vec[1][3] = int(CART(((3*(rows-1))/4)));
    tr_op_vec[0][3] = int((3*(cols-1))/4);
 */   
    
    ImgMat op_tr = ImgMat(tr_op_vec);
    op_tr.sixelemextend();
    op_tr.printval();
    
    ImgMat op_tr_inv = op_tr.invertMat();
    op_tr_inv.printval();

    //top Triangle x,y    
    vector< vector<double>> tr_xy_vec(2, vector<double> (6));
    tr_xy_vec[0][0] = 0;
    tr_xy_vec[1][0] = CART(0);
    tr_xy_vec[1][1] = int(CART((rows - 1) -((rows-1)/(2))));
    tr_xy_vec[0][1] = int((sqrt(3)*(cols-1))/2);
    tr_xy_vec[1][2] = int(CART((rows-1)/2));
    tr_xy_vec[0][2] = int((cols-1)/2);
    tr_xy_vec[1][3] = int(CART((rows - 1) -((rows-1)*sqrt(3)/2)));
    tr_xy_vec[0][3] = int(((cols-1))/2);
    tr_xy_vec[1][4] = int(CART(rows-1));
    tr_xy_vec[0][4] = int(cols-1);
 tr_xy_vec[0][5] = int(((cols-1)/sqrt(2)));
    tr_xy_vec[1][5] = int(CART((rows - 1) -((rows-1)/sqrt(2))));
    
/*    tr_xy_vec[1][1] = int(CART((rows-1)/4));
    tr_xy_vec[0][1] = int((cols-1)/4);
    tr_xy_vec[1][2] = int(CART((rows-1)/2));
    tr_xy_vec[0][2] = int((cols-1)/2);
    tr_xy_vec[1][3] = int(CART(((3*(rows-1))/4)));
    tr_xy_vec[0][3] = int((3*(cols-1))/4);*/
    ImgMat tr_op_xyMat(tr_xy_vec);
    
    
    ImgMat tr_op_finalMat = tr_op_xyMat.MatMul(op_tr_inv);
    
    
 //--------------------------------------_TOP LEFT--------------------------------------------------------
//Top Left Triangle u,v
    vector< vector<double>> op_tl_uv_vec(2, vector<double> (6));
    op_tl_uv_vec[0][0] = 0;
    op_tl_uv_vec[1][0] = CART(0);
    op_tl_uv_vec[1][1] = CART((rows-1)/4);
    op_tl_uv_vec[0][1] = (cols-1)/4;
    op_tl_uv_vec[1][2] = CART((rows-1)/2);
    op_tl_uv_vec[0][2] = (cols-1)/2;
    op_tl_uv_vec[1][3] = CART(((3*(rows-1))/4));
    op_tl_uv_vec[0][3] = (cols-1)/4;
    op_tl_uv_vec[1][4] = CART(rows-1);
    op_tl_uv_vec[0][4] = 0;
    op_tl_uv_vec[0][5] = 0;
    op_tl_uv_vec[1][5] = CART(((rows-1)/2));
    
    
     ImgMat op_tl_uv_mat = ImgMat(op_tl_uv_vec);
     op_tl_uv_mat.sixelemextend();
     
    
    ImgMat op_tl_uv_mat_inv = op_tl_uv_mat.invertMat();

     //top Left Triangle x,y    
    vector< vector<double>> op_tl_xy_vec(2, vector<double> (6));
    op_tl_xy_vec[0][0] = 0;
    op_tl_xy_vec[1][0] = CART(0);
    op_tl_xy_vec[1][1] = CART((rows-1)/4);
    op_tl_xy_vec[0][1] = (cols-1)/4;
    op_tl_xy_vec[1][2] = CART((rows-1)/2);
    op_tl_xy_vec[0][2] = (cols-1)/2;
    op_tl_xy_vec[1][3] = CART((3*(rows-1))/4);
    op_tl_xy_vec[0][3] =(cols-1)/4;
    op_tl_xy_vec[1][4] = CART(rows-1);
    op_tl_xy_vec[0][4] = 0;
op_tl_xy_vec[1][5] = CART((rows-1)/2);
    op_tl_xy_vec[0][5] = 64;
    
    ImgMat tl_op_xyMat(op_tl_xy_vec);
    
    ImgMat tl_op_finalMat = tl_op_xyMat.MatMul(op_tl_uv_mat_inv);
     //--------------------------------------BOTTOM LEFT--------------------------------------------------------
//Bottom Left Triangle u,v
    vector< vector<double>> bl_op_uv_vec(2, vector<double> (6));
    bl_op_uv_vec[0][0] = 0;
    bl_op_uv_vec[1][0] = CART(rows-1);
    bl_op_uv_vec[1][1] = CART(3*(rows-1)/4);
    bl_op_uv_vec[0][1] = (cols-1)/4;
    bl_op_uv_vec[1][2] = CART((rows-1)/2);
    bl_op_uv_vec[0][2] = (cols-1)/2;
    bl_op_uv_vec[1][3] = CART(((3*(rows-1))/4));
    bl_op_uv_vec[0][3] = ((3*(cols-1))/4);
    bl_op_uv_vec[1][4] = CART(rows-1);
    bl_op_uv_vec[0][4] = cols-1;
   bl_op_uv_vec[0][5] = (cols-1)/2;
    bl_op_uv_vec[1][5] = CART(rows-1);
    
    
     ImgMat op_bl_uv = ImgMat(bl_op_uv_vec);
     op_bl_uv.sixelemextend();
   
    ImgMat op_bl_uv_inv = op_bl_uv.invertMat();

     //top Left Triangle x,y    
    vector< vector<double>> bl_op_xy_vec(2, vector<double> (6));
    bl_op_xy_vec[0][0] = 0;
    bl_op_xy_vec[1][0] = CART(rows-1);
    bl_op_xy_vec[1][1] = CART(3*(rows-1)/4);
    bl_op_xy_vec[0][1] = (cols-1)/4;
    bl_op_xy_vec[1][2] = CART((rows-1)/2);
    bl_op_xy_vec[0][2] = (cols-1)/2;
    bl_op_xy_vec[1][3] = CART((3*(rows-1))/4);
    bl_op_xy_vec[0][3] = ((3*(cols-1))/4);
    bl_op_xy_vec[1][4] = CART(rows-1);
    bl_op_xy_vec[0][4] = cols-1;
    bl_op_xy_vec[1][5] = CART(rows-1-64);
    bl_op_xy_vec[0][5] = (cols-1)/2;
    
    ImgMat bl_op_xymat(bl_op_xy_vec);
    ImgMat bl_op_finalMat = bl_op_xymat.MatMul(op_bl_uv_inv);
    
    
    vector<ImgMat> transform_op_mats;
    transform_op_mats.push_back(tr_op_finalMat);
    transform_op_mats.push_back(tl_op_finalMat);
    transform_op_mats.push_back(bl_op_finalMat);
    ImageVec op_retval = retval.shape_transform(transform_op_mats);
    op_retval.writeToFile(argv[3]);
    return 0;
}
