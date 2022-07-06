#include "grammar.h"

Grammar::Grammar(std::string L, std::vector<std::string> R) {
	setGrammar(L, R);
}

void Grammar::setGrammar(std::string L, std::vector<std::string> R) {
	LR = L;
	RR = R;
	
	const int symbols_count = 256;
	char symbols_status[symbols_count];
	for (int i = 0; i < symbols_count; i++) {
		symbols_status[i] = 0;
	}
	
	int count;
	for (int i = 0; i < LR.length(); i += count) {
		if (symbols_status[LR[i]] == 0) {
			symbols_status[LR[i]] = 1;
			count = 1;
			while (LR[i + count] == LR[i]) {
				count++;
			}
			inf.push_back({LR[i], count, i});
		}
	}
	
	for (int i = 0; i < RR.size(); i++) {
		for (int j = 0; j < RR[i].length(); j++) {
			if (symbols_status[RR[i][j]] == 0) {
				symbols_status[RR[i][j]] = 2;
				T += RR[i][j];
			}
		}
	}
}

void Grammar::output() {
	for(int i = 0; i < LR.length(); i++) {
		std::cout << i + 1 << ": " << LR[i] << " -> " << RR[i] << '\n';
	}
}
