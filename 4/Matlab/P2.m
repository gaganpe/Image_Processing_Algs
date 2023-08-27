
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Problem 3 and Problem 2 Implementation
% Implementation 1:
% M-file name: Readme.m P3B.m P3c.m P2.m mat_read.m mat_write.m
% Usage: P3.m
% Output image: No Image 
function  P2(filename,height,width,sz)

rows =height ;
cols =width;

img = mat_read( filename, rows, cols,1 );
gray_img = double(img);

new_filt = law_filt_gen();
%gray_img = conv2d_gray(gray_img,gaussian_filter);
op_img = conv2d_gray(gray_img,new_filt);
filter_sz = sz;
filt_resp = p2_std_avg(op_img,filter_sz);
[rows,cols,channels] = size(filt_resp);
kmeans_ip = zeros(rows*cols,channels);

for  i = 1:rows
    for j = 1:cols
        kmeans_ip(i*cols+j,:) = filt_resp(i,j,:);
    end
end
kmeans_mean= mean(kmeans_ip);
kmeans_centered = kmeans_ip - repmat(kmeans_mean, size(kmeans_ip, 1), 1);
kmeans_std = std(kmeans_centered);
kmeans_normalized= kmeans_centered ./ repmat(kmeans_std, size(kmeans_ip, 1), 1);
[idx, ~] = kmeans(kmeans_normalized, 6);
R_segvalue = [107,114,175,167,144,157];
G_segvalue = [143,99,128,57,147,189];
B_segvalue = [159,107,74,32,104,204];
final_img = zeros(rows,cols,3);
final_img_2 = zeros(rows,cols,3);
close_img = zeros(rows,cols);
[~, score, ~] = pca(kmeans_normalized, 'NumComponents', 6);
[idx2, ~] = kmeans(score, 6);
for  i = 1:rows
    for j = 1:cols
        index = idx(i*cols+j) ; 
        final_img(i,j,1) = R_segvalue(index);
        final_img(i,j,2) = G_segvalue(index);
        final_img(i,j,3) = B_segvalue(index);
        index2 = idx2(i*cols+j) ; 
        final_img_2(i,j,1) = R_segvalue(index2);
        final_img_2(i,j,2) = G_segvalue(index2);
        final_img_2(i,j,3) = B_segvalue(index2);
        close_img(i,j) = index2;
    end
end
close_img = close_img/255;   
figure;
imshow(final_img/255);
figure;
imshow(final_img_2/255);

se = strel('disk',10);
closeBW = imclose(close_img,se);
close_img = closeBW*255;
for  i = 1:rows
    for j = 1:cols
        index = close_img(i,j) ; 
        final_img(i,j,1) = R_segvalue(index);
        final_img(i,j,2) = G_segvalue(index);
        final_img(i,j,3) = B_segvalue(index);

    end
end

figure;
imshow(final_img/255);
end

function filters =  law_filt_gen()
    filters = zeros(5,5,25);
    filtgen = zeros(5,5);
    L5 = [1,4,6,4,1];
    E5 = [-1,-2,0,2,1];
    S5 = [-1,-0,2,0,-1];
    W5 = [-1,2,0,-2,1];
    R5 = [1,-4,6,-4,1];
    filtgen(1,:) = L5;
    filtgen(2,:) = E5;
    filtgen(3,:) = S5;
    filtgen(4,:) = W5;
    filtgen(5,:) = R5;
    index=1;
    for i = 1:5
        for j = 1:5
            first_1d = filtgen(i,:);
            second_1d = filtgen(j,:);
            op_filt = zeros(length(first_1d), length(second_1d));
            % compute the tensor product
            for k = 1:length(first_1d)
                for l = 1:length(second_1d)
                    op_filt(k, l) = first_1d(k) * second_1d(l);
                end
            end
            filters(:,:,index) = op_filt;
            index = index+1;
        end
    end
end

function op_img = conv2d_gray(ip_img,filters)
    [input_rows, input_cols] = size(ip_img);
    [filter_rows, ~, num_filters] = size(filters);
    pad_sz = floor(filter_rows/2);
    
    
    
    padded_input = ip_img;
    
    for i = 1:pad_sz
        [pad_rw,pad_cl] = size(padded_input);
        new_row_st = padded_input(2+((i-1)*2), :);
        new_row_en = padded_input(pad_rw - (1+((i-1)*2)), :);
        padded_input = [new_row_st;padded_input;new_row_en];
        new_col_st = padded_input(:,2+((i-1)*2));
        new_col_en = padded_input(:,pad_cl - (1+((i-1)*2)) );
        padded_input = [new_col_st padded_input new_col_en];
    end
    %imshow(padded_input);
    
    
    
    
    op_img = zeros(input_rows, input_cols, num_filters);
    for k = 1:num_filters
    filter = filters(:,:,k);
    for i = 1:input_rows
        for j = 1:input_cols
            window = padded_input(i:i+filter_rows-1, j:j+filter_rows-1);
            elementwise_product = window.* filter;
            convolution_sum = sum(elementwise_product, 'all');
            op_img(i,j,k) = convolution_sum;
        end
    end
    
    end
end

function op_img = p2_std_avg(filt_response,filter_sz)
    [input_rows, input_cols,channels] = size(filt_response);
    
    pad_sz = floor(filter_sz/2);
    padded_input = filt_response;
    
    for i = 1:pad_sz
        [pad_rw,pad_cl,~] = size(padded_input);
        new_row_st = padded_input(2+((i-1)*2), :,:);
        new_row_en = padded_input(pad_rw - (1+((i-1)*2)), :,:);
        padded_input = [new_row_st;padded_input;new_row_en];
        new_col_st = padded_input(:,2+((i-1)*2),:);
        new_col_en = padded_input(:,pad_cl - (1+((i-1)*2)),:);
        padded_input = [new_col_st padded_input new_col_en];
    end
    inter_img = zeros(input_rows, input_cols, channels);
    for k = 1:channels
        for i = 1:input_rows
            for j = 1:input_cols
                window = padded_input(i:i+filter_sz-1, j:j+filter_sz-1,k);
                %elementwise_product = window.* filter;
                window = window.*window;
                convolution_sum = sum(window, 'all');
                inter_img(i,j,k) = convolution_sum/(filter_sz*filter_sz);
            end
        end
    end
    op_img = zeros(input_rows, input_cols, channels-1);
    for i = 1:input_rows
            for j = 1:input_cols
                window = inter_img(i, j,2:end);
                window = window/(inter_img(i, j,1));
                %elementwise_product = window.* filter;
                op_img(i,j,:) = window;
            end
    end
end

