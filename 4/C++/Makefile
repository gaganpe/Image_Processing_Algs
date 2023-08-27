
#
# Compiled on Ubuntu 22.04 with g++
# g++ version (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
# Define compiler and linker
CC=g++
LN=g++
printval = ------------------------------------------------------------------------------------------------
All : prob1 

prob1: text_ana 






text_ana:p1.cpp ImageVec.cpp ImgMat.cpp
	@echo "$(printval)"
	@echo "Compiling code for Texture analysis"
	@echo "Execute using ./text_ana train_image_Folder_path test_image_folder_path Width Height " 
	@echo "The C++ code will generate a train and txt path in the same path as the executable (train_txt and test_txt)" 
	@echo "Execute Python code using python3 HW4_1b.py train_txt_path test_txt_path" 
	$(CC) p1.cpp ImageVec.cpp ImgMat.cpp -o text_ana -I /usr/local/include/opencv4/ -L /usr/lib     -lopencv_core -lopencv_photo     -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -g
	@echo "$(printval)"
	@echo "$(printval)"
clean :
	rm text_ana 
