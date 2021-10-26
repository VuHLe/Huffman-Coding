#include "HuffmanTree.h"
#include "HeapQueue.h"
#include <map>
#include <stdlib.h>
#include <iostream>
#include <list>


std::string HuffmanTree::compress(const std::string inputStr)
{
	std::map<char, size_t> charFreq;

	// Gets frequency of each character and stores in map with key being the character
	for (unsigned int i = 0; i < inputStr.size(); i++) {

		// Checks if character has already been added to map, if so, increases frequency by one
		std::map<char, size_t>::iterator itr;
		itr = charFreq.find(inputStr[i]);
		if (itr != charFreq.end()) {
			itr->second++;
		}
		else {
			// Inputs the character into the map with the frequency of 1 if not in map already
			charFreq.insert(std::pair<char, size_t>(inputStr[i], 1));
		}
	}

	// Convert map pairs to Huffman nodes and add to the heap Queue
	HeapQueue <HuffmanNode*, HuffmanNode::Compare> pQueue;

	for (std::map<char, size_t>::iterator itr = charFreq.begin(); itr != charFreq.end(); itr++) {     // Iterator to iterate through map
		HuffmanNode* n = new HuffmanNode(itr->first, itr->second);	// Creates new node and assigns pair to it
		pQueue.insert(n);		// Adds node to heap priority queue
	}

	// Until 1 node in Queue iterates Queue
	for (int i = 0; pQueue.size() > 1; i++) {
		// Removes 2 nodes 
		HuffmanNode* temp1 = pQueue.min();
		pQueue.removeMin();
		HuffmanNode* temp2 = pQueue.min();
		pQueue.removeMin();

		// Create intermediate node to be parent (combine their frequencies for parent)
		size_t totalFreq = temp1->getFrequency() + temp2->getFrequency();
		HuffmanNode* parent = new HuffmanNode(NULL, totalFreq); // Creates parent node
		temp1->parent = parent; // Makes parent node the parent of the nodes
		temp2->parent = parent; 
		parent->right = temp2; // Assigns parent 'right' to higher node
		parent->left = temp1; // Assigns parent 'left; to lower node

		// Add parent node back into Queue
		pQueue.insert(parent);
	}


	// Only one node is left in Queue, which is now the root
	root = pQueue.min();


	// Now preorder traversal to encode each node and store in a map
	std::map<char, std::string> encodedChars;
	std::string s;
	preorderEncode(root, s, &encodedChars);

	// Concatinate all encoded strings in the order of the message
	std::string encMessage = "";
	for (int i = 0; i < inputStr.length(); i++) {

		// Iterates through map
		for (std::map<char, std::string>::iterator itr = encodedChars.begin(); itr != encodedChars.end(); itr++) {

			// Checks if character from string equals, if so, adds encoded string to message string
			if (inputStr[i] == itr->first) {
				encMessage += itr->second;
			}
		}
	}
	/**
	// TEST CODE
	std::cout << std::endl;
	for (std::map<char, std::string>::iterator itr = encodedChars.begin(); itr != encodedChars.end(); itr++) {
		std::cout << itr->first << "\n" << itr->second << std::endl;
	}
	**/
	return encMessage;
}

//________________________________________________________________________________________________
// Going to be a recursive call to traverse the tree in perorder
void HuffmanTree::preorderEncode(HuffmanNode* r, std::string s, std::map<char, std::string>* m) {

	// If leaf node, store code in place of old character in map
	if (r->isLeaf()) {
		m->insert(std::pair<char, std::string>(r->getCharacter(), s));
	}

	// Assigns 0 to the left edge and recur
	if (r->left) {
		s.append("0");
		preorderEncode(r->left, s, m);
		s.pop_back();
	}

	// Assigns 1 to the right edgs and recur
	if (r->right) {
		s.append("1");
		preorderEncode(r->right, s, m);
		s.pop_back();
	}

}
//_________________________________________________________________________________________________
// Going to be a recursive call to traverse the tree in postorder
void HuffmanTree::postorderEncode(HuffmanNode* r,std::string s, std::string* serialStr) const
{
	// go all the way down to the left of branch to get to leaf
	if (r->left) {
		
		postorderEncode(r->left, s, serialStr);
	}
	// if there is no left, then go to the right
	if (r->right) {
		postorderEncode(r->right, s, serialStr);
	}
	// if leaf node, and "L" and the its character to the string
	if (r->isLeaf())
	{
		s.append("L");
		std::string str(1, r->getCharacter());
		s.append(str);
	}
	// if branch node, only add "B" to the string, since its character is NULL
	else if (r->isBranch())
	{
		s.append("B");
	}
	
	serialStr->append(s);
}
//_________________________________________________________________________________
// Find the character from the Huffman tree
void HuffmanTree::binaryDecode(HuffmanNode* r, size_t i, std::string codeStr, std::string* originalStr)
{
	if (r != NULL)
	{
		// if 0, recur to the left 
		if (codeStr[i] == '0')
		{
			binaryDecode(r->left, ++i, codeStr, originalStr);
			
		}
		// if 1, recur to the right
		else if (codeStr[i] == '1')
		{
			binaryDecode(r->right, ++i, codeStr, originalStr);
		}
		// If leaf node, store the character from that node in the string
		if (r->isLeaf())
		{
			std::string s(1, r->getCharacter());
			originalStr->append(s);
		}
	}

}

//_________________________________________________________________________________
std::string HuffmanTree::serializeTree() const
{
	std::string serialMessage = "";
	std::string s = "";

	postorderEncode(root, s, &serialMessage);

	return serialMessage;
}
//____________________________________________________________________________________
std::string HuffmanTree::decompress(const std::string inputCode)
{
	std::string originalMessage = "";
	std::map<char, std::string> encodedChars;
	std::string s;
	size_t i = 0;

	// preorder traversal the Huffman tree to encode each node and store in encodedChar map
	preorderEncode(root, s, &encodedChars);

	while (i < inputCode.size() - 1)
	{
		// get a character from the Huffman tree
		binaryDecode(root, i, inputCode, &originalMessage);

		// loop through the encodedChars map to get the size of the binary code that equivalent to the charater we just got
		for (std::map<char, std::string>::iterator itr = encodedChars.begin(); itr != encodedChars.end(); itr++)
		{
			if (originalMessage.back() == itr->first)
			{
				i = i + itr->second.size(); // increment the index based on the binary code string's size to get to the next code
											// then call binaryEncode again to get the next character
				break;
			}
		}
	}

	return originalMessage;
}

//____________________________________________________________________________________

int main() {
	HuffmanTree t;
	std::string inputString;
	std::cout << "Enter the secret message to encode: \n";
	std::getline(std::cin, inputString);
	std::cout << "\n";

	std::string codeStr = t.compress(inputString);
	std::string serialStr = t.serializeTree();
	std::string originalStr = t.decompress(codeStr);

	std::cout << "Code message: \n" << codeStr << "\n\n" << "Serialize message: \n" << serialStr << "\n\n" 
		<< "Confirm the message: \n" << originalStr << "\n\n";

}