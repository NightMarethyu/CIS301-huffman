#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <bitset>
#include <queue>

#include "Node.h"

using namespace std;

vector<string> split(string, char);
void getPrefixCodes(vector<string>);
void addLeaf(vector<string>);
void decodeDoc(vector<unsigned char>, int, int);
void viewHuff(Node*);


ofstream ofs;
Node* head = new Node();

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "Usage: \"decompression301.exe [filepath]\"" << endl;
        exit(-1);
    }

    string path = argv[1];

    ifstream ifs;
    ifs.open(path, ios::in, ios::binary);

    if (!ifs) {
        cout << "Error: File Not found at " << path << endl;
        exit(-1);
    }

    cout << "Reading in file...";

    string line;
    vector<string> huffmanCodes;
    int bitcount;

    getline(ifs, line);

    while (line != "*****") {
        huffmanCodes.push_back(line);
        getline(ifs, line);
    }

    getline(ifs, line);
    bitcount = stoi(line);

    int bytecount = bitcount % 8 == 0 ? bitcount / 8 : (bitcount / 8) + 1;

    vector<unsigned char> chars;

    for (int i = 0; i <= bytecount; i++) {
        char c;
        ifs.get(c);
        chars.push_back(c);
    }

    ifs.close();

    cout << "Done" << endl;

    auto path2 = split(path, '\\');
    string newFilePath = "";
    for (int i = 0; i < path2.size() - 1; i++) {
        newFilePath += path2[i] + "\\";
    }

    auto fileNameVect = split(path2[path2.size() - 1], '.');
    string outputFileName = newFilePath + fileNameVect[0] + "2.txt";

    ofs.open(outputFileName, ios::out | ios::binary);

    cout << "Reading Prefix Codes..." << endl;

    auto start1 = chrono::high_resolution_clock::now();

    getPrefixCodes(huffmanCodes);

    auto end1 = chrono::high_resolution_clock::now();

    chrono::duration<double> duration1 = end1 - start1;

    cout << "Prefix Code time: " << duration1.count() << " seconds" << endl << "Reading compressed binary...";

    auto start2 = chrono::high_resolution_clock::now();

    //viewHuff(head);
    decodeDoc(chars, bitcount, bytecount);

    auto end2 = chrono::high_resolution_clock::now();

    chrono::duration<double> duration2 = end2 - start2;
    cout << "Total time: " << duration2.count() << " seconds" << endl;

    ofs.close();
    head->byebye(head);

    chrono::duration<double> totalTime = duration1 + duration2;

    cout << "Program time: " << totalTime.count() << " seconds" << endl;
}

void getPrefixCodes(vector<string> lines) {
    for (string line : lines) {
        addLeaf(split(line, ' '));
    }
}

void addLeaf(vector<string> code) {
    //cout << "Adding Encoding for " << code[1] << "...";
    auto current = head;
    char n;
    if (code[1] == "space") {
        n = ' ';
    }
    else if (code[1] == "newline") {
        n = '\n';
    }
    else if (code[1] == "return") {
        n = '\r';
    }
    else if (code[1] == "tab") {
        n = '\t';
    }
    else {
        n = code[1][0];
    }

    for (int i = 0; i < code[0].size(); i++) {
        if (code[0][i] == '1') {
            if (!current->right) {
                current->right = new Node();
            }
            current->right->encoding = current->encoding + "1";
            current = current->right;
        }
        else {
            if (!current->left) {
                current->left = new Node();
            }
            current->left->encoding = current->encoding + "0";
            current = current->left;
        }
    }
    current->name = n;
}

void viewHuff(Node* cur) {
    if (cur) {
        if (cur->name) {
            cout << cur->name << " ";
        }
        if (cur->encoding != "") {
            cout << cur->encoding;
        }
        cout << endl;
        viewHuff(cur->left);
        viewHuff(cur->right);
    }
}

void decodeDoc(vector<unsigned char> chars, int bitcount, int bytecount) {
    string output = "";
    Node* current = head;

    for (unsigned char c : chars) {
        bitset<8> set = c;
        for (int i = 7; i >= 0; i--) {
            if (current->name) {
                ofs << current->name;
                current = head;
            }
            if (set[i]) {
                current = current->right;
            }
            else {
                current = current->left;
            }
            bitcount--;
            if (bitcount == 0) {
                break;
            }
        }
    }
}

vector<string> split(string s, char c) {
    vector<string> vals;
    size_t pos = 0;
    string token;
    while ((pos = s.find(c)) != string::npos) {
        token = s.substr(0, pos);
        vals.push_back(token);
        s.erase(0, pos + 1);
    }
    vals.push_back(s);
    return vals;
}
