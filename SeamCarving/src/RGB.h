#pragma once
#include <iostream>



struct Pixel {
	uint8_t r, g, b;
	

public:
	Pixel(uint8_t r, uint8_t g, uint8_t b)
		: r(r), g(g), b(b)
	{
	}
	Pixel(const Pixel& copy)
		: r(copy.r), g(copy.g), b(copy.b)
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

	bool isEmpty() {
		return r == 0 && g == 0 && b == 0;
	}


	//-----------------------Operator--------------------------------------

	friend std::ostream& operator<<(std::ostream& stream, const Pixel& pixel) {
		stream << "[" << (short)pixel.r << ", " << (short)pixel.g << ", " << (short)pixel.b << "]";
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
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;

		return *this;
	}

	Pixel& operator= (Pixel&& other) noexcept {
		std::cout << "Moved" << std::endl;

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

static Pixel NULL_PIXEL = { 0,0,0 };