#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <string>
#include <vector>
#include <stack>
#include "grammar.h"

typedef unsigned char byte;

struct elemL1 {
	byte is_term; // Признак терминал/нетерминал
	byte sym; // Символ
	int nAlt; // Номер текущей альтернативы (для нетерминала)
	int countAlt; // Количество альтернатив (для нетерминала)
};

struct elemL2 {
	byte is_term; // Признак терминал/нетерминал
	byte sym; // Символ
};


class Recognizer {
public:
	Recognizer(Grammar gr = Grammar());
	void setGrammar(Grammar gr);
	std::vector<int> recognize(std::string input);
	
private:
	// Грамматика
	Grammar grammar;
	
	// Стеки и элементы
	std::stack<elemL1> L1;
	elemL1 x;
	std::stack<elemL2> L2;
	elemL2 y;
	
	// Результат распознавания
	std::stack<int> result;
	
	char sos; // Состояние алгоритма (q, b, t)
	char S; // Начальный символ грамматики
	
	int findNet(char); // Поиск нетерминала
	int findTerm(char); // Поиск терминала
	
	// Работа со стеками
	void popL2_RR(int);
	void pushL2_RR(int);
	void pushL2_LR(int);
	
	// Шаги
	void step1();
	void step2();
	void step3();
	void step5();
	void step6a();
	void step6v();
};

#endif
