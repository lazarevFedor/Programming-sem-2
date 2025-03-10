#include <iostream>
#include <Windows.h>
#include <ctime>
#include <chrono>
#include <limits>
#undef max
using namespace std;
using namespace chrono;


//Обычные функции
void menu(){
    cout << "1) Заполнить список\n" <<
         "2) Действия со списоком\n" <<
         "3) Вывести список\n" <<
         "4) Заполнить массив\n" <<
         "5) Действия с массивом\n" <<
         "6) Вывести массив\n" <<
         "7) Очистить экран\n" <<
         "8) Время\n" <<
         "9) Выход\n-->> ";
}


void editMenu(){
    cout << "\n1) Вставить элемент\n" <<
         "2) Удалить элемент по индексу\n" <<
         "3) Удалить элемент по значению\n" <<
         "4) Обмен элементов\n" <<
         "5) Получить элемент по индексу\n" <<
         "6) Получить элемент по значению\n" <<
         "7) Сортировка\n-->> ";
}


int randint(){
    return rand() % 100;
}


void clearStream() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.sync();
}

//Структура элемента списка, структура списка
struct Node {
    int data;
    Node* prev, *next;
    Node(int inputData) {
        data = inputData;
        prev = next = nullptr;
    }
};


struct LinkedList{
    Node* head, * tail;

    LinkedList(): head(nullptr), tail(nullptr){}

    void pushFront(int data){
        Node* ptr = new Node(data);
        ptr->next = head;
        if (head != nullptr){
            head->prev = ptr;
        }
        if (tail == nullptr){ //Если объект самый первый tail == nullptr
            tail = ptr;
        }
        head = ptr;
    }

    void pushBack(int data){
        Node* ptr = new Node(data);
        ptr->prev = tail;
        if (tail != nullptr){
            tail->next = ptr;
        }
        if (head == nullptr){ //Если объект самый первый head == nullptr
            head = ptr;
        }
        tail = ptr;
    }

    void popFront(){
        if(head == nullptr) return; //Если нет элементов, то удалять нечего
        Node* ptr = head->next;
        if(ptr != nullptr){
            ptr->prev = nullptr;
        }
        else //если единственный объект
            tail = nullptr;
        delete head;
        head = ptr;
    }

    void popBack(){
        if(tail == nullptr) return; //Если нет элементов, то удалять нечего
        Node* ptr = tail->prev;
        if(ptr != nullptr){
            ptr->next = nullptr;
        }
        else //если единственный объект
            head = nullptr;
        delete tail;
        tail = ptr;
    }

    Node* getAt(int index){
        Node* ptr = head;
        int n = 0;
        while(n != index){
            if (ptr == nullptr) return nullptr;//если нет объектов или дошли до конца списка
            ptr = ptr->next;
            n++;
        }
        return ptr;
    }

    void insert (int index, int data){
        Node* right = getAt(index);
        if (right == nullptr){
            pushBack(data);
            return;
        }
        Node* left = right->prev;
        if (left == nullptr) {
            pushFront(data);
            return;
        }
        Node* ptr = new Node(data);
        ptr->prev = left;
        ptr->next = right;
        left->next = ptr;
        right->prev = ptr;
    }

    void eraseByIndex(int index){
        Node* ptr = getAt(index);
        if (ptr == nullptr){
            cout << "\nНет элемента для удаления!";
            return;
        }
        if (ptr->prev == nullptr){ //если удаляемый элемент первый
            popFront();
            return;
        }
        if (ptr->next == nullptr){ //если удаляемый элемент последний
            popBack();
            return;
        }
        Node* left = ptr->prev;
        Node* right = ptr->next;
        left->next = right;
        right->prev = left;
        delete ptr;
    }

    void eraseByValue(int number){
        Node* ptr = head;
        while(ptr != nullptr && ptr->data != number){
            ptr = ptr->next;
        }
        if (ptr == nullptr){
            cout << "\nНет элемента для удаления!";
            return;
        }
        if (ptr->prev == nullptr){ //если удаляемый элемент первый
            popFront();
            return;
        }
        if (ptr->next == nullptr){ //если удаляемый элемент последний
            popBack();
            return;
        }
        Node* left = ptr->prev;
        Node* right = ptr->next;
        left->next = right;
        right->prev = left;
        delete ptr;
    }

