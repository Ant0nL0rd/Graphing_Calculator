#include <cmath>
#include <string>
#include <list>

struct node {
	node *l, *r, *par;
	char sig, fun;
	bool var;
	double val;
	node() : l(nullptr), r(nullptr), par(nullptr), sig(NULL), fun(NULL), var(false), val(0){}
};

class solver {
	node *root;
public:
	std::string seq;

	solver(std::string _eq);

	~solver();

	double count(double var);

	void parser(std::string eq);

private:
	void insertParenthesis(std::list<char> *work, char ch1, char ch2);

	void destroy(node *cur);

	double rec_count(node *cur, double var);

};