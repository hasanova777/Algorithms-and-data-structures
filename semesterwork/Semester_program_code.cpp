/*
    ������:

    ��������� ���������� ��������� ���� � ������ � ������� ������ ���� � ������ ��
    ���������. ��� ������� ������ ������ �������������� ��������� ������ AA-������.

    1) ����� �� ��������� ������ ����������� � AA-������ ��� ������ ������
    void insert(string key, int value). ��� ���� � �������� ����� ������������ ����
    �����, � � �������� �������� - ������� ��� ��� �����������, �� ���� ��� ������
    ���������� ����� value ����� 1, � ��� ������ ����������� value ������������� �� 1.
    ����� �������� ����� � ������ ���������� ����� int search(string key) ���
    ����������� �������� �������� value ��� ������������ �����.

    2) ����� ������� ���� ���� � AA-������, ��������� ������ ����� ������� � �����
    ����� ���� ����� ���� ������ (� ��������������� ������ ���������). ��� ������
    ���� � ���������� ������� ������������ ����� AANode* min(), ����������� ��
    AA-������ ���� � ���������� ��������� ����� (��� ��� ����� ���� string, �� ��
    ��������� ���������� � ������������������ �������). ����� ���������� ���� �
    ����������� ������ ������������ ������ � ������� ����� � ��������, ����������� �
    ����, � �������� ������� ���� �� AA-������ ��� ������ ������ bool remove(string key).

    ��������� 1. �������� ����� ��� ������ ����� ���� ��� � ���� ������, �������� ��
    �������, ��� �� ������������ �����-���� �������. ����� �� ��������� ������ �����
    �������� � AA-������ �������� � vector<string> words. � ������, ���������� � ������
    ������, ������ words ����������� �������, ���������������� ��������������� �������
    ��� ������ ������ string generate_word(), �������������� �� ������ ������ ������ rand().

    ��������� 2. ����� ������ ������� AA-������, �������������� ��� ������� ���� ���������
    ������, � ����� �����, ����������� �� �� ������� ������, ���������� ��� ������
    ������������ ����������� ���������� chrono.
*/

#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct AANode {
    int level;
    string key;
    int value;
    AANode* right;
    AANode* left;
    AANode* parent;
};

class AATree {
public:
    AATree();
    ~AATree();
    void insert(string, int);
    bool remove(string);
    int search(string key);
    AANode* min();
    AANode* max();
private:
    AANode* root;
    void dealloc_memory(AANode* N);
    void skew(AANode*);
    bool split(AANode*);
    void balance(AANode*);
    AANode* insert_helper(AANode*, AANode*);
    bool remove_helper(AANode*, AANode*, string);
    int search_helper(AANode*, string);
};

/*
    ����������� ������ AATree
*/

AATree::AATree() {
    root = new AANode;
    root = nullptr;
}

/*
    ���������� ������ AATree
*/

AATree::~AATree() {
    dealloc_memory(root);
}

void AATree::dealloc_memory(AANode* N) {
    if (N == nullptr) {
        return;
    }
    dealloc_memory(N->left);
    dealloc_memory(N->right);
    delete N;
}

/*
    ���������� ������ insert, ������� ��������� �������� "�������" �����;
    �������� �������������� ��������� ��������:
    1. ���������� ���� �� ������ �������
    2. ����� ������ skew (��� ���������� �������������� ����� �����)
    3. ����� ������ split (��� ���������� ���� �������������� ������ ������,
    ��������, ������������� ������� ������ ��������� ���� �������� ���������)
*/

void AATree::insert(string key, int value) {
    AANode* temp = new AANode;
    temp->key = key;
    temp->value = value;
    temp->level = 1;
    temp->left = nullptr;
    temp->right = nullptr;
    temp->parent = nullptr;
    insert_helper(root, temp);
}