    void swapElements(int index1, int index2){
        Node* ptr1 = getAt(index1);
        Node* ptr2 = getAt(index2);
        Node* left1 = getAt(index1-1);
        Node* left2 = getAt(index2-1);
        Node* right1 = getAt(index1+1);
        Node* right2 = getAt(index2+1);

        if (ptr1 == nullptr) return; // Указатель не существует
        else if (ptr2 == nullptr) return; // Указатель не существует
        else if (index1 == index2) return;
        if (left1 != nullptr) {
            left1->next = ptr2;
        } else {
            head = ptr2;
        }
        if (left2 != nullptr) {
            left2->next = ptr1;
        } else {
            head = ptr1;
        }
        if (right1 != nullptr) {
            right1->prev = ptr2;
        } else{
            tail = ptr2;
        }
        if (right2 != nullptr) {
            right2->prev = ptr1;
        } else{
            tail = ptr1;
        }
        // Обновление указателей next и prev у самих элементов
        Node* temp = ptr1->next;
        ptr1->next = ptr2->next;
        ptr2->next = temp;
        temp = ptr1->prev;
        ptr1->prev = ptr2->prev;
        ptr2->prev = temp;
    }

    void printList(){
        cout << "\nСписок: ";
        for (Node *ptr = head; ptr != nullptr; ptr = ptr->next)
            cout << ptr->data << " ";
        cout << "\n";
    }

    int findInList(int number){
        int id = 0;
        for (Node *ptr = head; ptr != nullptr; ptr = ptr->next){
            if (ptr->data == number){
                return id;
            }
            id++;
        }
        return -1;
    }

    int elementsInList(){
        int num = 0;
        for (Node *ptr = head; ptr != nullptr; ptr = ptr->next){
            num++;
        }
        return num;
    }

    void shakerList(int &countInList){
        bool swapped = true;
        int start = 0;
        int end = countInList - 1;
        while (swapped) {
            swapped = false;
            for (int i = start; i < end; ++i) {
                if (getAt(i)->data > getAt(i+1)->data){
                    swapElements(i, i+1);
                    swapped = true;
                }
            }
            if (!swapped) {
                break;
            }
            swapped = false;
            --end;
            for (int i = end - 1; i >= start; --i) {
                if (getAt(i)->data > getAt(i+1)->data) {
                    swapElements(i, i+1);
                    swapped = true;
                }
            }
            ++start;
        }
    }

    void randList(int &lenght){
        for (int i = 0; i < lenght; i++){
            pushBack(randint());
        }
    }

    void fillList(){
        int number;
        while (cin >> number) {
            pushBack(number);
            if (cin.peek() == '\n') {
                break;
            }
        }
        clearStream();
    }

    void clearList(){
        while(head != nullptr) popBack();
    }
};

//Функции динамического массива
void randArray(int &sizeArr, int* &arr){
    int  *rez = new int[sizeArr];
    for (int i = 0; i < sizeArr; i++){
        *(rez + i) = randint();
    }
    delete[] arr;
    arr = rez;
}


void printArray(int &sizeArr, int* &arr){
    cout << "\nМассив: ";
    for (int i = 0; i < sizeArr; i++){
        cout << arr[i] << " ";
    }
    cout << "\n";
}


void addToArray(int &sizeArr, int* &arr, int &id, int& number){
    if (id < 0 || id > sizeArr) return;
    int  *rez = new int[sizeArr+1];
    for (int i = 0; i < id; i++) {
        rez[i] = arr[i];
    }
    rez[id] = number;
    for (int i = id; i < sizeArr; i++) {
        rez[i+1] = arr[i];
    }
    sizeArr++;
    delete[] arr;
    arr = rez;
}


void fillArray(int &sizeArr, int* &arr, int &number){
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
}


void deleteByIndex(int &sizeArr, int* &arr, int& id){
    int  *rez = new int[sizeArr-1];
    int j = 0;
    for (int i = 0; i < sizeArr; i++) {
        if (i != id) {
            *(rez + j) = *(arr + i);
            j++;
        }
    }
    if (j < sizeArr){
        sizeArr--;
        delete[] arr;
        arr = rez;
    }
    else cout << "Нет элемента для удаления!\n";
}


void deleteByValue(int &sizeArr, int* &arr, int& number){
    int  *rez = new int[sizeArr-1];
    int j = 0;
    for (int i = 0; i < sizeArr; i++) {
        if (*(arr + i) != number) {
            *(rez + j) = *(arr + i);
            j++;
        }
    }
    if (j < sizeArr){
        sizeArr--;
        delete[] arr;
        arr = rez;
    }
    else cout << "Нет элемента для удаления!";
}


