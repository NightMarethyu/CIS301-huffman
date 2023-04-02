#pragma once
#include <string>

class Node
{
public:
	char name;
	std::string encoding;
	Node* left;
	Node* right;
	Node();

	Node* getNextNode(bool);
	void byebye(Node*);
};

