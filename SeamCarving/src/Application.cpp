#include <iostream>

#include "SeamCarving.h"
#include "Timer.h"

#define END 99
#define START 1

static int input = 0;

void Dialog(std::string& filename, int& iteration) {
	std::string iterations;
	std::cout << "Image Name: ";
	std::cin >> filename;

	std::cout << "How many times do you want do to reduce?";
	std::cin >> iterations;

	iteration = std::stoi(iterations.c_str());
}

void Start() {
	try {
		std::string filename;
		int iteration = 0;

		std::cout << "Start The Programm->" << START << "\nClose The Programm->" << END << "\n->";
		std::cin >> input;

		if (input == START) {
			Dialog(filename, iteration);
			Timer timer;
			timer.StartTimer();
			SeamCarving seamCarving(filename, iteration);
			seamCarving.Run(iteration);
			timer.StopTimer();
			std::cout << "Finished in: " << timer.ElapsedTimeInMili() << " ms" << std::endl;
			std::cout << "Finished in: " << timer.ElapsedTimeInSecound() << " s" << std::endl;
		}
		else {
			std::cout << "Programm will close..." << std::endl;
			exit(EXIT_SUCCESS);
		}
	}
	catch (std::invalid_argument& e) {
		std::cout << "\nError: Please Enter a Number here!!" << std::endl;
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "\nError: Programm will close..." << std::endl;
		exit(EXIT_SUCCESS);
	}
}

int main() {

	while (input != END) {
		Start();
	}


	return EXIT_SUCCESS;
}