void swapInArray(int &sizeArr, int* &arr, int index1, int index2){
    if (index1 < 0 || index2 < 0) return;
    if (index1 >= sizeArr || index2 >= sizeArr) return;
    swap(*(arr + index1), *(arr + index2));
}


void getByValue(int &sizeArr, int* &arr,int &number){
    for (int i = 0; i < sizeArr; i++){
        if (*(arr + i) == number){
            cout << "Индекс: " << i << "\n";
            return;
        }
    }
    cout << "\nЗначение не найдено!\n";
}


void clearArray(int &sizeArr, int* &arr){
    sizeArr = 0;
    int  *rez = new int[0];
    delete[] arr;
    arr = rez;
}


void shakerArray(int* &arr, int &sizeArr) {
    bool swapped = true;
    int start = 0;
    int end = sizeArr - 1;
    while (swapped) {
        swapped = false;
        // Проход слева направо
        for (int i = start; i < end; ++i) {
            if (arr[i] > arr[i + 1]){
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
        swapped = false;
        --end;
        // Проход справа налево
        for (int i = end - 1; i >= start; --i) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                swapped = true;
            }
        }
        ++start;
    }
}


// Структуры для записи времени

struct Time{
    long unsigned int fill = 0;
    long unsigned int rand = 0;
    unsigned int insert = 0;
    unsigned int eraseById = 0;
    unsigned int eraseByVal = 0;
    unsigned int getById = 0;
    unsigned int getByVal = 0;
    unsigned long int sort = 0;
    void showInfo(){
        cout << "\nСоздание: " << rand <<
        "\nЗаполнение: " << fill <<
        "\nВставка: " << insert <<
        "\nУдаление по индексу: " << eraseById <<
        "\nУдаление по значению: " << eraseByVal <<
        "\nПолучение по индексу: " << getById <<
        "\nПолучение по значению: " << getByVal <<
        "\nСортировка: " << sort << "\n\n";
    }
};

