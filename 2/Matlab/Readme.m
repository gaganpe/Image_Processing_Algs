
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 1: Structured Edge and Evaluation Metrics
% Implementation 1:
% M-file name: edges_Mine.m myPreRecF1.m
% Usage: edges_Mine.m 
% Output image:
% Parameters:
Path_For_SE = 'D:\Courses\EE_569\SE\Data\Pig.jpg';
Path_For_Sobel_Map = 'D:\Courses\EE_569\Sobel\Pig_Sobel_Grad.png';
eval_Metrics_GT_Path = 'D:\Courses\EE_569\SE\Data\Pig_GT.mat';
sobel_Or_SE_Metrics = 1;%Set 1 for SE and 0 for Sobel,Used for Eval Metrics
thresh_val = 0.1; % Threshold for Structured Edge
thresh_num = 6; % Number of values for threshold (Will always be from range 0.1 to thresh_num/10)
% Other parameters here
edges_Mine;
%Paths of four edgemaps
path1 = 'D:\Courses\EE_569\Canny\Tiger_Canny_120_360.png';
path2 = 'D:\Courses\EE_569\Canny\Tiger_Canny_170_510.png';
path3 = 'D:\Courses\EE_569\Canny\Tiger_Canny_200_400.png';
path4 = 'D:\Courses\EE_569\Canny\Tiger_Canny_200_600.png';
gt_path = 'D:\Courses\EE_569\SE\Data\Tiger_GT.mat';
% Demo for Structured Edge Detector (please see readme.txt first).
%update the lower and upper threshold
lower_thresh = zeros(1, 4);upper_thresh = zeros(1, 4);
lower_thresh(1) = 120;upper_thresh(1) = 360;
lower_thresh(2) = 170;upper_thresh(2) = 510;
lower_thresh(3) = 200;upper_thresh(3) = 400;
lower_thresh(4) = 200;upper_thresh(4) = 600;
cannyEdges;
