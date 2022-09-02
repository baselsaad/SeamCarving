#include <iostream>

#include "SeamCarving.h"
#include "Timer.h"
#include "stb_image.h"
#include "stb_image_write.h"

// Run the App
#if CONFIGURATION == DEBUG
SeamCarving::SeamCarving(const std::string& imagePath, const int& iterations)
	: m_ImagePath(PROJECT_PATH + imagePath), m_Iterations(iterations)
{
	m_LocalBuffer = stbi_load(m_ImagePath.c_str(), &m_Width, &m_Height, &m_Channels, STBI_rgb);

	// Allocate Enough memory at the beginning to avoid resizing and copying
	m_PixelBuffer.reserve(m_Height * m_Width);
	m_Energy = std::vector<uint8_t>(m_Height * m_Width);
}
#else
SeamCarving::SeamCarving(const std::string& imagePath, const int& iterations)
	: m_ImagePath(imagePath), m_Iterations(iterations)
{
	m_LocalBuffer = stbi_load(m_ImagePath.c_str(), &m_Width, &m_Height, &m_Channels, STBI_rgb);

	// Allocate Enough memory at the beginning to avoid resizing and copying
	m_PixelBuffer.reserve(m_Height * m_Width);
	m_Energy = std::vector<uint8_t>(m_Height * m_Width);
}
#endif

SeamCarving::~SeamCarving()
{
	stbi_image_free(m_LocalBuffer);
}

void SeamCarving::CheckInputs()
{
	if (m_Iterations < 0)
		return;

	if (m_ImagePath.empty())
		throw std::runtime_error("Error: Can't load the Image, Make sure you gave the correct path and the image is valid");

	if (m_LocalBuffer == nullptr)
		throw std::runtime_error("\nError: Can't load and read the Image");
}

void SeamCarving::RedPixels()
{
	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			uint8_t* pixelOffset = m_LocalBuffer + (y * m_Width + x) * m_Channels;
			m_PixelBuffer.emplace_back(pixelOffset[0], pixelOffset[1], pixelOffset[2], false);

			//uint8_t* pixelOffset = m_LocalBuffer + (y * m_Width + x) * m_Channels;
			//uint8_t& r = pixelOffset[0];
			//uint8_t& g = pixelOffset[1];
			//uint8_t& b = pixelOffset[2];
			//uint8_t a = m_Channels >= 4 ? pixelOffset[3] : 0xff;
		}
	}
}

void SeamCarving::WritePixels(const int& width)
{
	uint8_t* result = new uint8_t[width * m_Height * m_Channels];

	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			uint8_t* currentPixel = result + GetIndex(y, x, width) * m_Channels;

			currentPixel[0] = m_PixelBuffer[GetIndex(y, x, m_Width)].R;
			currentPixel[1] = m_PixelBuffer[GetIndex(y, x, m_Width)].G;
			currentPixel[2] = m_PixelBuffer[GetIndex(y, x, m_Width)].B;
		}
	}

#if CONFIGURATION == DEBUG
	std::string path = PROJECT_PATH "out.jpg";
#else
	std::string path = "out.jpg";
#endif

	int state = stbi_write_jpg(path.c_str(), width, m_Height, m_Channels, result, 100);

	if (state == 0)
		std::cout << "Failed\n";

	delete[] result;
}

void SeamCarving::PrintAllPixels()
{
	if (m_LocalBuffer == nullptr)
		return;

	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			std::cout << GetPixel(y, x, m_Width) << " ";
		}
		std::cout << "\n";
	}
}

void SeamCarving::Run(const int& iterations)
{
	CheckInputs();
	RedPixels();

	int width = m_Width;
	for (int i = 0; i < iterations; i++)
	{
		LocalEnergy(width);
		TotalEnergy(width);
		std::vector<int> seamPath = FindOptimalPath(width);
		Carving(width, seamPath);
		width--;
	}

	WritePixels(width);
}

void SeamCarving::LocalEnergy(const int& width)
{
	int i = 0;
	for (int y = 0; y < m_Height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			const Pixel& currentPixel = GetPixel(y, x, width);
			const Pixel& leftPixel = GetPixel(y, x - 1, width);// x - 1, will get the Pixel before the Current one
			const Pixel& upperPixel = GetPixel(y - 1, x, width);// y - 1, will get the Pixel Above the Current one

			uint32_t currentAndLeft = ColorDifference(currentPixel, leftPixel);
			uint32_t currentAndUpper = ColorDifference(currentPixel, upperPixel);

			if (leftPixel.IsEmpty())
			{
				currentAndLeft = 0;
			}

			if (upperPixel.IsEmpty())
			{
				currentAndUpper = 0;
			}

			m_Energy[i] = currentAndLeft + currentAndUpper;
			i++;
		}
	}
}

void SeamCarving::TotalEnergy(const int& width)
{
	for (int y = 1; y < m_Height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			const int& aboveLeft = GetEnergyFromArray(y - 1, std::max(x - 1, 0), width);
			const int& aboveCenter = GetEnergyFromArray(y - 1, x, width);
			const int& aboveRight = GetEnergyFromArray(y - 1, std::min(x + 1, width - 1), width);

			m_Energy[GetIndex(y, x, m_Width)] += std::min(std::min(aboveCenter, aboveLeft), aboveRight);
		}
	}
}

int SeamCarving::GetMinimumInLastColum(const int& width)
{
	int y = m_Height - 1; // last column

	int minimumIndex = 0;
	int minimumValue = m_Energy[GetIndex(y, 0, m_Width)];
	for (int x = 1; x < width; x++)
	{
		const int& nextEnergy = m_Energy[GetIndex(y, x, m_Width)];
		if (nextEnergy < minimumValue)
		{
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

	//reverse
	for (int y = m_Height - 2; y >= 0; y--)
	{
		const int& left = GetEnergyFromArray(y, std::max(indexOfMin - 1, 0), width);
		const int& center = GetEnergyFromArray(y, indexOfMin, width);
		const int& right = GetEnergyFromArray(y, std::min(indexOfMin + 1, width - 1), width);

		if (std::min(left, center) > right)
			offset = 1;
		else if (std::min(left, right) > center)
			offset = 0;
		else if (std::min(right, center) > left)
			offset = -1;

		indexOfMin += offset;
		indexOfMin = std::min(std::max(indexOfMin, 0), width - 1); //Edge Case
		seamPath.emplace_back(indexOfMin);
	}
	return seamPath;
}

void SeamCarving::Carving(const int& width, const std::vector<int>& seamPath)
{
	for (int y = 0; y < m_Height; y++)
	{
		// The First X-Pos in Image is in The last Index in SeamPath, thats way we begin from the bottom of the vector
		for (int x = seamPath.at(m_Height - y - 1); x < width - 1; x++)
		{
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

const Pixel& SeamCarving::GetPixel(const int& y, const int& x, const int& width) const
{
	if (x < 0 || y < 0 || x >= width || y >= m_Height)
	{
		return NULL_PIXEL;
	}
	else
	{
		return m_PixelBuffer[GetIndex(y, x, m_Width)];
	}
}

const int& SeamCarving::GetEnergyFromArray(const int& y, const int& x, const int& width) const
{
	if (x < 0 || y < 0 || x >= width || y >= m_Height)
	{
		return NOT_EXIST;
	}
	else
	{
		return m_Energy[GetIndex(y, x, m_Width)];
	}
}

__forceinline int SeamCarving::GetIndex(const int& y, const int& x, const int& width) const
{
	return (y * width + x);
}