//.........................................................
int main() {
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));
    time_point<steady_clock, duration<__int64, ratio<1, 1000000000>>> start, end;
    nanoseconds result;
    Time arrayTime, listTime;
    int choise, sizeArr = 0, id, number, countInList;
    int* arr = new int [sizeArr];
    LinkedList list;
    while(true){
        menu();
        cin >> choise;
        switch (choise){
            case 1:
                cout << "1) Зарандомить\n" <<
                     "2) Ввести ручками\n-->> ";
                cin >> choise;
                if (choise == 1){
                    list.clearList();
                    int lenght = 0;
                    cout << "Введите количество элементов -->> ";
                    cin >> lenght;
                    clearStream();
                    start = steady_clock::now();
                    list.randList(lenght);
                    end = steady_clock::now();
                    result = duration_cast<nanoseconds>(end - start);
                    listTime.rand = result.count();
                    list.printList();
                }
                else if (choise == 2){
                    list.clearList();
                    cout << "Список: ";
                    start = steady_clock::now();
                    list.fillList();
                    end = steady_clock::now();
                    result = duration_cast<nanoseconds>(end - start);
                    listTime.fill = result.count();
                }
                break;
            case 2:
                Node* pointer;
                editMenu();
                cin >> choise;
                switch(choise){
                    case 1:
                        cout << "Индекс: ";
                        cin >> id;
                        cout << "Значение: ";
                        cin >> number;
                        start = steady_clock::now();
                        list.insert(id, number);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        listTime.insert = result.count();
                        list.printList();
                        break;
                    case 2:
                        cout << "Индекс: ";
                        cin >> id;
                        start = steady_clock::now();
                        list.eraseByIndex(id);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        listTime.eraseById = result.count();
                        list.printList();
                        break;
                    case 3:
                        cout << "Значение: ";
                        cin >> number;
                        start = steady_clock::now();
                        list.eraseByValue(number);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        listTime.eraseByVal = result.count();
                        list.printList();
                        break;
                    case 4:
                        cout << "Индекс1: ";
                        cin >> id;
                        cout << "Индекс2: ";
                        cin >> number;
                        list.swapElements(id, number);
                        list.printList();
                        break;
                    case 5:
                        cout << "Индекс: ";
                        cin >> id;
                        start = steady_clock::now();
                        pointer = list.getAt(id);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        if (pointer != nullptr){
                            cout << "Значение: " << pointer->data << "\n";
                            listTime.getById = result.count();
                        }
                        else cout << "\nНеправильно введён индекс!\n\n";
                        break;
                    case 6:
                        cout << "Значение: ";
                        cin >> number;
                        start = steady_clock::now();
                        id = list.findInList(number);
                        end = steady_clock::now();
                        if (id == -1){
                            cout << "Такого значения нет!\n\n";
                        }
                        else{
                            cout << "Индекс: " << id << "\n";
                            result = duration_cast<nanoseconds>(end - start);
                            listTime.getByVal = result.count();
                        }
                        break;
                    case 7:
                        countInList = list.elementsInList();
                        start = steady_clock::now();
                        list.shakerList(countInList);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        list.printList();
                        listTime.sort = result.count();
                        break;
                    default:
                        cout << "Неправильно введен номер!!!\n\n";
                        break;
                }
                break;
            case 3:
                list.printList();
                break;
            case 4:
                clearArray(sizeArr, arr);
                cout << "1) Зарандомить\n" <<
                     "2) Ввести ручками\n-->> ";
                cin >> choise;
                if (choise == 1){
                    cout << "Введите размер массива\n-->> ";
                    cin >> sizeArr;
                    clearStream();
                    start = steady_clock::now();
                    randArray(sizeArr, arr);
                    end = steady_clock::now();
                    printArray(sizeArr, arr);
                    result = duration_cast<nanoseconds>(end - start);
                    arrayTime.rand = result.count();
                }
                else if (choise == 2){
                    cout << "Массив: ";
                    start = steady_clock::now();
                    fillArray(sizeArr, arr, number);
                    end = steady_clock::now();
                    result = duration_cast<nanoseconds>(end - start);
                    arrayTime.fill = result.count();
                }
                break;
            case 5:
                editMenu();
                cin >> choise;
                switch(choise){
                    case 1:
                        cout << "Индекс: ";
                        cin >> id;
                        cout << "Значение: ";
                        cin >> number;
                        start = steady_clock::now();
                        addToArray(sizeArr, arr, id, number);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        arrayTime.insert = result.count();
                        printArray(sizeArr, arr);
                        break;
                    case 2:
                        cout << "Индекс: ";
                        cin >> id;
                        start = steady_clock::now();
                        deleteByIndex(sizeArr, arr, id);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        arrayTime.eraseById = result.count();
                        printArray(sizeArr, arr);
                        break;
                    case 3:
                        if (sizeArr == 0){
                            cout << "Массив пустой!\n";
                            break;
                        }
                        cout << "Значение: ";
                        cin >> number;
                        start = steady_clock::now();
                        deleteByValue(sizeArr, arr, number);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        arrayTime.eraseByVal = result.count();
                        printArray(sizeArr, arr);
                        break;
                    case 4:
                        cout << "Индекс1: ";
                        cin >> id;
                        cout << "Индекс2: ";
                        cin >> number;
                        swapInArray(sizeArr, arr, id, number);
                        break;
                    case 5:
                        cout << "Индекс: ";
                        cin >> id;
                        if (id < 0 || id >= sizeArr){
                            cout << "\nНеправильно введён индекс!\n\n";
                        }
                        else{
                            start = steady_clock::now();
                            number = arr[id];
                            end = steady_clock::now();
                            cout << "Значение: " << number << "\n";
                            result = duration_cast<nanoseconds>(end - start);
                            arrayTime.getById = result.count();
                        }
                        break;
                    case 6:
                        cout << "Значение: ";
                        cin >> number;
                        start = steady_clock::now();
                        getByValue(sizeArr, arr, number);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        arrayTime.getByVal = result.count();
                        break;
                    case 7:
                        start = steady_clock::now();
                        shakerArray(arr, sizeArr);
                        end = steady_clock::now();
                        result = duration_cast<nanoseconds>(end - start);
                        arrayTime.sort = result.count();
                        printArray(sizeArr, arr);
                        break;
                    default:
                        cout << "Неправильно введён номер!\n\n";
                        break;
                }
                break;
            case 6:
                printArray(sizeArr, arr);
                break;
            case 7:
                system("cls");
                break;
            case 8:
                cout << "\nСписок:";
                listTime.showInfo();
                cout << "Массив:";
                arrayTime.showInfo();
                break;
            case 9:
                list.clearList();
                delete [] arr;
                arr = nullptr;
                exit(0);
            default:
                cout << "\nНеправильно ввёден номер!\n\n";
                break;
        }
    }
}
