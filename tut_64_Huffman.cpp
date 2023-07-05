#include <iostream>
#include <queue>
#include <unordered_map>

using namespace std;

// Huffman tree node
struct HuffmanNode {
    char character;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
};

// Comparison function for priority queue
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

// Build frequency table
unordered_map<char, int> buildFrequencyTable(const string& data) {
    unordered_map<char, int> frequencyTable;
    for (char ch : data) {
        frequencyTable[ch]++;
    }
    return frequencyTable;
}

// Build Huffman tree
HuffmanNode* buildHuffmanTree(const unordered_map<char, int>& frequencyTable) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (const auto& entry : frequencyTable) {
        pq.push(new HuffmanNode(entry.first, entry.second));
    }

    while (pq.size() > 1) {
        // Remove the two nodes of the highest priority (lowest frequency) from the queue
        HuffmanNode* leftNode = pq.top();
        pq.pop();
        HuffmanNode* rightNode = pq.top();
        pq.pop();

        // Create a new internal node with the sum of the two frequencies
        // and add it to the priority queue
        HuffmanNode* newNode = new HuffmanNode('$', leftNode->frequency + rightNode->frequency);
        newNode->left = leftNode;
        newNode->right = rightNode;
        pq.push(newNode);
    }

    // The remaining node is the root of the Huffman tree
    return pq.top();
}

// Build Huffman codes
void buildHuffmanCodes(HuffmanNode* root, const string& prefix, unordered_map<char, string>& huffmanCodes) {
    if (root == nullptr) {
        return;
    }

    // Leaf node
    if (!root->left && !root->right) {
        huffmanCodes[root->character] = prefix;
    }

    buildHuffmanCodes(root->left, prefix + "0", huffmanCodes);
    buildHuffmanCodes(root->right, prefix + "1", huffmanCodes);
}

// Huffman encoding
string huffmanEncoding(const string& data) {
    if (data.empty()) {
        return "";
    }

    unordered_map<char, int> frequencyTable = buildFrequencyTable(data);
    HuffmanNode* huffmanTree = buildHuffmanTree(frequencyTable);
    unordered_map<char, string> huffmanCodes;
    buildHuffmanCodes(huffmanTree, "", huffmanCodes);

    string encodedData = "";
    for (char ch : data) {
        encodedData += huffmanCodes[ch];
    }

    return encodedData;
}

// Huffman decoding
string huffmanDecoding(const string& encodedData, HuffmanNode* root) {
    if (encodedData.empty() || root == nullptr) {
        return "";
    }

    string decodedData = "";
    HuffmanNode* currentNode = root;
    for (char bit : encodedData) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        if (!currentNode->left && !currentNode->right) {
            decodedData += currentNode->character;
            currentNode = root;
        }
    }

    return decodedData;
}

int main() {
    string data = "huffman coding example";
    string encodedData = huffmanEncoding(data);

    cout << "Original data: " << data << endl;
    cout << "Encoded data: " << encodedData << endl;

    return 0;
}
