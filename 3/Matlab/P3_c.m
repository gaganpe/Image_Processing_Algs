
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 3: Object Segmentation and Analysis
% Implementation 1:
% M-file name: P3_c.m mat_read.m
% Usage: P3_tot.m 
% Output image:
% Parameters:
function  P3_c( filename, height, width)
rows = height;
cols = width;
img = mat_read( filename, rows, cols );

figure;
imshow(img/255);
gray_img  = zeros(rows,cols);

inv_gray_img  = zeros(rows,cols);

for i = 1:rows
    for j = 1:cols
        gray_img(i,j) = 0.2989*img(i,j,1) + 0.5870*img(i,j,2) + 0.1140*img(i,j,3);
    end
end
op_gray_img  = gray_img;
for i = 2:rows-1
    for j = 2:cols-1
        op_gray_img(i,j) = (gray_img(i-1,j-1)/9 )+ (gray_img(i-1,j)/9 )+ (gray_img(i-1,j+1)/9) ;
        op_gray_img(i,j) = op_gray_img(i,j) +(gray_img(i,j-1)/9)+(gray_img(i,j)/9)+(gray_img(i,j+1)/9) ;
        op_gray_img(i,j) = op_gray_img(i,j) +(gray_img(i+1,j-1)/9) + (gray_img(i+1,j)/9) +(gray_img(i+1,j+1)/9 );
        inv_gray_img(i,j) = 255 - op_gray_img(i,j);
    end
end
figure;
imshow(gray_img/255);

figure;
imshow(op_gray_img/255);
inv_img = inv_gray_img>20;
figure;
imshow(inv_img);


bw_shrink = bwmorph(inv_img, 'shrink', Inf);
figure;
imshow(bw_shrink);
global test;
test = double(inv_img);
global visited;
visited = zeros(size(test));
white_pixels = [];

for i = 1:rows
    for j = 1:cols
        if bw_shrink(i,j) == 1
            white_pixels = [white_pixels; i j];
        end
    end
end
fprintf('The total number of beans = %d \n', size(white_pixels,1));
white_pixels = sortrows(white_pixels, 2);
for i = 1:size(white_pixels,1)
     bfs(white_pixels(i,1),white_pixels(i,2),rows,cols,i+1);
end
new_list_size = [];
for i = 1:size(white_pixels,1)
    value = i+1;
    count=0;
    for j = 1:rows
        for k = 1:cols
            if test(j,k) == value
                count= count+1;
            end
        end
    end
    %fprintf('The connected comp from x = %d y = %d count = %d val=%d\n', white_pixels(i,1), white_pixels(i,2),count,value);
    new_list_size = [new_list_size ; i count];
    
end
new_list_size = sortrows(new_list_size, 2);
for i = 1:size(white_pixels,1)
    fprintf('The size of bean no. %d is %d\n', new_list_size(i,1), new_list_size(i,2));
end
color_show  = zeros(rows,cols,3);
R = [0,127,180,190,255];
G = [50,10,255,100,0];
B = [255,190,108,50,0];
for i = 1:rows
        for j = 1:cols
            if test(i,j) > 1
                color_show(i,j,1) = R(test(i,j)-1);
                color_show(i,j,2) = G(test(i,j)-1);
                color_show(i,j,3) = B(test(i,j)-1);
            end
        end
 end
figure;
imshow(color_show/255);
end
function  bfs( x,y, rows, cols, val)
    global visited;
    global test;
    if( x<1 || y<1 || x>rows || y>cols || test(x,y)~=1 || visited(x,y) ==1)
        if(x>=1 && y>=1 && x<=rows && y<=cols)
        visited(x,y) = 1;
        end
        return;
    end
    test(x,y) = val;
    visited(x,y) = 1;
    directions = [[0, 1]; [0, -1];[1, 0];[-1, 0]];
    for i = 1:size(directions,1)
        new_x =  x+directions(i,1);
        new_y =  y+directions(i,2);            
        if(new_x>=1 && new_y>=1 && new_x<=rows && new_y<=cols && ~(visited(new_x,new_y)))
             bfs( x+directions(i,1),y+directions(i,2), rows, cols, val);
        end
        
    end
end 