#include "Node.h"
#include <string>

using namespace std;

Node::Node() {};

Node* Node::getNextNode(bool t) {
	if (name) {
		return this;
	}
	return t ? right : left;
}

void Node::byebye(Node* cur) {
	if (cur) {
		byebye(cur->left);
		byebye(cur->right);
		delete cur;
	}
}