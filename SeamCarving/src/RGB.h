#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>


struct Pixel {
	int r, g, b;
	mutable bool Empty = true;
public:
	Pixel(const uint8_t& r, const uint8_t& g, const uint8_t& b)
		: r(r), g(g), b(b), Empty(false)
	{
	}

	Pixel(const uint8_t& r, const uint8_t& g, const uint8_t& b, const bool& empty)
		: r(r), g(g), b(b), Empty(empty)
	{
	}

	Pixel(const cv::Vec3b& vec)
		: r(vec[2]), g(vec[1]), b(vec[0]), Empty(false)
	{
		printf("Copied\n");
	}

	Pixel(Pixel&& pixel) noexcept {
		printf("Moved\n");
		r = pixel.r;
		g = pixel.g;
		b = pixel.b;

		pixel.r = NULL;
		pixel.g = NULL;
		pixel.b = NULL;
	}

public:
	int SumOfAllColor() {
		return r + g + b;
	}

	__forceinline bool& IsEmpty() {
		return Empty;
	}

	__forceinline const bool& IsEmpty() const {
		return Empty;
	}

	void Clear() {
		this->r = 0;
		this->g = 0;
		this->b = 0;
	}

	cv::Vec3b GetVec3b() const {
		return cv::Vec3b(b, g, r);
	}

	void SetEmpty(const bool& b) const {
		Empty = b;
	}

	std::string toString() {
		return "[" + std::to_string(r) + "," + std::to_string(g) + ", " + std::to_string(b) + "]";
	}

public:
	__forceinline static uint32_t DiffColor(const Pixel& a, const Pixel& b) {
		// Because std::pow takes much time to calculate 
		return PowerToTwo(a.r - b.r) + PowerToTwo(a.g - b.g) + PowerToTwo(a.b - b.b);
	}

	__forceinline static uint32_t PowerToTwo(const int& number) {
		return (number * number);
	}

	//-----------------------Operator--------------------------------------

	friend std::ostream& operator<<(std::ostream& stream, const Pixel& pixel) {
		stream << "[";
		stream << (short)pixel.r << ", " << (short)pixel.g << ", " << (short)pixel.b << "]";
		return stream;
	}

	__forceinline Pixel operator- (const Pixel& other) const {
		return Pixel(r - other.r, g - other.g, b - other.b);
	}

	Pixel& operator= (const Pixel& other) noexcept {
		printf("Copied\n");

		this->r = other.r;
		this->g = other.g;
		this->b = other.b;

		return *this;
	}

	Pixel& operator= (Pixel&& other) noexcept {
		//printf("Moved\n");

		if (this != &other) {
			this->r = other.r;
			this->g = other.g;
			this->b = other.b;

			other.r = NULL;
			other.g = NULL;
			other.b = NULL;
		}

		return *this;
	}

};

static Pixel NULL_PIXEL(0, 0, 0, true);

