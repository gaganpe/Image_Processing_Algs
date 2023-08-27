
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 3 and Problem 2 Implementation
% Implementation 1:
% M-file name: Readme.m P3B.m P3c.m P2.m mat_read.m mat_write.m
% Usage: P3.m
% Output image: No Image 
%%
%Parameters: For Problem 2
height = 450;
width = 600;
filter_sz = 37;
filename = "D:\Courses\EE_569\HW4\EE569_2023Spring_HW4_materials\Mosaic.raw";
P2(filename,height,width,filter_sz);


%%
%Parameters: For Problem 3b
height = 400;
width = 600;
filename = "D:\Courses\EE_569\HW4\EE569_2023Spring_HW4_materials\polar_bear_2.raw";
filename2 = "D:\Courses\EE_569\HW4\EE569_2023Spring_HW4_materials\brown_bear_1.raw";
P3B(filename,filename2,height,width);
%%
% Parameters: For Problem 3C

filename = "D:\Courses\EE_569\HW4\EE569_2023Spring_HW4_materials\Polar_bear_1.raw";
filename2 = "D:\Courses\EE_569\HW4\EE569_2023Spring_HW4_materials\Polar_bear_2.raw";
filename3 = "D:\Courses\EE_569\HW4\EE569_2023Spring_HW4_materials\Brown_bear_1.raw";
filename4 = "D:\Courses\EE_569\HW4\EE569_2023Spring_HW4_materials\Polar_bear_3.raw";
test_file = "D:\Courses\EE_569\HW4\EE569_2023Spring_HW4_materials\Polar_bear_3.raw";
height = 400;
width = 600;
P3c(filename,filename2,filename3,filename4,test_file,height,width);