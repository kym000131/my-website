#ifndef TO_POSTFIX_H
#define TO_POSTFIX_H

#include <iostream>
#include <sstream>
#include "stack.h"

using namespace std;

string to_postfix(const string& infix) {
	string postfix="";
	Stack<char> sta;
	stringstream ss;
	ss.str(infix);
	char _c;
	string ifif = "";
	while (ss >> _c) {
		ifif = ifif + _c;
	}
	char c;
	for(int i=0; i < ifif.size(); i++){

		c = ifif[i];

		if (c >= '0' && c <= '9') {
			postfix = postfix + c;
		}
		else if (c == '.') {
			postfix = postfix + c;
		}
		else if (c == '+') {
			if (sta.empty()) {
				sta.push('+');
				if (ifif[i - 1] != ')') {
					postfix = postfix + " ";
				}
			}
			else {
				if (ifif[i - 1] != ')') {
					postfix = postfix + " ";
				}
				while (sta.top() != '(') {
					postfix = postfix + sta.top() + " ";
					sta.pop();
					if (sta.empty()) break;
				}
				sta.push(c);
			}
		}
		else if (c == '-') {
			if (i == 0 || ifif[i-1] == '*' ||  ifif[i - 1] == '+' || ifif[i - 1] == '(' || ifif[i - 1] == '/') {
				if (ifif[i + 1] == '(') {
					postfix = postfix + "-1 ";
					sta.push('*');
				}
				else {
					postfix = postfix + c;
				}
			}
			else {
				if (sta.empty()) {
					sta.push('-');
					if (ifif[i - 1] != ')') {
						postfix = postfix + " ";
					}
				}
				else {
					if (ifif[i - 1] != ')') {
						postfix = postfix + " ";
					}
					while (sta.top() != '(') {
						postfix = postfix + sta.top() + " ";
						sta.pop();
						if (sta.empty()) break;
					}
					sta.push(c);
				}
			}
			
		}
		else if (c == '*') {
			if (sta.empty()) {
				sta.push('*');
				if (ifif[i - 1] != ')') {
					postfix = postfix + " ";
				}
			}
			else {
				if (ifif[i - 1] != ')') {
					postfix = postfix + " ";
				}
				if (sta.top() == '*' || sta.top() == '/') {
					while ((sta.top() == '*' || sta.top() == '/')) {
						postfix = postfix + sta.top()+" ";
						sta.pop();
						if (sta.empty()) break;
					}
				}
				sta.push('*');
			}
		}
		else if (c == '/') {
			if (sta.empty()) {
				sta.push('/');
				if (ifif[i - 1] != ')') {
					postfix = postfix + " ";
				}
			}
			else {
				if (ifif[i - 1] != ')') {
					postfix = postfix + " ";
				}
				if (sta.top() == '*' || sta.top() == '/') {
					while ((sta.top() == '*' || sta.top() == '/')) {
						postfix = postfix + sta.top()+" ";
						sta.pop();
						if (sta.empty()) break;
					}
				}
				sta.push('/');
			}
		}
		else if (c == '(') {
			sta.push('(');
		}
		else if (c == ')') {
			if (ifif[i - 1] != ')') {
				postfix = postfix + " ";
			}
			while (sta.top() != '(') {
				postfix = postfix + sta.top()+" ";
				sta.pop();
			}
			sta.pop();
		}
	}

	while (!sta.empty()) {
		if (ifif[ifif.size()-1] != ')') {
			postfix = postfix + " ";
		}
		postfix = postfix + sta.top();
		if (sta.size() != 1) {
			postfix = postfix + " ";
		}
		sta.pop();
	}
	if (postfix[postfix.size() - 1] == ' ') {
		postfix.resize(postfix.size() - 1);
	}
	return postfix;
}

#endif //TO_POSTFIX_H


