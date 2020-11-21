#include "calc.h"
#include <iostream>
#include <map>

#define Xsin -128
#define Xarcsin -127
#define Xsh -126
#define Xcos -125
#define Xarccos -124
#define Xch -123
#define Xtan -122
#define Xarctan -121
#define Xth -120
#define Xexp -119
#define Xln -118

#define ISFUN(x) (x > -129 && x < -117)

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
			if (ISFUN(*ita)) {
				++ita;
			}
			if (*ita == 'x') {
				++ita;
			} else if (*ita >= '0' && *ita <= '9' || *ita == '.') {
				while (*ita >= '0' && *ita <= '9' || *ita == '.') {
					++ita;
				}
			} else if (*ita == '(' || *ita == '['){
				++deptha;
				while (deptha != depth) {
					++ita;
					if (*ita == '(' || *ita == '[') {
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
					} else if (*itb == '(' || *itb == '[') {
						--deptha;
					}
				}
				--itb;
				if (ISFUN(*itb)) {
					--itb;
				}
			}
			if ((*itb != '(' && *itb != '[') || *ita != ')') {
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
	strL->push_front('\\');
	strL->push_back('\\');
	for (auto it = strL->begin(); it != strL->end(); ++it) {
		auto ita = it;
		++ita;
		if (*it == 's') {
			if (*ita != '(') {
				if (*ita++ == 'i' && *ita == 'n') {
					strL->erase((ita--));
					strL->erase(ita);
					*it = Xsin;
				} else if (*ita == 'h') {
					strL->erase(ita);
					*it = Xsh;
				}
			}
		}else if (*it == 'c') {
			if (*ita != '(') {
				if (*ita++ == 'o' && *ita =='s') {
					strL->erase((ita--));
					strL->erase(ita);
					*it = Xcos;
				} else if (*ita == 'h') {
					strL->erase(ita);
					*it = Xch;
				}
			}
		}else if (*it == 't') {
			if (*ita++ == 'a' && *ita == 'n') {
					strL->erase((ita--));
					strL->erase(ita);
					*it = Xtan;
			} else if (*ita == 'h') {
				strL->erase(ita);
				*it = Xth;
			}
		}else if (*it == 'e') {
			if (*ita++ == 'x' && *ita == 'p') {
				strL->erase((ita--));
				strL->erase(ita);
				*it = Xexp;
			}
		} else if (*it == 'a' && *ita++ == 'r' && *ita++ == 'c') {
			switch (*ita) {
			case 's': if (*++ita == 'i' && *++ita == 'n') { *it == Xarcsin; for (int i = 0; i < 5; ++i) { strL->erase(ita--); } } break;
			case 'c': if (*++ita == 'o' && *++ita == 's') { *it == Xarccos; for (int i = 0; i < 5; ++i) { strL->erase(ita--); } } break;
			case 't': if (*++ita == 'a' && *++ita == 'n') { *it == Xarctan; for (int i = 0; i < 5; ++i) { strL->erase(ita--); } } break;
			}
		} else if (*it == 'l' && *ita == 'n') {
			strL->erase(ita);
			*it = Xln;
		}
		if (ISFUN(*it)) {
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
		} else if (*it == 'e') {
			for (auto ita : "2.71828182845904523536") {
				strL->insert(it, ita);
			}
			auto ita = it;
			--it;
			strL->erase(ita);
		} else if (*it == 'g') {
			for (auto ita : "1.61803398874989484820") {
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
		} else if (*it == '*' || *it == '/' || *it == '+' || *it == '-' || *it == '^' || *it == 'l') {
			cur = cur->par;
			cur->sig = *it;
			cur->r = new node;
			cur->r->par = cur;
			cur = cur->r;
		} else if (ISFUN(*it)) {
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
		case 'l':	return log(rec_count(cur->l, var)) / log(rec_count(cur->r, var)); break;
		}
	} else {
		switch (cur->fun) {
		case Xsin:   	return(sin(rec_count(cur->l, var))); break;
		case Xsh:	    return(sinh(rec_count(cur->l, var))); break;
		case Xarcsin:	return(asin(rec_count(cur->l, var))); break;
		case Xcos:	    return(cos(rec_count(cur->l, var))); break;
		case Xch:	    return(cosh(rec_count(cur->l, var))); break;
		case Xarccos:	return(acos(rec_count(cur->l, var))); break;
		case Xtan:	    return(tan(rec_count(cur->l, var))); break;
		case Xth:	    return(tanh(rec_count(cur->l, var))); break;
		case Xarctan:	return(atan(rec_count(cur->l, var))); break;
		case Xexp:	    return(exp(rec_count(cur->l, var))); break;
		case Xln:       return(log(rec_count(cur->l, var))); break;
		}
	}
}