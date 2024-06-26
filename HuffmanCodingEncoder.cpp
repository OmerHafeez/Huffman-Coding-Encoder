#include <iostream>
#include <fstream>

const int MAX_CHAR = 256;

class Node {
public:
    char data;
    int freq;
    Node* left;
    Node* right;

    Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

class PriorityQueue {
public:
    Node** heapArray;
    int capacity;
    int size;


    PriorityQueue(int capacity) : capacity(capacity), size(0) {
        heapArray = new Node*[capacity];
    }

    void push(Node* node) {
        if (size == capacity) {
            std::cout << "Priority queue is full!" << std::endl;
            return;
        }

        int i = size++;
        while (i > 0 && node->freq < heapArray[(i - 1) / 2]->freq) {
            heapArray[i] = heapArray[(i - 1) / 2];
            i = (i - 1) / 2;
        }
        heapArray[i] = node;
    }

    Node* pop() {
        if (size == 0) {
            return nullptr;
        }

        Node* minNode = heapArray[0];
        heapArray[0] = heapArray[--size];
        heapify(0);
        return minNode;
    }

private:
    void heapify(int index) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < size && heapArray[left]->freq < heapArray[smallest]->freq) {
            smallest = left;
        }

        if (right < size && heapArray[right]->freq < heapArray[smallest]->freq) {
            smallest = right;
        }

        if (smallest != index) {
            std::swap(heapArray[index], heapArray[smallest]);
            heapify(smallest);
        }
    }
};

class CodeMap {
private:
    std::string codes[MAX_CHAR];

public:
    void setCode(char c, std::string code) {
        codes[static_cast<unsigned char>(c)] = code;
    }

    std::string getCode(char c) const {
        return codes[static_cast<unsigned char>(c)];
    }
};

Node* buildHuffmanTree(int freq[]) {
    PriorityQueue pq(MAX_CHAR);

    for (int i = 0; i < MAX_CHAR; ++i) {
        if (freq[i] > 0) {
            pq.push(new Node(static_cast<char>(i), freq[i]));
        }
    }

    while (pq.size != 1) {
        Node* left = pq.pop();
        Node* right = pq.pop();

        int sumFreq = left->freq + right->freq;
        Node* newNode = new Node('\0', sumFreq);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    return pq.pop();
}

void buildHuffmanCodes(Node* root, std::string code, CodeMap& codes) {
    if (root == nullptr) {
        return;
    }

    if (!root->left && !root->right) {
        codes.setCode(root->data, code);
    }

    buildHuffmanCodes(root->left, code + "0", codes);
    buildHuffmanCodes(root->right, code + "1", codes);
}

std::string encodeText(const std::string& text, const CodeMap& codes) {
    std::string encodedText;
    for (char c : text) {
        encodedText += codes.getCode(c);
    }
    return encodedText;
}

void writeEncodedFile(const std::string& encodedText) {
    std::ofstream outputFile("output.txt");
    if (outputFile.is_open()) {
        outputFile << encodedText;
        outputFile.close();
    } else {
        std::cout << "Unable to open output.txt";
    }
}

void writeCodesFile(const CodeMap& codes) {
    std::ofstream codesFile("codes.txt");
    if (codesFile.is_open()) {
        for (int i = 0; i < MAX_CHAR; ++i) {
            if (!codes.getCode(static_cast<char>(i)).empty()) {
                codesFile << static_cast<char>(i) << ": " << codes.getCode(static_cast<char>(i)) << "\n";
            }
        }
        codesFile.close();
    } else {
        std::cout << "Unable to open codes.txt";
    }
}

void deleteTree(Node* root) {
    if (root != nullptr) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

int main() {
    std::string text = "AABBAACDDD"; // Input text

    int frequencies[MAX_CHAR] = {0};
    for (char c : text) {
        frequencies[static_cast<unsigned char>(c)]++;
    }

    Node* huffmanRoot = buildHuffmanTree(frequencies);

    CodeMap codes;
    buildHuffmanCodes(huffmanRoot, "", codes);

    std::string encodedText = encodeText(text, codes);

    writeEncodedFile(encodedText);
    writeCodesFile(codes);

    deleteTree(huffmanRoot);

    std::cout << "Encoding completed successfully." << std::endl;

    return 0;
}
