#include <iostream>
#include <vector>
#include <string>

#include "recognizer.h"
#include "grammar.h"

int main() {
	Grammar grammar;
	grammar.output();
	std::string line;
	std::cout << "Input your line for recognizing: ";
	std::cin >> line;
	Recognizer recognizer(grammar);
	std::vector<int> result = recognizer.recognize(line);
	std::cout << "Result: ";
	for (int i = 0; i < result.size(); i++) {
		std::cout << result[i] << ' ';
	}
	std::cout << '\n';
	return 0;
}
