#ifndef HUFFMANTREE_HPP
#define HUFFMANTREE_HPP

#include <string>
#include "HuffmanBase.h"
#include "HeapQueue.h"
#include <list>
#include <map>

class HuffmanTree
{
private:
	HuffmanNode* root;
public:
	std::string compress(const std::string inputStr);
	std::string serializeTree() const;
	std::string decompress(const std::string inputCode);
protected:
	void preorderEncode(HuffmanNode* r, std::string s, std::map<char, std::string>* m);
	void postorderEncode(HuffmanNode* r, std::string s, std::string* serialStr) const;
	void binaryDecode(HuffmanNode* r, size_t i, std::string codeStr, std::string* originalStr);
};


#endif
