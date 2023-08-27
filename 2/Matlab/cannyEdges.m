
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 1: Canny Evaluation Metrics
% Implementation 1:
% M-file name: cannyEdges.m cannyPreRecF1.m
% Usage: cannyEdges
% Output image:
% Parameters:

% Demo for Structured Edge Detector (please see readme.txt first).



%% detect edge and visualize results
%I = imread('D:\Courses\EE_569\SE\Data\Pig.jpg');
I_1 = imread(path1);
I_1 = double(I_1)/255;
I_2 = imread(path2);
I_2 = double(I_2)/255;
I_3 = imread(path3);
I_3 = double(I_3)/255;
I_4 = imread(path4);
I_4 = double(I_4)/255;
E_Map = {~I_1,~I_2,~I_3,~I_4};
% tic, E=edgesDetect(I,model); toc
% figure(1); im(I); figure(2); im(1-E);

% C = 1-E;
% %figure(3); im(~C);
% B = C<0.5;
% figure(4); im(~B);
thresh_num = 4;
tiger_file = load(gt_path);
tiger_GT = tiger_file.groundTruth;
    Output_Precision = zeros(thresh_num, 5);
    Output_recall = zeros(thresh_num, 5);
    %Output_fscore = zeros(7, 5);
for i = 1:5
    %first_Gt = tiger_GT{1,i}.Boundaries;
    thrs = linspace(0.1, thresh_num/10, thresh_num);
    %thrs = [0.2,0.4,0.6,0.8,0.9];
     [a,b,c,f,d,g] = cannyPreRecF1(I_1,{tiger_GT{i}},E_Map,{'thrs', thrs} );
    recall = b./(c);
    precision = f./(d);
    Output_recall(:, i) = recall ;
    
    Output_Precision(:, i) = precision;
    
    %fmeasure = (2*precision*recall)/(precision+recall);
%     fprintf('The value of Precision is %.4f\n', precision);
%     fprintf('The value of Recall is %.4f\n', recall);
%     fprintf('The value of fmeasure is %.4f\n', fmeasure);
%     fprintf('---------------------------\n');
    
    
end
%Output_Precision(isnan(Output_Precision))=0;
%Output_recall(isnan(Output_recall))=0;
Output_fscore =(2*Output_Precision.*Output_recall)./(Output_Precision+Output_recall);
%Output_fscore(isnan(Output_fscore))=0;
Precision_means_across_thresh = mean(Output_Precision, 1);
recall_means_across_thresh = mean(Output_recall, 1);
fscore_means_across_thresh= mean(Output_fscore, 1);
Precision_means_across_GT = mean(Output_Precision,2);
recall_means_across_GT = mean(Output_recall,2);
Precision_means_across_thresh = Precision_means_across_thresh';
recall_means_across_thresh = recall_means_across_thresh';
fscore_means_across_thresh = (2*Precision_means_across_thresh.*recall_means_across_thresh)./(Precision_means_across_thresh+recall_means_across_thresh);
mean_Pre = mean(Precision_means_across_thresh);
mean_re = mean(recall_means_across_thresh);
mean_f = mean(fscore_means_across_thresh);
fscore_means_across_GT= (2*Precision_means_across_GT.*recall_means_across_GT)./(Precision_means_across_GT+recall_means_across_GT);
fscore_means_across_GT = fscore_means_across_GT';
x = lower_thresh;
y = upper_thresh;
z = fscore_means_across_GT;
plot3(x,y,z,'-s');
xlabel('lower thresholds');
ylabel('Upper thresholds');
zlabel('F score');
title('Tiger Canny F-score v/s Thresholds');
grid on
fscore_means_across_GT = fscore_means_across_GT';