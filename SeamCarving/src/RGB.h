#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>


struct Pixel {
	uint8_t R, G, B;
	mutable bool Empty = true;

public:
	Pixel(const uint8_t& r, const uint8_t& g, const uint8_t& b)
		: R(r), G(g), B(b), Empty(false)
	{
	}

	Pixel(const uint8_t& r, const uint8_t& g, const uint8_t& b, const bool& empty)
		: R(r), G(g), B(b), Empty(empty)
	{
	}

	Pixel(const cv::Vec3b& vec)
		: R(vec[2]), G(vec[1]), B(vec[0]), Empty(false)
	{
		printf("Copied\n");
	}

	Pixel(const Pixel& other)
		: R(other.R)
		, G(other.G)
		, B(other.B)
	{
		printf("Copied\n");
	}

	Pixel(Pixel&& pixel) noexcept
	{
		printf("Moved\n");
		R = pixel.R;
		G = pixel.G;
		B = pixel.B;

		pixel.R = NULL;
		pixel.G = NULL;
		pixel.B = NULL;
	}

public:
	int SumOfAllColor()
	{
		return R + G + B;
	}

	__forceinline bool& IsEmpty()
	{
		return Empty;
	}

	__forceinline const bool& IsEmpty() const
	{
		return Empty;
	}

	void Clear()
	{
		this->R = 0;
		this->G = 0;
		this->B = 0;
	}

	cv::Vec3b GetVec3b() const
	{
		return cv::Vec3b(B, G, R);
	}

	void SetEmpty(const bool& b) const
	{
		Empty = b;
	}

	std::string toString()
	{
		return "[" + std::to_string(R) + "," + std::to_string(G) + ", " + std::to_string(B) + "]";
	}

public:
	__forceinline static uint32_t DiffColor(const Pixel& a, const Pixel& b)
	{
		// Because std::pow takes much time to calculate 
		return PowerToTwo(a.R - b.R) + PowerToTwo(a.G - b.G) + PowerToTwo(a.B - b.B);
	}

	__forceinline static uint32_t PowerToTwo(const int& number)
	{
		return (number * number);
	}

	//-----------------------Operator--------------------------------------

	friend std::ostream& operator<<(std::ostream& stream, const Pixel& pixel)
	{
		stream << "[";
		stream << (short)pixel.R << ", " << (short)pixel.G << ", " << (short)pixel.B << "]";
		return stream;
	}

	__forceinline Pixel operator- (const Pixel& other) const
	{
		return Pixel(R - other.R, G - other.G, B - other.B);
	}

	Pixel& operator= (const Pixel& other) noexcept
	{
		printf("Copied\n");

		this->R = other.R;
		this->G = other.G;
		this->B = other.B;

		return *this;
	}

	Pixel& operator= (Pixel&& other) noexcept
	{
		//printf("Moved\n");

		if (this != &other) {
			this->R = other.R;
			this->G = other.G;
			this->B = other.B;

			other.R = NULL;
			other.G = NULL;
			other.B = NULL;
		}

		return *this;
	}

};

static Pixel NULL_PIXEL(0, 0, 0, true);

