% Demo for Structured Edge Detector (please see readme.txt first).

%% set opts for training (see edgesTrain.m)
opts=edgesTrain();                % default options (good settings)
opts.modelDir='models/';          % model will be in models/forest
opts.modelFnm='modelBsds';        % model name
opts.nPos=5e5; opts.nNeg=5e5;     % decrease to speedup training
opts.useParfor=0;                 % parallelize if sufficient memory

%% train edge detector (~20m/8Gb per tree, proportional to nPos/nNeg)
tic, model=edgesTrain(opts); toc; % will load model if already trained

%% set detection parameters (can set after training)
model.opts.multiscale=1;          % for top accuracy set multiscale=1
model.opts.sharpen=2;             % for top speed set sharpen=0
model.opts.nTreesEval=10;          % for top speed set nTreesEval=1
model.opts.nThreads=8;            % max number threads for evaluation
model.opts.nms=1;                 % set to true to enable nms

%% evaluate edge detector on BSDS500 (see edgesEval.m)
if(0), edgesEval( model, 'show',1, 'name','' ); end

%% detect edge and visualize results
I = imread(Path_For_SE);
I_1 = imread(Path_For_Sobel_Map);
I_1 = double(I_1)/255;
tic, E=edgesDetect(I,model); toc
figure(1); im(I); figure(2); im(1-E);
C = 1-E;
%figure(3); im(~C);
B = C<(1-thresh_val);
figure(3); im(~B);

tiger_file = load(eval_Metrics_GT_Path);
%[a,b,c,f,d,g] = edgesEvalImg(E,'D:\Courses\EE_569\SE\Data\Tiger_GT.mat');
orig_precision = b./(c);
orig_recall = f./(d);
tiger_GT = tiger_file.groundTruth;
    Output_Precision = zeros(thresh_num, 5);
    Output_recall = zeros(thresh_num, 5);
    %Output_fscore = zeros(7, 5);
thrs = linspace(0.1, thresh_num/10, thresh_num);
for i = 1:5
    %first_Gt = tiger_GT{1,i}.Boundaries;
    
    %thrs = [0.2,0.4,0.6,0.8,0.9];
    
    if (sobel_Or_SE_Metrics == 1);Ed_Map = E; else; Ed_Map = I_1; end
     [a,b,c,f,d,g] = myPreRecF1(Ed_Map,{tiger_GT{i}},{'thrs', thrs} );
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
Precision_means_across_GT = mean(Output_Precision,2);
recall_means_across_GT = mean(Output_recall,2);
fscore_means_across_GT= (2*Precision_means_across_GT.*recall_means_across_GT)./(Precision_means_across_GT+recall_means_across_GT);
fscore_means_across_GT = fscore_means_across_GT';
x = thrs;
y = fscore_means_across_GT;
figure(4);
plt1 = plot(x,y,'-s');
title('Pig SE F-score v/s Thresholds');

xlabel('Thresholds');
ylabel('F-Score');
fscore_means_across_GT = fscore_means_across_GT';
Precision_means_across_thresh = Precision_means_across_thresh';
recall_means_across_thresh = recall_means_across_thresh';
fscore_means_across_thresh = (2*Precision_means_across_thresh.*recall_means_across_thresh)./(Precision_means_across_thresh+recall_means_across_thresh);
mean_Pre = mean(Precision_means_across_thresh);
mean_re = mean(recall_means_across_thresh);
mean_f = mean(fscore_means_across_thresh);
