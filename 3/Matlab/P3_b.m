
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 3: Defect detection and counting
% Implementation 1:
% M-file name: P3_b.m mat_read.m
% Usage: P3_tot.m 
% Output image:
% Parameters:
function  P3_b( filename, height, width)

rows =height ;
cols =width;
img = mat_read( filename, rows, cols, true );
inv_img = ~img;
figure;
imshow(inv_img);
% Display the second image in a new figure window
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

for i = 1:size(white_pixels,1)
    
     bfs(white_pixels(i,1),white_pixels(i,2),rows,cols,i+1);
    
end
list_defect = [];
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
    if count < 50
       list_defect = [list_defect;count];
       fprintf('The connected comp from x = %d y = %d count = %d\n', white_pixels(i,1), white_pixels(i,2),count);
    end
    
    
end
figure;

hist(list_defect);
xlabel("Number of Pixels");
ylabel("Pixel Frequency");
title("Defect pixel histogram");
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
    directions = [[0, 1]; [0, -1];[1, 0];[-1, 0];[-1,1];[-1,-1];[1,1];[1,-1]];
    for i = 1:size(directions,1)
        new_x =  x+directions(i,1);
        new_y =  y+directions(i,2);            
        if(new_x>=1 && new_y>=1 && new_x<=rows && new_y<=cols && ~(visited(new_x,new_y)))
             bfs( x+directions(i,1),y+directions(i,2), rows, cols, val);
        end
        
    end
end 