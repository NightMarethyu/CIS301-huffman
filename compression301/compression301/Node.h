#pragma once
#include <string>
#include <vector>

using namespace std;

class Node
{
public:
	char name;
	int frequency;
	string bitencode;
	Node* left;
	Node* right;
	Node();
	Node(char, int);
	void byebye(Node*);
};

