#pragma once
#include <vector>
#include "RGB.h"

#define WIDTH 0
#define HEIGHT 1

#define DEBUG 0
#define RELEASE 1



class SeamCarving
{


public:
	SeamCarving(const std::string& imagePath, const int& iterations);
	~SeamCarving();


public:
	void Run(const int& iterations);
	void PrintAllPixels();

private:
	void CheckInputs();
	void RedPixels();

	void LocalEnergy(const int& iterations);
	void TotalEnergy(const int& iterations);
	int GetMinimumInLastColum(const int& width);
	std::vector<int> FindOptimalPath(const int& width);
	void Carving(const int& width, const std::vector<int>& seam);
	void WritePixels(const int& width);

	/*
	* Get Index of Pixel in Array when we have its x and y values
	*/
	__forceinline int GetIndex(const int& y, const int& x, const int& matrixWidth) const;
	__forceinline uint32_t ColorDifference(const Pixel& a, const Pixel& b);
	const Pixel& GetPixel(const int& x, const int& y, const int& width) const;
	const int& GetEnergyFromArray(const int& x, const int& y, const int& width) const;

private:
	std::string m_ImagePath;
	int m_Iterations;

	uint8_t* m_LocalBuffer = nullptr;
	int m_Width;
	int m_Height;
	int m_Channels;

	std::vector<Pixel> m_PixelBuffer;
	std::vector<uint8_t> m_Energy;

	static const int NOT_EXIST = -1;
};



