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
#define Xfloor -117
#define Xmod -116
#define Xlog -115

#define ISFUN(x) (x > -129 && x < -116)
#define ISBINF(x) (x == '*' || x == '/' || x == '+' || x == '-' || x == '^' || x == Xlog || x == Xmod)

struct solver::node {
	node *l, *r, *par;
	char sig;
	bool var; /**< true if variable is used in this part of equation. */
	double val;
	node() : l(nullptr), r(nullptr), par(nullptr), sig(NULL), var(false), val(0) {}
};

struct solver::BorNode {
	std::map<char, BorNode *> next;
	int depth;
	char code;
	BorNode(int depth_ = 0) : depth(depth_), code(NULL) {}
};

solver::solver(std::string _eq) : seq(_eq), root(new node), bor(new BorNode) {
	std::string strs[14] = { "mod", "sin", "sh", "cos", "ch", "tan", "th", "exp", "ln", "floor", "arcsin", "arccos", "arctan", "log"};
	char chrs[14] = { Xmod, Xsin, Xsh, Xcos, Xch, Xtan, Xth, Xexp, Xln, Xfloor, Xarcsin, Xarccos, Xarctan, Xlog};
	bor = new BorNode;
	BorNode *cur = new BorNode(0);
	for (int i = 0; i < 14; ++i) {
		cur = bor;
		for (auto ita : strs[i]) {
			if (cur->next.find(ita) == cur->next.end()) {
				cur->next[ita] = new BorNode(cur->depth + 1);
			}
			cur = cur->next[ita];
		}
		cur->code = chrs[i];
	}
	parser(_eq);
}

solver::~solver() {
	destroy(root);
	destroyBor(bor);
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

void solver::destroyBor(BorNode *cur) {
	for (auto it : cur->next) {
		destroyBor(it.second);
	}
	delete cur;
}

void solver::insertParenthesis(std::list<char> *work, char ch1, char ch2) {
	int depth = 0;//add parenthesis around every operation
	for (auto it = work->begin(); it != work->end(); ++it) {
		int count = 0;
		if (*it == '(' || *it == '[') {
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
			if (*itb != '(' || *ita != ')') {
				++itb;
				work->insert(itb, '(');
				work->insert(ita, ')');
			}
		}
	}
}

void solver::process(std::list<char> *strL) {
	for (auto it : seq) {
		strL->push_back(it);
	}
	if (strL->front() == '\\') {
		strL->pop_front();
		while (strL->front() != '\\') {
			strL->pop_front();
		}
	}
	strL->push_front('\\');
	strL->push_back('\\');
	BorNode *cur = bor;
	for (auto it = strL->begin(); it != strL->end(); ++it) {
		if (cur->next.find(*it) == cur->next.end()) {
			cur = bor;
		} else {
			cur = cur->next[*it];
			if (cur->next.empty()) {
				for (int i = 0; i < cur->depth - 1; ++i) {
					strL->erase(it--);
				}
				*it = cur->code;
				cur = bor;
			}
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
	for (auto it = strL->begin(); it != strL->end(); ++it) {
		if (ISFUN(*it)) {
			++it;
			*it = '[';
		}
	}
	insertParenthesis(strL, '^', Xlog);
	insertParenthesis(strL, '*', '/');
	insertParenthesis(strL, '+', '-');
	insertParenthesis(strL, Xmod, '-');
	for (auto it = strL->begin(); it != strL->end(); ++it) {
		if (*it == '[') {
			*it = '(';
		}
	}
	return;
}

void solver::parser(std::string eq) {
	seq = eq;
	std::list<char> *strL = new std::list<char>;
	process(strL);
	for (auto it : (*strL)) {
		std::cout << it;
	}
	std::cout << '\n';
	//start of tree building
	destroy(root);
	node *cur = root = new node;
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
		} else if (ISBINF(*it)) {
			cur = cur->par;
			cur->sig = *it;
			cur->r = new node;
			cur->r->par = cur;
			cur = cur->r;
		} else if (ISFUN(*it)) {
			cur->sig = *it;
		}
	}
	delete strL;
}

double solver::count(double var) {
	return rec_count(root, var);
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
		case Xlog:	return log(rec_count(cur->l, var)) / log(rec_count(cur->r, var)); break;
		case Xmod:	return fmod(rec_count(cur->l, var), rec_count(cur->r, var)); break;
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
		case Xfloor:    return((long long)rec_count(cur->l, var)); break;
		}
	}
}