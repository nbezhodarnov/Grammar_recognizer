#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <string>
#include <vector>

struct infNet {
	char Net; // Нетерминал
	int countAlt; // Количество альтернатив в списке правил
	int firstN; // Номер первой альтернативы в списке правил
};

struct Grammar {
	std::string LR; // Левые части правил
	std::vector<std::string> RR; // Правые части правил
	std::string T; // Множество терминалов
	std::vector<infNet> inf; // Множество нетерминалов
	
	Grammar(std::string L = "ABBTTMMM", std::vector<std::string> R = {"!B!","T+B","T","M","M*T","a","b","(B)"});
	
	void setGrammar(std::string, std::vector<std::string>);
	
	void output();
};

#endif
