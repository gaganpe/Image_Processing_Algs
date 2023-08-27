# Compiled on Ubuntu 22.04 with g++
# g++ version (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
# Define compiler and linker
CC=g++
LN=g++
printval = ------------------------------------------------------------------------------------------------
All : prob1 prob2

prob1: Bil_demosaic P1_HistEq_A P1_HistEq_B P1_CLAHE
prob2: P2_Basic_Denoising P2_Bilateral_Filter P2_NLM P2_Color_Basic P2_Color_NLM P2_Color_Mean P2_HSV_Simple_Mean P2_HSV






Bil_demosaic:P1_Bil_demosaic.cpp ImageVec.cpp 
	@echo "$(printval)"
	@echo "Compiling code for Bilinear Demosaicing"
	@echo "Execute using ./Bil_demosaic input_image.raw output_image.raw Width Height"
	$(CC) P1_Bil_demosaic.cpp ImageVec.cpp -o Bil_demosaic -g
	@echo "$(printval)"
P1_HistEq_A:P1_HistEq_A.cpp ImageVec.cpp 
	@echo "$(printval)"
	@echo "Compiling code for Transfer Function Histogram Equalization"
	@echo "Execute using ./P1_HistEq_A input_image.raw output_image.raw Width Height"
	$(CC) P1_HistEq_A.cpp ImageVec.cpp -o P1_HistEq_A -g
	@echo "$(printval)"
P1_HistEq_B:P1_HistEq_B.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Bucket Filling Histogram Equalization"
	@echo "Execute using ./P1_HistEq_B input_image.raw output_image.raw Width Height"
	$(CC) P1_HistEq_B.cpp ImageVec.cpp -o P1_HistEq_B -g
	@echo "$(printval)"
P1_CLAHE:P1_CLAHE.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for CLAHE Histogram Equalization"
	@echo "Execute using ./P1_CLAHE input_image.raw output_image_CLAHE.raw output_image_TF.raw output_image_BF.raw Width Height"
	$(CC) P1_CLAHE.cpp ImageVec.cpp -o P1_CLAHE     -I /usr/local/include/opencv4/     -L /usr/lib     -lopencv_core     -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -g
	@echo "$(printval)"
P2_Basic_Denoising:P2_Basic_Denoising.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Mean and Gaussian Filter Denoising Algorithm"
	@echo "Execute using ./P2_Basic_Denoising input_image.raw output_image_Mean.raw output_image_Gaussian.raw Width Height Mean_Filter_Size Gaus_Filter_size Gaus_Standard_Deviation clean_image.raw"
	$(CC) P2_Basic_Denoising.cpp ImageVec.cpp -o P2_Basic_Denoising -g
	@echo "$(printval)"
P2_Bilateral_Filter:P2_Bilateral_Filter.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Bilateral Filter Denoising Algorithm"
	@echo "Execute using ./P2_Bilateral_Filter input_image.raw output_image.raw Width Height clean_image.raw"
	$(CC) P2_Bilateral_Filter.cpp ImageVec.cpp -o P2_Bilateral_Filter -g
	@echo "$(printval)"
P2_NLM:P2_NLM.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for NLM Filter Denoising Algorithm"
	@echo "Execute using ./P2_NLM input_image.raw output_image.raw Width Height h big_search_window_size small_neighbour_window_size"
	$(CC) P2_NLM.cpp ImageVec.cpp -o P2_NLM     -I /usr/local/include/opencv4/     -L /usr/lib     -lopencv_core -lopencv_photo     -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -g
	@echo "$(printval)"
P2_Color_Basic:P2_Color_Basic.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Colour Filtering (With Bilateral) Denoising Algorithm"
	@echo "Execute using ./P2_Color_Basc input_image.raw Clean_image.raw output_image.raw Width Height" 
	$(CC) P2_Color_Basic.cpp ImageVec.cpp -o P2_Color_Basic -g
	@echo "$(printval)"
P2_Color_Mean:P2_Color_Mean.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Colour Filtering (With Simple mean all channels) Denoising Algorithm"
	@echo "Execute using ./P2_Color_Mean input_image.raw Clean_image.raw output_image.raw Width Height Mean_Filter_Size" 
	$(CC) P2_Color_Mean.cpp ImageVec.cpp -o P2_Color_Mean -g
	@echo "$(printval)"
P2_HSV_Simple_Mean:P2_HSV_Simple_Mean.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Colour Filtering of only H channel with simple averaging"
	@echo "Execute using ./P2_HSV_Simple_Mean input_image.raw output_image.raw Width Height Mean_Filter_Size" 
	$(CC) P2_HSV_Simple_Mean.cpp ImageVec.cpp -o P2_HSV_Simple_Mean -I /usr/local/include/opencv4/ -L /usr/lib     -lopencv_core -lopencv_photo     -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -g
	@echo "$(printval)"
P2_HSV:P2_HSV.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Colour Filtering of only H channel with angle averaging"
	@echo "Execute using ./P2_HSV input_image.raw Clean_image.raw output_image.raw Width Height Mean_Filter_Size" 
	$(CC) P2_HSV.cpp ImageVec.cpp -o P2_HSV -I /usr/local/include/opencv4/ -L /usr/lib     -lopencv_core -lopencv_photo     -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -g
	@echo "$(printval)"
P2_Color_NLM:P2_Color_NLM.cpp ImageVec.cpp     
	@echo "$(printval)"
	@echo "Compiling code for Colour FIltering with NLM filter"
	@echo "Execute using ./P2_Color_NLM input_image.raw Clean_image.raw output_image.raw Width Height"
	$(CC) P2_Color_NLM.cpp ImageVec.cpp -o P2_Color_NLM -I /usr/local/include/opencv4/ -L /usr/lib     -lopencv_core -lopencv_photo     -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -g
	@echo "$(printval)"
clean :
	rm Bil_demosaic P1_HistEq_A P1_HistEq_B P1_CLAHE P2_Basic_Denoising P2_Bilateral_Filter P2_NLM P2_Color_Basic P2_Color_NLM P2_Color_Mean P2_HSV_Simple_Mean P2_HSV
