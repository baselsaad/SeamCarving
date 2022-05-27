#include "SeamCarving.h"

// Run the App
#if CONFIGURATION == DEBUG
SeamCarving::SeamCarving(const std::string& imagePath, const int& iterations)
	: m_ImagePath(PROJECT_PATH + imagePath)
	, m_Iterations(iterations)
{
	m_ImageBuffer = cv::imread(m_ImagePath);
	m_Height = m_ImageBuffer.rows;
	m_Width = m_ImageBuffer.cols;
	// Allocate Enough memory at the beginng to avoid resizing and copying
	m_Pixels.reserve(m_Height * m_Width);
	m_Energy.reserve(m_Height * m_Width);
}
#else
SeamCarving::SeamCarving(const std::string& imagePath, const int& iterations)
	: m_ImagePath(imagePath)
	, m_Iterations(iterations)
{
	m_ImageBuffer = cv::imread(m_ImagePath);
	m_Height = m_ImageBuffer.rows;
	m_Width = m_ImageBuffer.cols;
	// Allocate Enough memory at the beginng to avoid resizing and copying
	m_Pixels.reserve(m_Height * m_Width);
	m_Energy.reserve(m_Height * m_Width);
}
#endif

SeamCarving::~SeamCarving()
{
}

void SeamCarving::CheckInputs() throw(std::runtime_error)
{
	if (m_ImagePath.empty()) {
		throw std::runtime_error("Error: Can't load the Image, Make sure you gave the correct path and the image is valid");
	}

	if (m_ImageBuffer.empty()) {
		throw std::runtime_error("\nError: Can't load and read the Image");
	}
}

void SeamCarving::Run(const int& iterations) throw(std::runtime_error)
{
	CheckInputs();
	//OpenImage("Original", m_ImageBuffer);
	RedPixels();

	int width = m_Width;
	for (int i = 0; i < iterations; i++) {
		LocalEnergy(width - i);
		TotalEnergy(width - i);
		FindOptimalPath(width - i);
		Carving(width - i);
		ConvertVectorToMat(m_ImageBuffer, m_Pixels, width - i);
	}

	//PrintAllPixels();

	cv::imwrite(PROJECT_PATH + std::string("out.jpg"), m_ImageBuffer);
}

void SeamCarving::PrintAllPixels()
{
	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < m_Width; x++) {
			std::cout << *GetPixelFromArray(y, x, m_Width) << " ";
		}
		std::cout << "\n";
	}
}

void SeamCarving::OpenImage(const std::string& windowTitle, const cv::Mat& image)
{
	cv::namedWindow(windowTitle, CV_WINDOW_AUTOSIZE);
	cv::imshow(windowTitle, image);
	cv::waitKey(0);
}

void SeamCarving::RedPixels()
{
	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < m_Width; x++) {
			auto pixels = m_ImageBuffer.at<cv::Vec3b>(y, x);
			m_Pixels.emplace_back(pixels[2], pixels[1], pixels[0]);
		}
	}
}

__forceinline int SeamCarving::GetIndex(const int& y, const int& x, const int& width)
{
	return (y * width + x);
}



void SeamCarving::LocalEnergy(const int& width)
{
	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < width; x++) {
			Pixel* currentPixel = GetPixelFromArray(y, x, width);
			// x - 1, will get the Pixel before the Current one
			Pixel* leftPixel = GetPixelFromArray(y, x - 1, width);
			// y - 1, will get the Pixel Above the Current one
			Pixel* upperPixel = GetPixelFromArray(y - 1, x, width);

			uint32_t currentAndLeft = ColorDifference(*currentPixel, *leftPixel);
			uint32_t currentAndUpper = ColorDifference(*currentPixel, *upperPixel);

			if (leftPixel == nullptr || leftPixel->isEmpty()) {
				currentAndLeft = 0;
			}

			if (upperPixel == nullptr || upperPixel->isEmpty()) {
				currentAndUpper = 0;
			}

			m_Energy.emplace_back(currentAndLeft + currentAndUpper);
		}
	}
}