AANode* AATree::insert_helper(AANode* temp, AANode* ins) {
    if (root == nullptr) {
        ins->parent = nullptr;
        ins->left = nullptr;
        ins->right = nullptr;
        root = ins;
        return root;
    }
    if (ins->key < temp->key) {
        if (temp->left) {
            return insert_helper(temp->left, ins);
        }
        temp->left = ins;
        ins->parent = temp;
        balance(ins);
        return ins;
    }
    if (ins->key > temp->key) {
        if (temp->right) {
            return insert_helper(temp->right, ins);
        }
        temp->right = ins;
        ins->parent = temp;
        balance(ins);
        return ins;
    }
    if (ins->key == temp->key) {
        temp->value = ins->value;
    }
    delete ins;
    return temp;
}

/*
    ���������� ������ remove, ������� ������������ �������� ����� � ������
*/

bool AATree::remove(string key) {
    return this->remove_helper(nullptr, root, key);
}

bool AATree::remove_helper(AANode* parent, AANode* current, string key) {
    if (!current) {
        return false;
    }
    if (current->key == key) {
        if (current->left == nullptr || current->right == nullptr) {
            AANode* temp = current->left;
            if (current->right) {
                temp = current->right;
            }
            if (parent) {
                if (parent->left == current) {
                    parent->left = temp;
                }
                else {
                    parent->right = temp;
                }
            }
            else {
                root = temp;
            }
        }
        else {
            AANode* replacement = current->right;
            while (replacement->left) {
                replacement = replacement->left;
            }
            string temp = current->key;
            current->key = replacement->key;
            replacement->key = temp;
            return remove_helper(current, current->right, temp);
        }
        delete current;
        return true;
    }
    return remove_helper(current, current->left, key) ||
        remove_helper(current, current->right, key);
}

/*
    ���������� ������ skew, ������� ��������� ����� ����� �� ����� ������; ������
    ����� ��������� �������������� ����� ����� ��� ������ �������� ���� ������
    ������ ���, ����� ������� �������������� ����� �����
*/

void AATree::skew(AANode* temp) {
    AANode* ptr = temp->left;
    if (temp->parent->left == temp) {
        temp->parent->left = ptr;
    }
    else {
        temp->parent->right = ptr;
    }
    ptr->parent = temp->parent;
    temp->parent = ptr;
    temp->left = ptr->right;
    if (temp->left != nullptr) {
        temp->left->parent = temp;
    }
    ptr->right = temp;
    temp->level = (temp->left ? temp->left->level + 1 : 1);
}

/*
  ���������� ������ split, ������� ��������� ��� ������ ����� �� ����� ������;
  ������ ����� ��������� ��� ���������������� ������ �������������� ����� ���
  ������ �������� ���� ����� �� ����, ���������� ��� ������� ����������������
  ������ �������������� �����
*/

bool AATree::split(AANode* temp) {
    AANode* ptr = temp->right;
    if (ptr && ptr->right && (ptr->right->level == temp->level)) {
        if (temp->parent->left == temp) {
            temp->parent->left = ptr;
        }
        else {
            temp->parent->right = ptr;
        }
        ptr->parent = temp->parent;
        temp->parent = ptr;
        temp->right = ptr->left;
        if (temp->right != nullptr) {
            temp->right->parent = temp;
        }
        ptr->left = temp;
        ptr->level = temp->level + 1;
        return true;
    }
    return false;
}

/*
    ���������� ������ balance, ������� ������ ������ ���������������� ��� ������
    ������� split � skew (����������� ������ ��� ����������������� ������)
*/

void AATree::balance(AANode* temp) {
    temp->left = nullptr;
    temp->right = nullptr;
    temp->level = 1;
    for (temp = temp->parent; temp != root; temp = temp->parent) {
        if (temp->level != (temp->left ? temp->left->level + 1 : 1)) {
            skew(temp);
            if (temp->right == nullptr) {
                temp = temp->parent;
            }
            else if (temp->level != temp->right->level) {
                temp = temp->parent;
            }
        }
        if (temp->parent != root) {
            if (!split(temp->parent)) {
                break;
            }
        }
    }
}

/*
    ���������� ������ search, ������� ������������ ����� ������� � ������
*/

int AATree::search(string key) {
    return search_helper(root, key);
}

