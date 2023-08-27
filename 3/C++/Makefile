
# Compiled on Ubuntu 22.04 with g++
# g++ version (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
# Define compiler and linker
CC=g++
LN=g++
printval = ------------------------------------------------------------------------------------------------
All : prob1 prob2 prob3

prob1: GeoMod 
prob2: panorama
prob3: p3_thinning






GeoMod:GeoMod.cpp ImageVec.cpp ImgMat.cpp
	@echo "$(printval)"
	@echo "Compiling code for Geometric Image Modification"
	@echo "Execute using ./Geo_Mod input_image.raw output_image.raw output_reversed_image.raw Width Height " 
	$(CC) GeoMod.cpp ImageVec.cpp ImgMat.cpp -o GeoMod -I /usr/local/include/opencv4/ -L /usr/lib     -lopencv_core -lopencv_photo     -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -g
	@echo "$(printval)"
panorama:panorama.cpp ImageVec.cpp   
	@echo "$(printval)"
	@echo "Compiling code for Homographic Transformation and Image Stitching"
	@echo "Execute using ./panorama input_left_image.raw input_centre_image.raw input_right_image.raw output_image.raw Width Height " 
	$(CC) panorama.cpp ImageVec.cpp ImgMat.cpp -o panorama -I /usr/local/include/opencv4/ -L /usr/lib     -lopencv_core -lopencv_photo     -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_features2d -g
	@echo "$(printval)"
p3_thinning:p3_thinning.cpp ImageVec.cpp   
	@echo "$(printval)"
	@echo "Compiling code for thinning"
	@echo "Execute using ./p3_thinning input_image.raw output_image.raw Width Height" 
	$(CC) p3_thinning.cpp ImageVec.cpp ImgMat.cpp -o p3_thinning -g -I /usr/local/include/opencv4/ -L /usr/lib     -lopencv_core -lopencv_photo     -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_features2d -g
	@echo "$(printval)"
clean :
	rm GeoMod panorama p3_thinning
