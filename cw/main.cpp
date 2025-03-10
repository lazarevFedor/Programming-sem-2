#include <iostream>
#include <Windows.h>
#include <limits>
#include <ctime>
#include <chrono>
#include <fstream>
#undef max
using namespace std;
using namespace chrono;


void clearStream() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.sync();
}


int randint(bool randArrSize){
    if(randArrSize){
        return rand() % 15 + 10;
    }
    return rand() % 198 - 99;
}


void mainMenu(){
    cout << "1) Сформировать дерево\n" <<
         "2) Действия с деревом\n" <<
         "3) Проверка на сбалансированность\n" <<
         "4) Генерация заданий\n" <<
         "5) Время выполнения\n" <<
         "6) Выход\n-->> ";
}


void actionMenu(){
    cout << "1) Вставка\n" <<
         "2) Удаление\n" <<
         "3) Получение\n-->> ";
}

//..........................................................................
struct Time{
    int rand = 0;
    int fill = 0;
    int insert = 0;
    int erase = 0;
    int search = 0;
    int balance = 0;
    void print(){
        cout << "\nВремя:";
        if (rand) cout << "\nЗаполнения случайными числами = " << rand;
        if (fill) cout << "\nЗаполения числами с клавиатуры = " << fill;
        if (insert) cout << "\nВставки = " << insert;
        if (erase) cout << "\nУдаления = " << erase;
        if (search) cout << "\nПолучения = " << search;
        if (balance) cout << "\nПроверки на сбалансированность = " << balance;
        cout << "\n";
    }
};

struct Node {
    int key;
    int height = 0;
    Node* left;
    Node* right;
    Node(int inputData){
        key = inputData;
        left = right = nullptr;
    }
};

struct trunk{
    string str;
    trunk* prev;
    trunk(trunk* prevAdr, string prev_str){
        str = prev_str;
        prev = prevAdr;
    }
    void showTrunk(trunk* p, int &count, ofstream &fout){
        if(p == nullptr) return;
        showTrunk(p->prev, count, fout);
        count++;
        fout << p->str;
    }
};

struct Tree {
    Node* root;

    void createTree(){
        root = nullptr;
    }

    void updHeight(Node* node){
        if (node == nullptr) return;
        node->height = (getHeight(node->left) >= getHeight(node->right)) ? getHeight(node->left) + 1 : getHeight(node->right) + 1;
    }

    int getHeight(Node* node){
        return node == nullptr ? -1 : node->height;
    }

    int getBalance(Node* node){
        return(node == nullptr) ? 0 : getHeight(node->right) - getHeight(node->left);
    }

    void rotateRight(Node* &node){
        Node* lst = node->left;
        Node* lrst = lst->right;
        lst->right = node;
        node->left = lrst;
        node = lst;
        updHeight(node->right);
        updHeight(node);
    }

    void rotateLeft(Node* &node){
        Node* rst = node->right;
        Node* rlst = rst->left;
        rst->left = node;
        node->right = rlst;
        node = rst;
        updHeight(node->left);
        updHeight(node);
    }

    Node* balance(Node* node){
        updHeight(node);
        if(getBalance(node) == 2){
            if(getBalance(node->right) < 0)
                rotateRight(node->right);
            rotateLeft(node);
        }
        if(getBalance(node) == -2){
            if(getBalance(node->left) > 0)
                rotateLeft(node->left);
            rotateRight(node);
        }
        return node;
    }

    Node* insert(Node* node, int key){
        if(!node) return new Node(key);
        if( key < node->key )
            node->left = insert(node->left,key);
        else
            node->right = insert(node->right,key);
        return balance(node);
    }

    Node* deleteNode(Node* node, int key){
        if(node == nullptr) return nullptr;
        if(key > node->key) node->right = deleteNode(node->right, key);
        else if(key < node->key) node->left = deleteNode(node->left, key);
        else{
            if(node->left == nullptr || node->right == nullptr){
                if(!node->left && !node->right){
                    delete node;
                    return nullptr;
                }
                else if(!node->left){
                    Node* tmp = node->right;
                    delete node;
                    return balance(tmp);
                }
                else if(!node->right){
                    Node* tmp = node->left;
                    delete node;
                    return balance(tmp);
                }
            }
            else{
                Node* tmp = getMaxTree(node->left);
                node->key = tmp->key;
                node->left = deleteNode(node->left, tmp->key);
                return balance(node);
            }
        }
        if(node != nullptr){
            updHeight(node);
        }
        return balance(node);
    }

