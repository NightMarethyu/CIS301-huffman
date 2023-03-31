#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "Node.h"

using namespace std;

int frequencies[256] = { NULL };
vector<Node*> huffman;
queue<char> info;
string easyCoding[256];
int bitUse;
ofstream ofs;

void getFrequencies(string);
void buildHuffman();
vector<string> split(string s, char c);
void preorder(Node*);
void encodeDoc();
bool compare(Node*, Node*);

int main(int argc, char** argv)
{
    string path = argv[1];

    if (argc != 2) {
        cout << "Usage: \"compression301.exe [filepath]\"" << endl;
        exit(-1);
    }

    ifstream ifs;
    ifs.open(path, ios::in);

    if (!ifs) {
        cout << "Error: File not found at " << path << endl;
        exit(-1);
    }
    ifs.close();

    auto path2 = split(path, '\\');
    string newFilePath = "";
    for (int i = 0; i < path2.size() - 1; i++) {
        newFilePath += path2[i] + "\\";
    }

    auto fileNameVect = split(path2[path2.size() - 1], '.');
    string outputFileName = newFilePath + fileNameVect[0] + ".zip301";

    ofs.open(outputFileName, ios::out | ios::binary);

    cout << "Calculating Frequencies...";

    auto start1 = chrono::high_resolution_clock::now();

    getFrequencies(path);

    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration1 = (end1 - start1);

    cout << "Total Time: " << duration1.count() << " seconds" << endl << "Generating Huffman Tree...";

    auto start2 = chrono::high_resolution_clock::now();

    buildHuffman();

    auto end2 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration2 = end2 - start2;

    cout << "Total Time: " << duration2.count() << " seconds" << endl << "Generating Huffman Encodings...";

    auto start3 = chrono::high_resolution_clock::now();

    preorder(huffman[0]);
    ofs << "*****" << endl << bitUse << endl;

    auto end3 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration3 = end3 - start3;

    cout << "Total Time: " << duration3.count() << " seconds" << endl << "Encoding Document...";
    auto start4 = chrono::high_resolution_clock::now();

    encodeDoc();

    auto end4 = chrono::high_resolution_clock::now();
    chrono::duration<double> duration4 = end4 - start4;

    cout << "Total Time: " << duration4.count() << " seconds" << endl;

    ofs.close();
    huffman[0]->byebye(huffman[0]);

    chrono::duration<double> totalTime = duration1 + duration2 + duration3 + duration4;

    cout << "Total Program Time: " << totalTime.count() << " seconds" << endl;

}

void getFrequencies(string path) {
    char ch;
    fstream fin(path, fstream::in);
    while (fin >> noskipws >> ch) {
        frequencies[ch]++;
        info.push(ch);
    }
    fin.close();
}

void buildHuffman() {

    for (int i = 0; i < 256; i++) {
        if (frequencies[i]) {
            char name = i;
            int freak = frequencies[i];
            huffman.push_back(new Node(name, freak));
        }
    }

    while (huffman.size() > 1) {
        sort(huffman.begin(), huffman.end(), compare);

        Node* combo = new Node(NULL, huffman.at(0)->frequency + huffman.at(1)->frequency);
        combo->left = huffman.at(0);
        combo->right = huffman.at(1);
        huffman.erase(huffman.begin() + 0);
        huffman.erase(huffman.begin() + 0);
        huffman.push_back(combo);
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

void preorder(Node* n) {
    if (n != NULL) {
        if (n->left) {
            n->left->bitencode = n->bitencode + "0";
            n->right->bitencode = n->bitencode + "1";
        }
        if (n->name) {
            bitUse += n->bitencode.size() * n->frequency;
            string line = n->bitencode + ' ';
            easyCoding[n->name] = n->bitencode;
            if (n->name == ' ') {
                line += "space";
            }
            else if (n->name == '\n') {
                line += "newline";
            }
            else if (n->name == '\r') {
                line += "return";
            }
            else if (n->name == '\t') {
                line += "tab";
            }
            else {
                line += n->name;
            }
            line += '\n';
            ofs << line;
        }
        preorder(n->left);
        preorder(n->right);
    }
}

bool compare(Node* a, Node* b) {
    return a->frequency < b->frequency;
}

void encodeDoc() {
    unsigned char output = 0b00000000;
    int count = 0;

    while (!info.empty()) {
        string bin = easyCoding[info.front()];
        info.pop();

        for (int i = 0; i < bin.length(); i++) {
            output = output << 1;
            if (bin[i] == '1') {
                output = output | 0b00000001;
            }
            count++;
            if (count == 8) {
                ofs << output;
                output = 0b00000000;
                count = 0;
            }
        }
    }

    if (count > 0) {
        while (count < 8) {
            output = output << 1;
            count++;
        }
    }
    ofs << output;
}