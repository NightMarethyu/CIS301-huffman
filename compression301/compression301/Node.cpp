#include "Node.h"

using namespace std;

Node::Node() {
	name = NULL;
	frequency = 0;
	bitencode = "";
	left = nullptr;
	right = nullptr;
}

Node::Node(char n, int f) {
	name = n;
	frequency = f;
	bitencode = "";
	left = nullptr;
	right = nullptr;
}

void Node::byebye(Node* cur) {
	if (cur) {
		byebye(cur->left);
		byebye(cur->right);
		delete cur;
	}
}