    void deleteTree(Node* &ptr){
        if (ptr != nullptr){
            deleteTree(ptr->right);
            deleteTree(ptr->left);
            delete ptr;
        }
    }

    Node* search(int key, Node* ptr){
        if (ptr == nullptr) return nullptr;
        if(key == ptr->key) return ptr;
        else if (key < ptr->key) return search(key, ptr->left);
        else return search(key, ptr->right);
        return ptr;
    }

    Node* getMinTree(Node* ptr) {
        while (ptr->left) {
            ptr = ptr->left;
        }
        return ptr;
    }

    Node* getMaxTree(Node* ptr) {
        while (ptr->right) {
            ptr = ptr->right;
        }
        return ptr;
    }

    void printTree(Node* node, trunk* prev, bool isRight, ofstream &fout){
        if(node == nullptr) return;
        string prev_str = "    ";
        trunk* tmp = new trunk(prev, prev_str);
        printTree(node->right, tmp, true, fout);
        if (!prev)
            tmp->str = "-->";
        else if (isRight) {
            tmp->str = ".-->";
            prev_str = "   |";
        }
        else {
            tmp->str = "`-->";
            prev->str = prev_str;
        }
        int count = 0;
        tmp->showTrunk(tmp, count, fout);
        fout << "{" << node->key << "}" << "\n";
        if (prev)
            prev->str = prev_str;
        tmp->str = "   |";
        printTree(node->left, tmp, false, fout);
    }

    void directBypass(Node* node, bool &isBalance, bool balancing) {
        if (node && isBalance && !balancing) {
            if(getBalance(node) == 2 || getBalance(node) == -2){
                isBalance = false;
            }
            directBypass(node->left, isBalance, balancing);
            directBypass(node->right, isBalance, balancing);
        }
    }

    void fillTree(int numOfNodes, int* arr, bool rand){
        if(rand){
            for (int i = 0; i < numOfNodes; i++){
                root = insert(root, randint(false));
            }
        }
        else{
            for (int i = 0; i < numOfNodes; i++){
                root = insert(root, arr[i]);
            }
        }
    }
};

//.................................................................
int fillArray(int* &arr){
    int number;
    int sizeArr = 0;
    while (cin >> number){
        int  *rez = new int[sizeArr+1];
        for (int i = 0; i < sizeArr; i++) {
            rez[i] = arr[i];
        }
        rez[sizeArr] = number;
        sizeArr++;
        delete[] arr;
        arr = rez;
        if (cin.peek() == '\n') {
            break;
        }
    }
    clearStream();
    return sizeArr;
}


int randArray(int &sizeArr, int* &arr){
    int  *rez = new int[sizeArr];
    for (int i = 0; i < sizeArr; i++){
        *(rez + i) = randint(false);
    }
    delete[] arr;
    arr = rez;
    return sizeArr;
}


void clearArray(int* &arr){
    int  *rez = new int[0];
    delete[] arr;
    arr = rez;
}

//................................................................
void fillAndLogTree(Tree &avl, int numOfNodes, int* arr, ofstream &taskAns){
    for (int i = 0; i < numOfNodes; i++){
        avl.root = avl.insert(avl.root, arr[i]);
        avl.printTree(avl.root, nullptr, true, taskAns);
        taskAns << "\n\n";
    }
}


Node* insertAndLog(Tree &avl, Node* node, int key, ofstream &taskAns){
    if(!node) return new Node(key);
    if(key < node->key)
        node->left = avl.insert(node->left,key);
    else
        node->right = avl.insert(node->right,key);
    avl.printTree(avl.root, nullptr, true, taskAns);
    taskAns << "\n\n";
    return avl.balance(node);
}


