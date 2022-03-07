#include<iostream>
#include <fstream>

using namespace std;

class Node {
public:
    string english;
    string russian;
    Node *left;
    Node *right;

    Node(string a, string b);

    void insert(string a, string b);

    void print();
};

Node::Node(string a, string b) : english(a), russian(b), left(nullptr), right(nullptr) {}

void Node::insert(string a, string b) {
    if (a > english && right) right->insert(a, b);
    else if (a > english && !right) right = new Node(a, b);
    else if (a < english && left) left->insert(a, b);
    else left = new Node(a, b);
}

void Node::print() {
    if (left) left->print();
    cout << english << " - " << russian << "\n";
    if (right) right->print();
}

class Tree {
public:
    Node *root;

    Tree() : root(nullptr) {}

    void insert(string, string);

    void remove(string);

    void print();

    void deleteTree(Node *current);

    string find(string);

    void import(string);

    ~Tree();
};

void Tree::insert(string a, string b) {
    if (!root) root = new Node(a, b);
    else root->insert(a, b);
}

void Tree::print() {
    root->print();
}

void Tree::remove(string key) {
    Node *current = root;
    Node *parent = nullptr;
    while (current && current->english != key) {
        parent = current;
        if (current->english > key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (!current)
        return;
    if (current->left == nullptr) {
        // Вместо current подвешивается его правое поддерево
        if (parent) {
            if (parent->left == current)
                parent->left = current->right;
            if (parent->right == current)
                parent->right = current->right;
        } else {
            root = current->right;
        }
        delete current;
        return;
    }
    if (current->right == nullptr) {
        // Вместо current подвешивается его левое поддерево
        if (parent) {
            if (parent->left == current)
                parent->left = current->left;
            if (parent->right == current)
                parent->right = current->left;
        } else {
            root = current->left;
        }
        delete current;
        return;
    }

    Node *replace = current->right;
    while (replace->left)
        replace = replace->left;
    string replaceEnglish = replace->english;
    string replaceRussian = replace->russian;
    remove(replaceEnglish);
    current->english = replaceEnglish;
    current->russian = replaceRussian;

}

Tree::~Tree() {
    deleteTree(root);
}

void Tree::deleteTree(Node *current) {
    if (current) {
        deleteTree(current->left);
        deleteTree(current->right);
        delete current;
    }
}

string Tree::find(string key) {
    Node *current = root;
    while (current && current->english != key) {
        if (current->english > key)
            current = current->left;
        else
            current = current->right;
    }
    if (current != nullptr) {
        return current->russian;
    } else {
        return "перевод не найден";
    }
}

void Tree::import(string filePath) {
    int countOfImport = 0;
    string line;

    ifstream in(filePath);
    if (in.is_open()) {
        while (getline(in, line)) {
            int delimiterPosition = line.find(":");
            int lenghtOfWord = line.length();
            if (delimiterPosition > 0 && delimiterPosition != lenghtOfWord) {
                string english = line.substr(0, delimiterPosition);
                string russian = line.substr(delimiterPosition + 1, lenghtOfWord - 1);
                insert(english, russian);
                countOfImport++;
            }
        }
        cout << "Импортировано слов: " << countOfImport << endl;
    } else {
        cout << "Ошибка в открытии файла" << endl;
    }
    in.close();     // закрываем файл
}

int main() {
    Tree dict;

    int choice;
    do {
        cout << "Введите: " << endl;
        cout << "1| Добавить слово в словарь" << endl;
        cout << "2| Импортировать из файла" << endl;
        cout << "3| Поиск по ключу" << endl;
        cout << "4| Удалить элемент" << endl;
        cout << "5| Последовательный доступ" << endl;
        cout << "0| Выход" << endl;
        cin >> choice;

        switch (choice) {
            case 1: {
                string newEnglish;
                string newRussian;
                cout << "Введите английское слово: " << endl;
                cin >> newEnglish;
                cout << "Введите его перевод: " << endl;
                cin >> newRussian;
                dict.insert(newEnglish, newRussian);
            };
                break;
            case 2: {
                string filePath;
                cout << "Введите путь до файла: " << endl;
                cin >> filePath;
                dict.import(filePath);
            }
                break;
            case 3: {
                string wordSearch;
                cout << "Введите слово для поиска: " << endl;
                cin >> wordSearch;
                cout << dict.find(wordSearch) << endl;
            };
                break;
            case 4: {
                string wordDelete;
                cout << "Введите слово для удаления: " << endl;
                cin >> wordDelete;
                dict.remove(wordDelete);
            };
                break;
            case 5:
                dict.print();
                break;
            case 0:
                break;
            default:
                cout << "Введите номер из предложенных" << endl;
                break;
        }
    } while (choice != 0);
    return 0;
}