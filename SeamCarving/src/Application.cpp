#include <iostream>

#include "SeamCarving.h"

#define END 99
#define START 1

static int input = 0;

void Start() {
	std::cout << "Start The Programm->" << START << "\nClose The Programm->" << END << "\n->";
	std::cin >> input;

	if (input == START) {
		std::string filename;
		int iteration = 0;

		std::cout << "Image Name: ";
		std::cin >> filename;

		std::cout << "How many times do you want do to reduce?";
		std::cin >> iteration;

		//TODO: Fix
		SeamCarving seamCarving(filename,  iteration);
		seamCarving.Run(iteration);
	}
	else {
		std::cout << "Programm will close..." << std::endl;
		exit(EXIT_FAILURE);
	}
}


int main() {
	while (input != END) {
		try {
			Start();
		}
		catch (std::runtime_error& e) {
			std::cout << e.what() << std::endl;
		}
	}

	return 0;
}