/*
 * Camera_Calibration.cpp
 *
 *  Created on: 30-Apr-2020
 *      Author: vimstan
 */

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>


//using namespace std;
//using namespace cv;


int checkerboard[2] = {6,9};

int main(){
	// Store 3D points
	std::vector<std::vector<cv::Point3f>> objpoints;
	// Store 2D points
	std::vector<std::vector<cv::Point2f>> imgpoints;
	// Define the 3D world coordinates
	std::vector<cv::Point3f> world_coordinates;
	for(int i = 0; i < checkerboard[1]; i++)
	{
		for(int j = 0; j < checkerboard[0]; j++)
		world_coordinates.push_back(cv::Point3f(j,i,0));
	}
	// Extract the path of the images
	std::vector<std::string> images_path;
	std::string path = "/Users/vimstan/Desktop/Projects/3D Camera Calibration/images/*.jpg";
	// glob() takes strings and stores them as arrays
	cv::glob(path, images_path);
	// Store pixel coordinates of checker board corners
	std::vector<cv::Point2f> checkerboard_corners;
	cv::Mat frame,gray;
	bool check;
	// Find the checker board corners
	for(int i = 0; i < images_path.size(); i++)
	{
		frame = cv::imread(images_path[i]);
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		check = cv::findChessboardCorners(frame, cv::Size(checkerboard[0],checkerboard[1]), checkerboard_corners, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);
		if(check)
		{
			// Refining the algorithm by iterating a set of times
			cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 50, 0.001);
			// Refining the pixels coordinates of checker board corners
			cv::cornerSubPix(gray, checkerboard_corners, cv::Size(11,11), cv::Size(-1,-1), criteria);
			// Draw the checker board refined corners
			cv::drawChessboardCorners(frame, cv::Size(checkerboard[0], checkerboard[1]), checkerboard_corners, check);
			objpoints.push_back(world_coordinates);
			imgpoints.push_back(checkerboard_corners);
		}
	}
	cv::Mat rotational_matrix, translational_matrix, camera_matrix, dist_coeffs;
	cv::calibrateCamera(objpoints, imgpoints, cv::Size(gray.rows, gray.cols), camera_matrix, dist_coeffs, rotational_matrix, translational_matrix);
	std::cout<<"Camera Matrix: "<<camera_matrix<<std::endl;
	std::cout<<"Distortion Coefficients: "<<dist_coeffs<<std::endl;
	std::cout<<"Rotational Matrix: "<<rotational_matrix<<std::endl;
	std::cout<<"Translational Matrix: "<<translational_matrix<<std::endl;

	return 0;


}