int AATree::search_helper(AANode* temp, string key) {
    if (temp == nullptr) {
        return 0;
    }
    if (key == temp->key) {
        return temp->value;
    }
    else if (key < temp->key) {
        return search_helper(temp->left, key);
    }
    else if (key > temp->key) {
        return search_helper(temp->right, key);
    }
    return 0;
}

/*
    ����� ����������� ������� � ������
*/

AANode* AATree::min() {
    AANode* N = root;
    if (N != nullptr) {
        while (N->left != nullptr) {
            N = N->left;
        }
    }
    return N;
}

/*
    ����� ������������ ������� � ������
*/

AANode* AATree::max() {
    AANode* N = root;
    if (N != nullptr) {
        while (N->right != nullptr) {
            N = N->right;
        }
    }
    return N;
}

bool is_punctuation(char symbol) {
    if (ispunct(symbol) || isspace(symbol)) {
        return true;
    }
    return false;
}

vector<string> split_string(string& str) {
    vector<string> words;
    string temp;
    for (size_t i = 0; i < str.size(); i++) {
        char letter = str[i];
        if (!is_punctuation(letter)) {
            if (isupper(letter)) {
                letter = tolower(letter);
            }
            temp += letter;
        }
        else {
            if (temp.length() > 0) {
                words.push_back(temp);
            }
            temp.clear();
        }
    }
    if (temp != "") {
        words.push_back(temp);
    }
    return words;
}

string generate_word() {
    const char* letters[] = { "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "a", "s", "d",
                        "f", "g", "h", "j", "k", "l", "z", "x", "c", "v", "b", "n", "m" };
    int length_of_word = (rand() % 7) + 1;
    string word = "";
    for (int j = 0; j < length_of_word; j++) {
        word += letters[rand() % (sizeof letters / sizeof(char*))];
    }
    return word;
}

void task_test(vector<string>& words, int number_of_test) {
    cout << "Test #" << number_of_test << endl;
    cout << "Input data size: " << number_of_test * 200 << " elements" << endl;
    for (int j = 0; j < 5; j++) {
        auto start = high_resolution_clock::now();
        AATree* tree = new AATree();
        for (int i = 0; i < words.size() - 1; i++) {
            int count = tree->search(words[i]) + 1;
            tree->insert(words[i], count);
        }
        auto startSearch = high_resolution_clock::now();
        int count = tree->search(words[words.size() - 1]) + 1;
        auto stopSearch = high_resolution_clock::now();
        cout << "search: " << duration_cast<microseconds>(stopSearch - startSearch).count() << "; ";
        auto startInsert = high_resolution_clock::now();
        tree->insert(words[words.size() - 1], count);
        auto stopInsert = high_resolution_clock::now();
        cout << "insert: " << duration_cast<microseconds>(stopInsert - startInsert).count() << "; ";
        auto startMin = high_resolution_clock::now();
        AANode* min_node = tree->min();
        auto stopMin = high_resolution_clock::now();
        cout << "min: " << duration_cast<microseconds>(stopMin - startMin).count() << "; ";
        if (min_node != nullptr) {
            //cout << min_node->key << ": " << min_node->value << endl;
            auto startRemove = high_resolution_clock::now();
            tree->remove(min_node->key);
            auto stopRemove = high_resolution_clock::now();
            cout << "remove: " << duration_cast<microseconds>(stopRemove - startRemove).count() << "; ";
            min_node = tree->min();
        }
        while (min_node != nullptr) {
            //cout << min_node->key << ": " << min_node->value << endl;
            tree->remove(min_node->key);
            min_node = tree->min();
        }
        tree->~AATree();
        auto stop = high_resolution_clock::now();
        cout << "task: " << duration_cast<microseconds>(stop - start).count() << endl;
    }
}

int main() {
    vector<string> words;
    //string text;
    //getline(cin, text);
    //words = split_string(text);
    for (int test = 1; test <= 50; test++) {
        for (int i = 0; i < 200; i++) {
            words.push_back(generate_word());
        }
        task_test(words, test);
    }
    return 0;
}
