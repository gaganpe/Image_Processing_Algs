
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 3 and Problem 2 Implementation
% Implementation 1:
% M-file name: Readme.m P3B.m P3c.m P2.m mat_read.m mat_write.m
% Usage: Readme.m
% Output image: No Image 
function  P3c(filename,filename2,filename3,filename4,test_file,height,width)

rows =height ;
cols =width;

img = mat_read( filename, rows, cols );
gray_img  = conv2gray( img, rows, cols)/255;
[keypoint_loc, keypoint_desc] = vl_sift(single(gray_img));
img2 = mat_read( filename2, rows, cols );
gray_img2  = conv2gray( img2, rows, cols)/255;
[keypoint_loc2, keypoint_desc2] = vl_sift(single(gray_img2));
img3 = mat_read( filename3, rows, cols );
gray_img3  = conv2gray( img3, rows, cols)/255;
[keypoint_loc3, keypoint_desc3] = vl_sift(single(gray_img3));
img4 = mat_read( filename4, rows, cols );
gray_img4  = conv2gray( img4, rows, cols)/255;
[keypoint_loc4, keypoint_desc4] = vl_sift(single(gray_img4));

test_img = mat_read( test_file, rows, cols );
gray_img_test  = conv2gray( test_img, rows, cols)/255;
[keypoint_loc_test, keypoint_desc_test] = vl_sift(single(gray_img_test));

keypoint_desc_test = keypoint_desc_test';

% create a new matrix with the desired size
kmeans_ip = zeros(128, size(keypoint_desc,2)+size(keypoint_desc2,2)+size(keypoint_desc3,2)+size(keypoint_desc4,2));

% copy the contents of A and B into the new matrix

kmeans_ip(:, 1:size(keypoint_desc,2)) = keypoint_desc;
kmeans_ip(:, size(keypoint_desc,2)+1:size(keypoint_desc,2)+size(keypoint_desc2,2)) = keypoint_desc2;
kmeans_ip(:, size(keypoint_desc,2)+size(keypoint_desc2,2)+1:size(keypoint_desc,2)+size(keypoint_desc2,2)+size(keypoint_desc3,2)) = keypoint_desc3;
kmeans_ip(:, size(keypoint_desc,2)+size(keypoint_desc2,2)+size(keypoint_desc3,2)+1:end) = keypoint_desc4;

kmeans_ip = kmeans_ip';
[idx, centers] = kmeans(kmeans_ip, 8);
k1_cl_points = idx(1:size(keypoint_desc,2),:);
k2_cl_points = idx( size(keypoint_desc,2)+1:size(keypoint_desc,2)+size(keypoint_desc2,2),:);
k3_cl_points = idx( size(keypoint_desc,2)+size(keypoint_desc2,2)+1:size(keypoint_desc,2)+size(keypoint_desc2,2)+size(keypoint_desc3,2),:);
k4_cl_points = idx( size(keypoint_desc,2)+size(keypoint_desc2,2)+size(keypoint_desc3,2)+1:end,:);

ktest_cl_points = kmeanpred(keypoint_desc_test,centers);

hist1 = plothist1(k1_cl_points,"Histogram of cluster values from Polar Bear 1");
hist2 = plothist1(k2_cl_points,"Histogram of cluster values from Polar Bear 2");
hist3 = plothist1(k3_cl_points,"Histogram of cluster values from Brown Bear 1");
hist4 = plothist1(k4_cl_points,"Histogram of cluster values from Polar Bear 3 ");

hist_test = plothist1(ktest_cl_points,"Histogram of cluster values from Bear");


disp("Similarity index between Polar Bear 1 and Bear")
histdiff(hist1,hist_test);
disp("Similarity index between Brown Bear 1 and Bear")
histdiff(hist3,hist_test);
end

function [ img ] = conv2gray( colorImg, rows, cols)
img  = zeros(rows,cols);
for i = 1:rows
    for j = 1:cols
        img(i,j) = 0.2989*colorImg(i,j,1) + 0.5870*colorImg(i,j,2) + 0.1140*colorImg(i,j,3);
    end
end
end 



function [predict_Op] = kmeanpred(descs,centers1)
    [descs_rw,~] = size(descs);
    predict_Op = zeros(descs_rw,1);
    for i = 1:descs_rw
        desc_vec = double(descs(i,:)');
        out = (desc_vec(:)*ones(1,8))';
        sumval = sum((out - centers1).^2 , 2);
        new_distance = sqrt(sumval);
        [~,cluster] = min(new_distance);
        predict_Op(i,1) = cluster;
    end
end

function [hist_prob]= plothist1(cl_points,Plot_name)
    [rw,~] = size(cl_points);
    x_val = (1:max(cl_points));
    vals = zeros(1,max(cl_points));
    for i = 1:rw
        value = cl_points(i,1);
        vals(1,value) = vals(1,value)+1;
    end
    figure;
    bar(x_val,vals);
    title(Plot_name);
    hist_prob = vals./sum(vals);
    xlabel('Word Number');
    ylabel('Number of Keypoints');
end



function  histdiff(hist1,hist2)
    new_mat = [hist1;hist2];
    maxvals = (max(new_mat));
    minvals = (min(new_mat));
    disp((sum(minvals)/sum(maxvals)));

end






