
# Compiled on Ubuntu 22.04 with g++
# g++ version (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
# Define compiler and linker
CC=g++
LN=g++
printval = ------------------------------------------------------------------------------------------------
All : prob1 prob2

prob1: Sobel_Filter Canny_Filter
prob2: basic_thresh error_difu error_difu_color mbvq_error_difu






Sobel_Filter:Sobel.cpp ImageVec.cpp 
	@echo "$(printval)"
	@echo "Compiling code for Sobel Filter"
	@echo "Execute using ./Sobel_Filter input_image.raw output_image_XGradient.raw output_image_YGradient.raw output_image_Sobel_Op.raw Width Height"
	$(CC) Sobel.cpp ImageVec.cpp -g -o Sobel_Filter
	@echo "$(printval)"
Canny_Filter:Canny_OCV.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Canny Filter"
	@echo "Execute using ./Canny_Filter input_image.raw output_image.raw Width Height Lower_threshold Upper_threshold" 
	$(CC) Canny_OCV.cpp ImageVec.cpp -o Canny_Filter -I /usr/local/include/opencv4/ -L /usr/lib     -lopencv_core -lopencv_photo     -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -g
	@echo "$(printval)"
basic_thresh:Thresholding.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Basic Thresholding Techniques"
	@echo "Execute using ./basic_thresh input_image.raw output_image_I2.raw output_image_I4.raw output_image_I32.raw Width Height " 
	$(CC) Thresholding.cpp ImageVec.cpp -g -o basic_thresh
	@echo "$(printval)"
error_difu:error_diffu_thresh.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Error diffusion methods"
	@echo "Execute using ./error_difu input_image.raw output_image_floyd_st.raw output_image_jjn.raw output_image_stucki.raw Width Height " 
	$(CC) error_diffu_thresh.cpp ImageVec.cpp -g -o error_difu
	@echo "$(printval)"
error_difu_color:error_difu_color.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Colour Separable Error Diffusion"
	@echo "Execute using ./error_difu_color input_image.raw output_image.raw Width Height " 
	$(CC) error_difu_color.cpp ImageVec.cpp -g -o error_difu_color
	@echo "$(printval)"
mbvq_error_difu:mbvq_error_diffu.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for MBVQ error diffusion"
	@echo "Execute using ./mbvq_error_difu input_image.raw output_image.raw Width Height " 
	$(CC) mbvq_error_diffu.cpp ImageVec.cpp -g -o mbvq_error_difu
	@echo "$(printval)"
clean :
	rm Sobel_Filter Canny_Filter basic_thresh error_difu error_difu_color mbvq_error_difu 
