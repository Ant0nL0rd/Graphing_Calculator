#include "calc.h"
#include <iostream>

solver::solver(std::string _eq) : seq(_eq), root(new node) {
	parser(_eq);
}

solver::~solver() {
	destroy(root);
}

double solver::count(double var) {
	return rec_count(root, var);
}

void solver::destroy(node *cur) {
	if (cur->l) {
		destroy(cur->l);
	}
	if (cur->r) {
		destroy(cur->r);
	}
	delete cur;
}

void solver::insertParenthesis(std::list<char> *work, char ch1, char ch2) {
	int depth = 0;//add parenthesis around every operation
	for (auto it = work->begin(); it != work->end(); ++it) {
		int count = 0;
		if (*it == '(') {
			++depth;
		} else if (*it == ')') {
			--depth;
		}
		else if (*it == ch1 || *it == ch2) {
			int deptha = depth;
			auto ita = it;
			++ita;
			if (*ita == 's' || *ita == 'c' || *ita == 't' || *ita == 'e') {
				++ita;
			}
			if (*ita == 'x') {
				++ita;
			} else if (*ita >= '0' && *ita <= '9' || *ita == '.') {
				while (*ita >= '0' && *ita <= '9' || *ita == '.') {
					++ita;
				}
			} else if (*ita == '('){
				++deptha;
				while (deptha != depth) {
					++ita;
					if (*ita == '(') {
						++deptha;
					} else if (*ita == ')') {
						--deptha;
					}
				}
				++ita;
			}
			auto itb = it;
			--itb;
			if (*itb == 'x') {
				--itb;
			} else if (*itb >= '0' && *itb <= '9' || *itb == '.') {
				while (*itb >= '0' && *itb <= '9' || *itb == '.') {
					--itb;
				}
			} else if (*itb == ')') {
				++deptha;
				while (deptha != depth) {
					--itb;
					if (*itb == ')') {
						++deptha;
					} else if (*itb == '(') {
						--deptha;
					}
				}
				--itb;
				if (*itb == 's' || *itb == 'c' || *itb == 't' || *itb == 'e') {
					--itb;
				}
			}
			if (*itb != '(' || *ita != ')') {
				++itb;
				work->insert(itb, '(');
				work->insert(ita, ')');
			}
		}
	}
}

void solver::parser(std::string eq) {
	seq = eq;//create operations tree out of string
	destroy(root);
	node *cur = root = new node;
	std::list<char> *strL = new std::list<char>;

	for (auto it : eq) {
		strL->push_back(it);
	}
	if (strL->front() == '\\') {
		strL->pop_front();
		while(strL->front() != '\\') {
			strL->pop_front();
		}
	}
	if (strL->empty()) {
		strL->push_back(0);
	}
	strL->push_front('\\');
	strL->push_back('\\');
	for (auto it = strL->begin(); it != strL->end(); ++it) {
		if (*it == 's' || *it == 'c' || *it == 't' || *it == 'e') {
			++it;
			*it = '[';
		}
	}
	for (auto it = strL->begin(); it != strL->end(); ++it) {
		if (*it == 'p') {
			for (auto ita : "3.14159265358979323846") {
				strL->insert(it, ita);
			}
			auto ita = it;
			--it;
			strL->erase(ita);
		}
	}
	insertParenthesis(strL, '^', 'l');
	insertParenthesis(strL, '*', '/');
	insertParenthesis(strL, '+', '-');
	for (auto it = strL->begin(); it != strL->end(); ++it) {
		if (*it == '[') {
			*it = '(';
		}
	}
	for (auto it : (*strL)) {
		std::cout << it;
	}
	std::cout << '\n';
	//start of tree building
	for (auto it = strL->begin(); it != strL->end(); ++it) {
		if (*it == '(') {
			cur->l = new node;
			cur->l->par = cur;
			cur = cur->l;
		} else if(*it==')'){
			cur = cur->par;
		} else if (*it == 'x') {
			cur->var = true;
		} else if (*it >= '0' && *it <= '9' || *it == '.') {
			bool negGo = false;
			int negCount = -1;
			while (*it >= '0' && *it <= '9' || *it == '.') {
				if (*it == '.') {
					negGo = true;
				} else if (negGo) {
					cur->val += ((*it) - '0') * pow(10, negCount);
					--negCount;
				} else {
					cur->val = cur->val * 10 + (*it) - '0';
				}
				++it;
			}
			--it;
		} else if (*it == '*' || *it == '/' || *it == '+' || *it == '-' || *it == '^') {
			cur = cur->par;
			cur->sig = *it;
			cur->r = new node;
			cur->r->par = cur;
			cur = cur->r;
		} else if (*it == 's' || *it == 'c' || *it == 't' || *it == 'e') {
			cur->fun = *it;
			cur->l = new node;
			cur->l->par = cur;
		}
	}
	delete strL;
}

double solver::rec_count(node *cur, double var) {
	if (cur->val) {//recursion based solving of operations tree
		return cur->val;
	} else if (cur->var) {
		return var;
	} else if (cur->sig) {
		switch (cur->sig) {
		case '+':	return rec_count(cur->l, var) + rec_count(cur->r, var); break;
		case '-':	return rec_count(cur->l, var) - rec_count(cur->r, var); break;
		case '*':	return rec_count(cur->l, var) * rec_count(cur->r, var); break;
		case '/':	return rec_count(cur->l, var) / rec_count(cur->r, var); break;
		case '^':	return pow(rec_count(cur->l, var), rec_count(cur->r, var)); break;
		}
	} else {
		switch (cur->fun) {
		case 's':	return(sin(rec_count(cur->l, var))); break;
		case 'c':	return(cos(rec_count(cur->l, var))); break;
		case 't':	return(tan(rec_count(cur->l, var))); break;
		case 'e':	return(exp(rec_count(cur->l, var))); break;
		}
	}
}