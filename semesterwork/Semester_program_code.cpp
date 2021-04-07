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

// конструктор класса AATree
AATree::AATree() {
    root = new AANode;
    root = nullptr;
}

// деструктор класса AATree
AATree::~AATree() {
    dealloc_memory(root);
}

/*
    метод dealloc_memory освобождает память вершин дерева
*/

void AATree::dealloc_memory(AANode* N) {
    if (N == nullptr) {
        return;
    }
    dealloc_memory(N->left);
    dealloc_memory(N->right);
    delete N;
}

/*
    методы insert и insert_helper осуществляют вставку вершины в дерево
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
    методы remove и remove_helper осуществляют удаление текущей вершины в дереве
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
    реализация метода skew(AANode* temp), который устраняет левую связь на одном уровне;
    данный метод устроняет горизонтальную левую связь при помощи вращения узла вправо
    каждый раз, когда найдена горизонтальная левая связь

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
  реализация метода split(AANode* temp), который устраняет две правых связи на одном уровне;
  данный метод устроняет две последовательные правые горизонтальные связи при помощи вращения
  узла влево и увеличения уровня среднего узна на единицу

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
    метод balance осуществляет правильную вставку по алгоритму:
    1. добавляется новая вершина
    2. вызывается метод skew
    3. вызывается метод split
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
    методы search и search_helper осуществляют поиск нужного узла
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

AANode* AATree::min() {
    AANode* N = root;
    if (N != nullptr) {
        while (N->left != nullptr) {
            N = N->left;
        }
    }
    return N;
}

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

int main() {
    //string text;
    //getline(cin, text);
    vector<string> words;
    //words = split_string(text);
    const char* letters[] = {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "a", "s", "d", 
                        "f", "g", "h", "j", "k", "l", "z", "x", "c", "v", "b", "n", "m"};
    for (int test = 1; test <= 50; test++) {
        for (int i = 0; i < test * 200; i++) {
            int length_of_word = (rand() % 7) + 1;
            string word = "";
            for (int j = 0; j < length_of_word; j++) {
                word += letters[rand() % (sizeof letters / sizeof(char*))];
            }
            words.push_back(word);
        }
        auto start = high_resolution_clock::now();
        AATree* tree = new AATree();
        for (int i = 0; i < words.size(); i++) {
            int count = tree->search(words[i]) + 1;
            tree->insert(words[i], count);
        }
        AANode* min_node = tree->min();
        while (min_node != nullptr) {
            //cout << min_node->key << ": " << min_node->value << endl;
            tree->remove(min_node->key);
            min_node = tree->min();
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << test * 200 << " " << duration.count() << endl;
        words.clear();
        tree->~AATree();
    }
    return 0;    
}
