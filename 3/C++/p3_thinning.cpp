

#include <iostream>
#include "ImageVec.h"
#include <unordered_set>

using namespace std;




int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw Width Height" << endl;
        return 0;
    }

    ImageVec newImg(argv[1], atoi(argv[4]), atoi(argv[3]), C_GREY);


    //--------------------------------------Thinning LUT Matrices--------------------------------------------------------
     
    unordered_set<string> thinning_mats;
    thinning_mats.insert("010011000");
    thinning_mats.insert("010110000");
    thinning_mats.insert("000110010");
    thinning_mats.insert("000011010");
    thinning_mats.insert("001011001");
    thinning_mats.insert("111010000");
thinning_mats.insert("100110100");
thinning_mats.insert("000010111");
thinning_mats.insert("110011000");
thinning_mats.insert("010011001");
thinning_mats.insert("011110000");
thinning_mats.insert("001011010");
thinning_mats.insert("011011000");
thinning_mats.insert("110110000");
thinning_mats.insert("000110110");
thinning_mats.insert("000011011");
thinning_mats.insert("110011001");
thinning_mats.insert("011110100");
thinning_mats.insert("111011000");
thinning_mats.insert("011011001");
thinning_mats.insert("111110000");
thinning_mats.insert("110110100");
thinning_mats.insert("100110110");
thinning_mats.insert("000110111");
thinning_mats.insert("000011111");
thinning_mats.insert("001011011");
thinning_mats.insert("111011001");
thinning_mats.insert("111110100");
thinning_mats.insert("100110111");
thinning_mats.insert("001011111");
thinning_mats.insert("011011011");
thinning_mats.insert("111111000");
thinning_mats.insert("110110110");
thinning_mats.insert("000111111");
thinning_mats.insert("111011011");
thinning_mats.insert("011011111");
thinning_mats.insert("111111100");
thinning_mats.insert("111111001");
thinning_mats.insert("111110110");
thinning_mats.insert("110110111");
thinning_mats.insert("100111111");
thinning_mats.insert("001111111");
thinning_mats.insert("111011111");
thinning_mats.insert("111111101");
thinning_mats.insert("111110111");
thinning_mats.insert("101111111");


vector<vector<vector<char>>> uncond_char(37, vector<vector<char>>(3, vector<char>(3)));

uncond_char[0] = {{'0','0','M'},{'0','M','0'},{'0','0','0'}};
uncond_char[1] = {{'M','0','0'},{'0','M','0'},{'0','0','0'}};
uncond_char[2] = {{'0','0','0'},{'0','M','0'},{'0','M','0'}};
uncond_char[3] = {{'0','0','0'},{'0','M','M'},{'0','0','0'}};
uncond_char[4] = {{'0','0','M'},{'0','M','M'},{'0','0','0'}};
uncond_char[5] = {{'0','M','M'},{'0','M','0'},{'0','0','0'}};
uncond_char[6] = {{'M','M','0'},{'0','M','0'},{'0','0','0'}};
uncond_char[7] = {{'M','0','0'},{'M','M','0'},{'0','0','0'}};
uncond_char[8] = {{'0','0','0'},{'M','M','0'},{'M','0','0'}};
uncond_char[9] = {{'0','0','0'},{'0','M','0'},{'M','M','0'}};
uncond_char[10] = {{'0','0','0'},{'0','M','0'},{'0','M','M'}};
uncond_char[11] = {{'0','0','0'},{'0','M','M'},{'0','0','M'}};
uncond_char[12] = {{'0','M','M'},{'M','M','0'},{'0','0','0'}};
uncond_char[13] = {{'M','M','0'},{'0','M','M'},{'0','0','0'}};
uncond_char[14] = {{'0','M','0'},{'0','M','M'},{'0','0','M'}};
uncond_char[15] = {{'0','0','M'},{'0','M','M'},{'0','M','0'}};
uncond_char[16] = {{'0','A','M'},{'0','M','B'},{'M','0','0'}};
uncond_char[17] = {{'M','B','0'},{'A','M','0'},{'0','0','M'}};
uncond_char[18] = {{'0','0','M'},{'A','M','0'},{'M','B','0'}};
uncond_char[19] = {{'M','0','0'},{'0','M','B'},{'0','A','M'}};
uncond_char[20] = {{'M','M','D'},{'M','M','D'},{'D','D','D'}};
uncond_char[21] = {{'D','M','0'},{'M','M','M'},{'D','0','0'}};
uncond_char[22] = {{'0','M','D'},{'M','M','M'},{'0','0','D'}};
uncond_char[23] = {{'0','0','D'},{'M','M','M'},{'0','M','D'}};
uncond_char[24] = {{'D','0','0'},{'M','M','M'},{'D','M','0'}};
uncond_char[25] = {{'D','M','D'},{'M','M','0'},{'0','M','0'}};
uncond_char[26] = {{'0','M','0'},{'M','M','0'},{'D','M','D'}};
uncond_char[27] = {{'0','M','0'},{'0','M','M'},{'D','M','D'}};
uncond_char[28] = {{'D','M','D'},{'0','M','M'},{'0','M','0'}};
uncond_char[29] = {{'M','D','M'},{'D','M','D'},{'A','B','C'}};
uncond_char[30] = {{'M','D','C'},{'D','M','B'},{'M','D','A'}};
uncond_char[31] = {{'C','B','A'},{'D','M','D'},{'M','D','M'}};
uncond_char[32] = {{'A','D','M'},{'B','M','D'},{'C','D','M'}};
uncond_char[33] = {{'D','M','0'},{'0','M','M'},{'M','0','D'}};
uncond_char[34] = {{'0','M','D'},{'M','M','0'},{'D','0','M'}};
uncond_char[35] = {{'D','0','M'},{'M','M','0'},{'0','M','D'}};
uncond_char[36] = {{'M','0','D'},{'0','M','M'},{'D','M','0'}};


ImageVec op_retval =newImg.thinning(thinning_mats,uncond_char,argv[2]);
    
    
    
    
    op_retval.writeToFile(argv[2]);
    return 0;
}
