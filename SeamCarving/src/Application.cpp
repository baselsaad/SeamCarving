#include <iostream>
#include <string>

#include "SeamCarving.h"
#include "Timer.h"
#include "stb_image.h"
#include "stb_image_write.h"

#define END 99
#define START 1

static int input = 0;

void Dialog(std::string& filename, int& iteration)
{
	std::string iterations;
	std::cout << "Image Name: ";
	std::cin >> filename;

	std::cout << "How many times do you want do to reduce?";
	std::cin >> iterations;

	iteration = std::stoi(iterations.c_str());
}

int Start()
{
	try {
		std::string filename;
		int iteration = 0;

		std::cout << "Start The Program->" << START << "\nClose The Program->" << END << "\n->";
		std::cin >> input;

		if (input == START)
		{
			Dialog(filename, iteration);
			Timer timer;
			timer.StartTimer();
			SeamCarving seamCarving(filename, iteration);
			seamCarving.Run(iteration);
			timer.StopTimer();
			std::cout << "Finished in: " << timer.ElapsedTimeInMili() << " ms" << std::endl;
			std::cout << "Finished in: " << timer.ElapsedTimeInSecound() << " s" << std::endl;
		}
		else
		{
			std::cout << "Program will close..." << std::endl;
			return END;
		}
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "\nError: Please Enter a Number here!!" << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "\nError: Program will close..." << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main()
{
	while (input != END)
	{
		input = Start();
	}

	return EXIT_SUCCESS;
}