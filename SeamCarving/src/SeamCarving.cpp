#include "SeamCarving.h"
#include "Timer.h"

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
	m_PixelBuffer.reserve(m_Height * m_Width);
	m_Energy = std::vector<int>(m_Height * m_Width);
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
	m_PixelBuffer.reserve(m_Height * m_Width);
	m_Energy = std::vector<int>(m_Height * m_Width);
}
#endif

SeamCarving::~SeamCarving()
{
}

void SeamCarving::CheckInputs()
{
	if (m_ImagePath.empty()) {
		throw std::runtime_error("Error: Can't load the Image, Make sure you gave the correct path and the image is valid");
	}

	if (m_ImageBuffer.empty()) {
		throw std::runtime_error("\nError: Can't load and read the Image");
	}

	if (m_Iterations < 0) {
		throw std::runtime_error("\nError: Iterations musst be 0 or higher!!");
	}
}

void SeamCarving::RedPixels()
{
	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < m_Width; x++) {
			auto& pixels = m_ImageBuffer.at<cv::Vec3b>(y, x);
			m_PixelBuffer.emplace_back(pixels[2], pixels[1], pixels[0], false);
		}
	}
}

void SeamCarving::PrintAllPixels()
{
	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < m_Width; x++) {
			std::cout << GetPixel(y, x, m_Width) << " ";
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

void SeamCarving::Run(const int& iterations)
{
	CheckInputs();
	RedPixels();

	int width = m_Width;
	for (int i = 0; i < iterations; i++) {
		LocalEnergy(width);
		TotalEnergy(width);
		std::vector<int> seamPath = FindOptimalPath(width);
		Carving(width, seamPath);
		width--;
	}

	ConvertVectorToMat(m_PixelBuffer, m_ImageBuffer, width);

#if  CONFIGURATION == DEBUG
	bool success = cv::imwrite(PROJECT_PATH + std::string("out.jpg"), m_ImageBuffer.colRange(0, width));
#else
	bool success = cv::imwrite(std::string("out.jpg"), m_ImageBuffer.colRange(0, width));
#endif

	if (!success) {
		std::cerr << "Error: Could not Write the File Correcly!" << std::endl;
		exit(EXIT_SUCCESS);
	}
}

void SeamCarving::ConvertVectorToMat(std::vector<Pixel>& src, cv::Mat& dest, const int& width)
{
	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < width; x++) {
			cv::Vec3b currentPixel;
			// rgb to bgr
			currentPixel[0] = src[GetIndex(y, x, m_Width)].B;
			currentPixel[1] = src[GetIndex(y, x, m_Width)].G;
			currentPixel[2] = src[GetIndex(y, x, m_Width)].R;
			dest.at<cv::Vec3b>(y, x) = currentPixel;
		}
	}
}

void SeamCarving::LocalEnergy(const int& width)
{
	int i = 0;
	for (int y = 0; y < m_Height; y++) {
		for (int x = 0; x < width; x++) {
			const Pixel& currentPixel = GetPixel(y, x, width);
			const Pixel& leftPixel = GetPixel(y, x - 1, width);// x - 1, will get the Pixel before the Current one
			const Pixel& upperPixel = GetPixel(y - 1, x, width);// y - 1, will get the Pixel Above the Current one

			uint32_t currentAndLeft = ColorDifference(currentPixel, leftPixel);
			uint32_t currentAndUpper = ColorDifference(currentPixel, upperPixel);

			if (leftPixel.IsEmpty()) {
				currentAndLeft = 0;
			}

			if (upperPixel.IsEmpty()) {
				currentAndUpper = 0;
			}

			m_Energy[i] = currentAndLeft + currentAndUpper;
			i++;
		}
	}
}

void SeamCarving::TotalEnergy(const int& width)
{
	for (int y = 1; y < m_Height; y++) {
		for (int x = 0; x < width; x++) {
			const int& aboveLeft = GetEnergyFromArray(y - 1, std::max(x - 1, 0), width);
			const int& aboveCenter = GetEnergyFromArray(y - 1, x, width);
			const int& aboveRight = GetEnergyFromArray(y - 1, std::min(x + 1, width - 1), width);

			m_Energy[GetIndex(y, x, m_Width)] += std::min(std::min(aboveCenter, aboveLeft), aboveRight);
		}
	}
}

int SeamCarving::GetMinimumInLastColum(const int& width)
{
	int y = m_Height - 1; // last colum

	int minimumIndex = 0;
	int minimumValue = m_Energy[GetIndex(y, 0, m_Width)];
	for (int x = 1; x < width; x++) {
		const int& nextEnergy = m_Energy[GetIndex(y, x, m_Width)];

		if (nextEnergy < minimumValue) {
			minimumIndex = x;
			minimumValue = nextEnergy;
		}
	}

	return minimumIndex;
}

std::vector<int> SeamCarving::FindOptimalPath(const int& width)
{
	std::vector<int> seamPath;
	seamPath.reserve(m_Height);

	int indexOfMin = GetMinimumInLastColum(width);
	seamPath.emplace_back(indexOfMin);

	int offset = 0;

	for (int y = m_Height - 2; y >= 0; y--) {
		const int& left = GetEnergyFromArray(y, std::max(indexOfMin - 1, 0), width);
		const int& center = GetEnergyFromArray(y, indexOfMin, width);
		const int& right = GetEnergyFromArray(y, std::min(indexOfMin + 1, width - 1), width);

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
		seamPath.emplace_back(indexOfMin);
	}
	return seamPath;
}

void SeamCarving::Carving(const int& width, const std::vector<int>& seamPath)
{
	for (int y = 0; y < m_Height; y++) {
		// The First X-Pos in Image is in The last Index in SeamPath, thats way we beginn from the bottom of the vector
		for (int x = seamPath.at(m_Height - y - 1); x < width - 1; x++) {
			const int& toRemovePixel = GetIndex(y, x, m_Width);
			const int& neighborPosition = GetIndex(y, x + 1, m_Width);

			m_PixelBuffer[toRemovePixel] = std::move(m_PixelBuffer[neighborPosition]);
		}

		m_PixelBuffer[GetIndex(y, width - 1, m_Width)].Clear();
	}
}

__forceinline uint32_t SeamCarving::ColorDifference(const Pixel& a, const Pixel& b)
{
	return Pixel::DiffColor(a, b);
}

const Pixel& SeamCarving::GetPixel(const int& y, const int& x, const int& width) const{
	if (x < 0 || y < 0 || x >= width || y >= m_Height) {
		return NULL_PIXEL;
	}
	else {
		return m_PixelBuffer[GetIndex(y, x, m_Width)];
	}
}

const int& SeamCarving::GetEnergyFromArray(const int& y, const int& x, const int& width) const
{
	if (x < 0 || y < 0 || x >= width || y >= m_Height) {
		return NOT_EXIST;
	}
	else {
		return m_Energy[GetIndex(y, x, m_Width)];
	}
}

__forceinline int SeamCarving::GetIndex(const int& y, const int& x, const int& width) const
{
	return (y * width + x);
}