Node* deleteNode(Tree &avl, Node* node, int key, ofstream &taskAns){
    if(node == nullptr) return nullptr;
    if(key > node->key) node->right = deleteNode(avl, node->right, key, taskAns);
    else if(key < node->key) node->left = deleteNode(avl, node->left, key, taskAns);
    else{
        if(node->left == nullptr || node->right == nullptr){
            if(!node->left && !node->right){
                delete node;
                return nullptr;
            }
            else if(!node->left){
                Node* tmp = node->right;
                delete node;
                return avl.balance(tmp);
            }
            else if(!node->right){
                Node* tmp = node->left;
                delete node;
                return avl.balance(tmp);
            }
        }
        else{
            Node* tmp = avl.getMaxTree(node->left);
            node->key = tmp->key;
            node->left = deleteNode(avl, node->left, tmp->key, taskAns);
            return avl.balance(node);
        }
    }
    if(node != nullptr){
        avl.updHeight(node);
    }
    avl.printTree(avl.root, nullptr, true, taskAns);
    taskAns << "\n\n";
    return avl.balance(node);
}


void completeTask(ofstream &task, ofstream &taskKey, ofstream &taskAns, int* &arr, Tree &avl, Time &time){
    avl.createTree();
    int numOfNodes;
    short choise;
    short key;
    cout << "\nВведите количество вариантов: ";
    cin >> choise;
    if (!cin.good()) {
        cout << "Неправильно введено число.\n";
        system("pause");
        clearStream();
        return;
    }
    task.open("C:\\Users\\fedos\\CLionProjects\\AVL-tree\\output_task.txt");
    taskKey.open("C:\\Users\\fedos\\CLionProjects\\AVL-tree\\output_key.txt");
    taskAns.open("C:\\Users\\fedos\\CLionProjects\\AVL-tree\\output_ans.txt");
    if (!task.is_open() || !taskKey.is_open() || !taskAns.is_open()){
        cout << "Ошибка открытия файла!!!";
        system("pause");
        task.close();
        taskKey.close();
        taskAns.close();
        return;
    }
    for (int i = 1; i <= choise; i++){
        clearArray(arr);
        avl.deleteTree(avl.root);
        avl.createTree();
        task << "ВАРИАНТ № " << i << "\n\n";
        taskKey << "ВАРИАНТ № " << i << "\n";
        taskAns << "ВАРИАНТ № " << i << "\n";

        task << "Задание № 1\nСоздать АВЛ-дерево из следующего массива:";
        taskKey << "Задание № 1\n";
        taskAns << "Задание № 1\n";
        numOfNodes = randint(true);
        randArray(numOfNodes, arr);
        for (int j = 0; j < numOfNodes; j++){
            task << " " << arr[j];
        }
        fillAndLogTree(avl, numOfNodes, arr, taskAns);
        taskAns << "Ответ:\n";
        avl.printTree(avl.root, nullptr, true, taskAns);
        taskAns << "\n";
        avl.printTree(avl.root, nullptr, true, taskKey);

        key = randint(false);
        task << "\nЗадание № 2\nВставить в дерево элемент с ключом: " << key << "\n";
        taskKey << "\nЗадание № 2\n";
        taskAns << "\nЗадание № 2\n";
        avl.root = insertAndLog(avl, avl.root, key, taskAns);
        taskAns << "Ответ:\n";
        avl.printTree(avl.root, nullptr, true, taskAns);
        taskAns << "\n";
        avl.printTree(avl.root, nullptr, true, taskKey);

        key = randint(true);
        task << "Задание № 3\nУдалить из дерева элемент с ключом: " << arr[key] << "\n\n";
        taskKey << "\nЗадание № 3\n";
        taskAns << "\nЗадание № 3\n";
        avl.root = deleteNode(avl, avl.root, arr[key], taskAns);
        taskAns << "Ответ:\n";
        avl.printTree(avl.root, nullptr, true, taskAns);
        avl.printTree(avl.root, nullptr, true, taskKey);
        taskKey << "\n\n";
        taskAns << "\n\n";
    }
    task.close();
    taskKey.close();
    taskAns.close();
}
//.................................................................
int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));
    ofstream fout, task,taskKey, taskAns;
    time_point<steady_clock, duration<__int64, ratio<1, 1000000000>>> start, end;
    nanoseconds result;
    Tree avl;
    Time time;
    avl.createTree();
    short choise;
    bool isBalance = true;
    int* arr = new int [0];
    while(true){
        system("cls");
        mainMenu();
        cin >> choise;
        clearStream();
        switch(choise){
            case 1:
                avl.deleteTree(avl.root);
                avl.createTree();
                clearArray(arr);
                cout << "\nВыберите способ заполнения:\n" <<
                     "1) Случайные числа\n" <<
                     "2) Ручной ввод\n-->> ";
                cin >> choise;
                clearStream();
                if(choise == 1){
                    cout << "\nВведите количество элементов: ";
                    cin >> choise;
                    start = steady_clock::now();
                    avl.fillTree(choise, nullptr, true);
                    end = steady_clock::now();
                    result = duration_cast<nanoseconds>(end - start);
                    time.rand = result.count();
                    fout.open("C:\\Users\\fedos\\CLionProjects\\AVL-tree\\Tree.txt");
                    if (!fout.is_open()) {
                        cout << "Ошибка открытия файла!!!";
                        fout.close();
                        break;
                    }
                    else{
                        avl.printTree(avl.root, nullptr, true, fout);
                        fout.close();
                    }
                }
                else if(choise == 2){
                    cout << "\nЗаполните массив: ";
                    choise = fillArray(arr);
                    start = steady_clock::now();
                    avl.fillTree(choise, arr, false);
                    end = steady_clock::now();
                    result = duration_cast<nanoseconds>(end - start);
                    time.fill = result.count();
                    fout.open("C:\\Users\\fedos\\CLionProjects\\AVL-tree\\Tree.txt");
                    if (!fout.is_open()) {
                        cout << "Ошибка открытия файла!!!";
                        fout.close();
                        break;
                    }
                    else{
                        avl.printTree(avl.root, nullptr, true, fout);
                        fout.close();
                    }
                }
                else {
                    cout << "\nНеправильно введен номер!\n";
                    system("pause");
                }
                break;
            case 2:
                actionMenu();
                cin >> choise;
                clearStream();
                switch(choise){
                    case 1:
                        cout << "\nВведите ключ: ";
                        cin >> choise;
                        if (!cin.good()) {
                            clearStream();
                            break;
                        }
                        start = steady_clock::now();
                        avl.root = avl.insert(avl.root, choise);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        time.insert = result.count();
                        fout.open("C:\\Users\\fedos\\CLionProjects\\AVL-tree\\Tree.txt");
                        if (!fout.is_open()) {
                            cout << "Ошибка открытия файла!!!";
                            fout.close();
                            break;
                        }
                        else{
                            avl.printTree(avl.root, nullptr, true, fout);
                            fout.close();
                        }
                        break;
                    case 2:
                        cout << "\nВведите ключ: ";
                        cin >> choise;
                        if (!cin.good()) {
                            clearStream();
                            break;
                        }
                        start = steady_clock::now();
                        avl.root = avl.deleteNode(avl.root, choise);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        time.erase = result.count();
                        fout.open("C:\\Users\\fedos\\CLionProjects\\AVL-tree\\Tree.txt");
                        if (!fout.is_open()) {
                            cout << "Ошибка открытия файла!!!";
                            fout.close();
                            break;
                        }
                        else{
                            avl.printTree(avl.root, nullptr, true, fout);
                            fout.close();
                        }
                        break;
                    case 3:
                        Node* adress;
                        cout << "\nВведите ключ: ";
                        cin >> choise;
                        if (!cin.good()) {
                            clearStream();
                            break;
                        }
                        start = steady_clock::now();
                        adress = avl.search(choise, avl.root);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        time.search = result.count();
                        if (adress) cout << "\nАдрес элемента: " << adress << "\n";
                        else cout << "\nЭлемент не найден в дереве.\n";
                        system("pause");
                        break;
                    default:
                        cout << "\nНеправильно введен номер!\n";
                        system("pause");
                }
                break;
            case 3:
                start = steady_clock::now();
                avl.directBypass(avl.root, isBalance, false);
                end = steady_clock::now();
                result = duration_cast<nanoseconds>(end - start);
                time.balance = result.count();
                if (isBalance){
                    cout << "\nДерево сбалансированно!\n";
                    system("pause");
                }
                else {
                    cout << "\nДерево не сбалансированно.\n";
                    system("pause");
                }
                isBalance = true;
                break;
            case 4:
                completeTask(task, taskKey, taskAns, arr, avl, time);
                clearArray(arr);
                break;
            case 5:
                time.print();
                system("pause");
                break;
            case 6:
                delete [] arr;
                arr = nullptr;
                avl.deleteTree(avl.root);
                exit(0);
                break;
            default:
                cout << "\nНеправильно введен номер!\n";
                system("pause");
        }
    }
    return 0;
}