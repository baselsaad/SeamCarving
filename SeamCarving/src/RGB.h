#pragma once
#include <iostream>


struct Pixel {
	uint8_t R, G, B, A;
	mutable bool Empty = true;

public:
	Pixel(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a)
		: R(r), G(g), B(b), A(a), Empty(false)
	{
	}

	Pixel(const uint8_t& r, const uint8_t& g, const uint8_t& b)
		: R(r), G(g), B(b), A(0xff), Empty(false)
	{
	}

	Pixel(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a, const bool& empty)
		: R(r), G(g), B(b), A(a), Empty(empty)
	{
	}

	Pixel(const Pixel& other)
		: R(other.R)
		, G(other.G)
		, B(other.B)
		, A(other.A)
	{
		printf("Copied\n");
	}

	Pixel(Pixel&& pixel) noexcept
	{
		printf("Moved\n");
		R = pixel.R;
		G = pixel.G;
		B = pixel.B;
		A = pixel.A;

		pixel.R = NULL;
		pixel.G = NULL;
		pixel.B = NULL;
		pixel.A = NULL;
	}

public:
	int SumOfAllColor()
	{
		return R + G + B;
	}

	__forceinline const bool& IsEmpty() const
	{
		return Empty;
	}

	void Clear()
	{
		R = 0;
		G = 0;
		B = 0;
		A = 0;
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

