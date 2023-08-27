
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 3 and Problem 2 Implementation
% Implementation 1:
% M-file name: Readme.m P3B.m P3c.m P2.m mat_read.m mat_write.m
% Usage: Readme.m
% Output image: No Image 
function  P3B(filename,filename2,height,width)
rows =height ;
cols =width;
img = mat_read( filename, rows, cols );
figure;
imshow(img/255);
gray_img  = zeros(rows,cols);

for i = 1:rows
    for j = 1:cols
        gray_img(i,j) = 0.2989*img(i,j,1) + 0.5870*img(i,j,2) + 0.1140*img(i,j,3);
    end
end
gray_img = gray_img/255;
[keypoint_loc, keypoint_desc] = vl_sift(single(gray_img));
[max_scale,max_index] = max(keypoint_loc(3,:));
x_yval = keypoint_loc(1:2,:);

fprintf('Index of maximum scale value: %d\n', max_index);
fprintf('Value of maximum scale: %f\n', max_scale);

max_loc = x_yval(:, max_index);
figure;
imshow(img/255);
vl_sfd = vl_plotsiftdescriptor(keypoint_desc(:,max_index),keypoint_loc(:,max_index));
    
set(vl_sfd,'color','g');
img1vl_pf = vl_plotframe(keypoint_loc(:,max_index));
set(img1vl_pf,'color','r','linewidth',1);
v1 = double(keypoint_desc(:,max_index));



img2 = mat_read( filename2, rows, cols );
%imshow(img/255);
figure;
imshow(img2/255);

gray_img2  = zeros(rows,cols);

for i = 1:rows
    for j = 1:cols
        gray_img2(i,j) = 0.2989*img2(i,j,1) + 0.5870*img2(i,j,2) + 0.1140*img2(i,j,3);
    end
end

[keypoint_loc2, keypoint_desc2] = vl_sift(single(gray_img2));

distance = 1000000;
img2_index = 0;
for i = 1:size(keypoint_desc2, 2)
    v2 = double(keypoint_desc2(:,i));
    new_distance = sqrt(sum((v1 - v2).^2));
    if(new_distance < distance)
        distance = new_distance;
        img2_index = i;
    end
end
disp("Min dist")
disp(distance);

figure;
imshow(img2/255);
vl_sfd2 = vl_plotsiftdescriptor(keypoint_desc2(:,img2_index),keypoint_loc2(:,img2_index));
    
set(vl_sfd2,'color','g');
img2vl_pf = vl_plotframe(keypoint_loc2(:,img2_index));
set(img2vl_pf ,'color','r','linewidth',1);

matches = vl_ubcmatch(keypoint_desc, keypoint_desc2);
figure;
imshow(cat(2, img/255, img2/255));
hold on;
%scatter(keypoint_loc(1, matches(1,:)), keypoint_loc(2, matches(1,:)), 'r');
%scatter(keypoint_loc2(1, matches(2,:)) + size(img,2), keypoint_loc2(2, matches(2,:)), 'r');
for i = 1:size(matches, 2)
    line([keypoint_loc(1, matches(1,i)), keypoint_loc2(1, matches(2,i))+size(img,2)], [keypoint_loc(2, matches(1,i)), keypoint_loc2(2, matches(2,i))]);
    vl_plotframe([keypoint_loc(1:2, matches(1,i)); keypoint_loc(3, matches(1,i)); keypoint_loc(4, matches(1,i))], 'color', 'g','linewidth',0.5); 
    vl_plotframe([keypoint_loc2(1:2, matches(2,i)) + [size(img,2); 0]; keypoint_loc2(3, matches(2,i)); keypoint_loc2(4, matches(2,i))], 'color', 'r','linewidth',0.5); 
end
hold off;













