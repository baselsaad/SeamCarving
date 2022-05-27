#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>


struct Pixel {
	int r, g, b;

public:
	Pixel(uint8_t r, uint8_t g, uint8_t b)
		: r(r), g(g), b(b)
	{
	}

	Pixel(const cv::Vec3b& vec)
		: r(vec[2]), g(vec[1]), b(vec[0])
	{
		std::cout << "Copied" << std::endl;
	}

	Pixel(Pixel&& pixel) noexcept {
		r = pixel.r;
		g = pixel.g;
		b = pixel.b;

		pixel.r = NULL;
		pixel.g = NULL;
		pixel.b = NULL;
	}

public:
	void PowerTo(const int& p) {
		r = std::pow(r, p);
		g = std::pow(g, p);
		b = std::pow(b, p);
	}

	int SumOfAllColor() {
		return r + g + b;
	}

	bool IsEmpty() {
		return r == 0 && g == 0 && b == 0;
	}

	const bool IsEmpty() const {
		return r == 0 && g == 0 && b == 0;
	}

	void Clear() {
		this->r = 0;
		this->g = 0;
		this->b = 0;
	}

	cv::Vec3b GetVec3b() const {
		cv::Vec3b vec;
		vec[2] = this->r;
		vec[1] = this->g;
		vec[0] = this->b;

		return vec;
	}

	std::string toString() {
		return "[" + std::to_string(r) + "," + std::to_string(g) + ", " + std::to_string(b) + "]";
	}

	//-----------------------Operator--------------------------------------

	friend std::ostream& operator<<(std::ostream& stream, const Pixel& pixel) {
		stream << "[";
		stream << (short)pixel.r << ", " << (short)pixel.g << ", " << (short)pixel.b << "]";
		return stream;
	}

	friend Pixel operator- (const Pixel& pixel, const Pixel& other) {
		Pixel temp(0, 0, 0);
		temp.r = pixel.r - other.r;
		temp.g = pixel.g - other.g;
		temp.b = pixel.b - other.b;
		return temp;
	}

	Pixel& operator= (const Pixel& other) noexcept {
		std::cout << "Copied" << std::endl;
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;

		return *this;
	}

	Pixel& operator= (Pixel&& other) noexcept {
		//std::cout << "Moved" << std::endl;

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

static const Pixel NULL_PIXEL = { 0,0,0 };