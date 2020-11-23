#pragma once
#include <cmath>
#include <string>
#include <list>
#include <map>

/// <summary>
/// node for binary tree of operations
/// 
/// </summary>


/// <summary>
/// 
/// </summary>
class solver {
	struct node {
		node *l, *r, *par;
		char sig;
		bool var; /**< true if variable is used in this part of equation. */
		double val;
		node() : l(nullptr), r(nullptr), par(nullptr), sig(NULL), var(false), val(0) {}
	};
	struct BorNode {
		std::map<char, BorNode*> next;
		int depth;
		char code;
		BorNode(int depth_ = 0) : depth(depth_), code(NULL) {}
	};

	node *root;  /**< root of binary tree of operations. */
	BorNode *bor;  /**< root of Bor tree. */
public:
	std::string seq;  /**< currently parsed string. */  

	/// <summary>
	/// initiate first parsing
	/// </summary>
	/// <param name="_eq">the first string for equation</param>
	solver(std::string _eq);

	~solver();

	/// <summary>
	/// counts contained equation with taken variable
	/// </summary>
	/// <param name="var">variable will be used for counting</param>
	/// <returns>the result of equation </returns>
	double count(double var);

	/// <summary>
	/// make binary tree of operations out of string
	/// </summary>
	/// <param name="eq">currently parsed string</param>
	void parser(std::string eq);

private:
	/// <summary>
	/// adds missing Parenthesis needed for string to be parsed
	/// </summary>
	/// <param name="work">list of chars being prosessed </param>
	/// <param name="ch1">first sign of the function will be checked for the necessary parentheses</param>
	/// <param name="ch2">first sign of the function will be checked for the necessary parentheses</param>
	void insertParenthesis(std::list<char> *work, char ch1, char ch2);

	/// <summary>
	/// replaces words with function codes
	/// </summary>
	/// <param name="work">list of chars being prosessed</param>
	void solver::process(std::list<char> *work);

	/// <summary>
	/// destroys current binary tree of operations
	/// </summary>
	/// <param name="cur">the currently destroyed node</param>
	void destroy(node *cur);

	/// <summary>
	/// destroys Bor tree
	/// </summary>
	/// <param name="cur">the currently destroyed node</param>
	void destroyBor(BorNode *cur);

	/// <summary>
	/// initiate recursive counting in equation tree
	/// </summary>
	/// <param name="cur">currently counted node of binary tree of operations</param>
	/// <param name="var">variable will be used for counting</param>
	/// <returns>result of counting current node</returns>
	double rec_count(node *cur, double var);

};