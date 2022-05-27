#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>

#include "RGB.h"

#define WIDTH 0
#define HEIGHT 1

#define DEBUG 0
#define RELEASE 1

#define NOT_EXSIST -1

class SeamCarving
{
public:
	SeamCarving(const std::string& imagePath, const int& iterations);
	~SeamCarving();


public:
	void Run(const int& iterations)throw(std::runtime_error);
	void PrintAllPixels();

private:
	void CheckInputs() throw(std::runtime_error);
	void OpenImage(const std::string& windowTitle, const cv::Mat& image);
	void RedPixels();

	void LocalEnergy(const int& iterations);
	void TotalEnergy(const int& iterations);
	int GetMinimumInLastColum(const int& width);
	void FindOptimalPath(const int& width);
	void Carving(const int& width);
	void ConvertVectorToMat(cv::Mat& outImage, std::vector<Pixel>& image, const int& width);

	/*
	* Get Index of Pixel in Array when we have its x and y values
	*/
	__forceinline int GetIndex(const int& y, const int& x, const int& matrixWidth);
	Pixel* GetPixelFromArray(const int& x, const int& y, const int& width);
	uint32_t ColorDifference(const Pixel& a, const Pixel& b);
	int GetEnergyFromArray(const int& x, const int& y, const int& width);

private:
	std::string m_ImagePath;
	int m_Iterations;

	int m_Width;
	int m_Height;

	std::vector<Pixel> m_Pixels;
	std::vector<int> m_Energy;
	std::vector<int> m_SeamPath;

	cv::Mat m_ImageBuffer;


};