void SeamCarving::TotalEnergy(const int& width)
{
	for (int y = 1; y < m_Height; y++) {
		for (int x = 0; x < width; x++) {
			int aboveLeft = GetEnergyFromArray(y - 1, std::max(x - 1, 0), width);
			int aboveCenter = GetEnergyFromArray(y - 1, x, width);
			int aboveRight = GetEnergyFromArray(y - 1, std::min(x + 1, width - 1), width);

			m_Energy[GetIndex(y, x, width)] += std::min(std::min(aboveCenter, aboveLeft), aboveRight);
		}
	}
}

int SeamCarving::GetMinimumInLastColum(const int& width)
{
	int y = m_Height - 1; // last colum

	int minimumIndex = 0;
	int minimumValue = m_Energy[GetIndex(y, 0, width)];

	for (int x = 1; x < m_Width; x++) {
		int nextEnergy = m_Energy[GetIndex(y, x, width)];

		if (nextEnergy < minimumValue) {
			minimumIndex = x;
			minimumValue = nextEnergy;
		}
	}
	
	return minimumIndex;
}

void SeamCarving::FindOptimalPath(const int& width)
{
	m_SeamPath.reserve(m_Height);

	int indexOfMin = GetMinimumInLastColum(width);
	int offset = 0;
	m_SeamPath.emplace_back(indexOfMin);

	for (int y = m_Height - 2; y >= 0; y--) {
		int left = GetEnergyFromArray(y, std::max(indexOfMin - 1, 0), width);
		int center = GetEnergyFromArray(y, indexOfMin, m_Width);
		int right = GetEnergyFromArray(y, std::min(indexOfMin + 1, width - 1), width);

		if (std::min(left, center) > right) {
			offset = 1;
		}
		else if (std::min(left, right) > center) {
			offset = 0;
		}
		else if (std::min(right, center) > left) {
			offset = -1;
		}

		indexOfMin += offset;
		indexOfMin = std::min(std::max(indexOfMin, 0), width - 1); //Edge Case
		m_SeamPath.emplace_back(indexOfMin);
	}

}

void SeamCarving::Carving(const int& width)
{
	for (int y = 0; y < m_Height; y++) {
		for (int x = m_SeamPath.at(m_Height - y - 1); x < width - 1; x++) {
			int currentPosition = GetIndex(y, x, width);
			int neighborPosition = GetIndex(y, x + 1, width);

			m_Pixels[currentPosition] = m_Pixels[neighborPosition];
		}

		m_Pixels[GetIndex(y, width - 1, m_Width)] = NULL_PIXEL;
	}
}

void SeamCarving::ConvertVectorToMat(cv::Mat& outImage, std::vector<Pixel>& image, const int& width)
{
	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < width; x++) {
			cv::Vec3b currentPixel;
			// rgb to bgr
			currentPixel[0] = m_Pixels[GetIndex(y, x, m_Width)].b;
			currentPixel[1] = m_Pixels[GetIndex(y, x, m_Width)].g;
			currentPixel[2] = m_Pixels[GetIndex(y, x, m_Width)].r;
			outImage.at<cv::Vec3b>(y, x) = currentPixel;
		}
	}
}

uint32_t SeamCarving::ColorDifference(const Pixel& a, const Pixel& b)
{
	Pixel result = a - b;
	result.PowerTo(2);
	return result.SumOfAllColor();
}

Pixel* SeamCarving::GetPixelFromArray(const int& y, const int& x, const int& width) {
	if (x < 0 || y < 0 || x >= width || y >= m_Height) {
		return &NULL_PIXEL;
	}
	else {
		return &m_Pixels[GetIndex(y, x, width)];
	}
}

int SeamCarving::GetEnergyFromArray(const int& y, const int& x, const int& width)
{
	if (x < 0 || y < 0 || x >= width || y >= m_Height) {
		return NOT_EXSIST;
	}
	else {
		return m_Energy[GetIndex(y, x, m_Width)];
	}
}











