#include "recognizer.h"

Recognizer::Recognizer (Grammar gr) {
	grammar = gr;
	S = gr.LR[0];
}

void Recognizer::setGrammar (Grammar gr) {
	grammar = gr;
	S = gr.LR[0];
}

int Recognizer::findNet(char s) {
	for (int i = 0; i < grammar.inf.size(); i++) {
		if (grammar.inf[i].Net == s) {
			return i;
		}
	}
	return -1;
}

int Recognizer::findTerm(char s) {
	for (int i = 0; i < grammar.T.length(); i++) {
		if (grammar.T[i] == s) {
			return i;
		}
	}
	return -1;
}

// Применение альтернативы (удаление элеметов из стека L2)
void Recognizer::popL2_RR(int rule_num){
	int length = grammar.RR[rule_num].length();
	for (int i = 0; i < length; i++){
		L2.pop();
	}
}

// Заполнение стека L2 по правым частям правил
void Recognizer::pushL2_RR(int rule_num){
	int length = grammar.RR[rule_num].length();
	for (int i = 0; i < length; i++){
		// Заполнение элемента стека L2
		y.sym = grammar.RR[rule_num][length - i - 1];
		y.is_term = (findNet(grammar.RR[rule_num][length - i - 1]) == -1);
		
		L2.push(y);
	}
}

// Заполнение стека L2 по левым частям правил
void Recognizer::pushL2_LR(int rule_num){
	// Заполнение элемента стека L2
	y.sym = grammar.LR[rule_num];
	y.is_term = (findNet(grammar.LR[rule_num]) == -1);
	
	L2.push(y);
}

// Шаг 1: применение первой альтернативы
void Recognizer::step1() {
	int net = findNet(y.sym);
	
	// Заполнение элемента стека L1
	x.is_term = 0;
	x.sym = y.sym;
	x.nAlt = 1;
	x.countAlt = grammar.inf[net].countAlt;
	
	L1.push(x);
	L2.pop();
	int rule_num = grammar.inf[net].firstN + x.nAlt - 1;
	pushL2_RR(rule_num);
}

// Шаг 2: перенесение терминального символа из L2 в L1
void Recognizer::step2() {
	L2.pop();
	
	// Заполнение элемента стека L1
	x.is_term=1;
	x.sym=y.sym;
	x.countAlt=0;
	x.nAlt=0;
	
	L1.push(x);
}

// Шаг 3: завершение алгоритма и формироание цепочки
void Recognizer::step3() {
	int net;
	while(!L1.empty()){
		x = L1.top();
		L1.pop();
		net = findNet(x.sym);
		if (net != -1) {
			result.push(grammar.inf[net].firstN + x.nAlt);
		}
	}
}

// Шаг 5: возврат (перенос элемента из L1 в L2)
void Recognizer::step5() {
	L1.pop();
	y.sym=x.sym;
	y.is_term=1;
	L2.push(y);
}

// Шаг 6a: применение следующей альтернативы
void Recognizer::step6a() {
	L1.pop();
	x.nAlt++;
	L1.push(x);
	int net = findNet(x.sym);
	int rule_num = grammar.inf[net].firstN + x.nAlt - 1;
	popL2_RR(rule_num - 1);
	pushL2_RR(rule_num);
}

// Шаг 6v: возврат (отмена текущей альтернативы)
void Recognizer::step6v() {
	int net = findNet(x.sym);
	int rule_num = grammar.inf[net].firstN + x.nAlt - 1;
	popL2_RR(rule_num);
	pushL2_LR(rule_num);
	L1.pop();
}

std::vector<int> Recognizer::recognize(std::string input) {
	std::vector<int> result_return;
	L1 = std::stack<elemL1>();
	L2 = std::stack<elemL2>();
	result = std::stack<int>();
	bool is_not_end = true;
	int n = input.length();
	int i = 0, j = 0, k = 0;
	sos = 'q';
	y.is_term = 0;
	y.sym = S;
	L2.push(y);
	k++;
	while(is_not_end) {
		switch (sos) {
			case 't':
				while(!result.empty()){
					result_return.push_back(result.top());
					result.pop();
				}
				is_not_end = false;
				break;
			
			case 'q':
				y = L2.top();
				if (findNet(y.sym) != -1) {
					step1();
				} else { 
					if(y.sym == input[i]) {
						step2();
						i++;
						if(i == n) {
							if(L2.empty()) { // Формирование результата
								step3();
								sos = 't';
							} else { 
								sos='b'; // Шаг 3`
							}
						} else {
							if (L2.empty()) {
								sos = 'b'; // Шаг 3`
							}
						}
					}
					else {
						sos = 'b'; // Шаг 4
					}
				}
				break;
			
			case 'b':
				x = L1.top();
				if (findTerm(x.sym) != -1) {
					step5();
					i--;
				} else {
					if (x.nAlt < x.countAlt) {
						step6a();
						sos = 'q';
					} else { 
						if (x.sym == S && i == 0) {
							is_not_end = false;
							std::cout << "Error in recognizing!\n";
						} else {
							step6v();
						}
					}
				}
				break;
		}
	}
	
	return result_return;
}
