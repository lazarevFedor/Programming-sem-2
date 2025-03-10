#include <iostream>
#include <Windows.h>
#include <limits>
#include <string>
#undef max
using namespace std;


void sleepScreen(){
    Sleep(200);
}


void clearStream() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.sync();
}


void fillString(string &inputString){
    cout << "\nВведите строку: ";
    clearStream();
    getline(cin, inputString);
}


void mainMenu(){
    cout << "1) Преобразовать выражение\n" <<
    "2) Проверить корректность выражения\n" <<
    "3) Вычислить выражение\n" <<
    "4) Очистить консоль\n" <<
    "5) Выход\n-->> ";
}


void recordingMenu(){
    cout << "1) Обратная польская запись\n" <<
    "2) Польская запись\n" <<
    "3) Простая запись\n-->> ";
}


struct Node {
    string data;
    int prior;
    Node* next;
    Node(string inputData, int inputPrior) {
        data = inputData;
        prior = inputPrior;
        next = nullptr;
    }
};


struct Stack {
    Node* head, * tail;
    void createStack(){
        head = tail = nullptr;
    }

    void clearStack(){
        while (head != nullptr) popFront();
    }

    void popFront(){
        if (head == nullptr) return;
        if (head == tail){
            delete tail;
            head = tail = nullptr;
            return;
        }
        Node* node = head;
        head = node->next;
        delete node;
    }
    void pushFront(string data, int prior){
        Node* node = new Node(data, prior);
        node->next = head;
        head = node;
        if (tail == nullptr) tail = node;
    }
};

//функции для нотации
int priorRPN(char c){
    switch (c){
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '(':
        case ')':
            return 0;
        default:
            if (isdigit(c)) return -1;
            else if (c >= 'A' && c <= 'Z') return -1;
            else if (c >= 'a' && c <= 'z') return -1;
            else return 404;
    }
}


int priorPN(char c){
    switch (c){
        case '+':
            return 1;
        case '-':
            return 2;
        case '*':
            return 3;
        case '/':
            return 4;
        case '(':
        case ')':
            return 0;
        default:
            if (isdigit(c)) return -1;
            else if (c >= 'A' && c <= 'Z') return -1;
            else if (c >= 'a' && c <= 'z') return -1;
            else return 404;
    }
}


void checkSymbol(Node* &ptr, char c, Stack &output, Stack &symbols, string &curr, bool PN){
    int priority;
    if (PN) priority = priorPN(c);
    else priority = priorRPN(c);
    if(ptr == nullptr || ptr->prior < priority){
        if (!curr.empty()){
            if (PN) reverse(curr.begin(), curr.end());
            cout << "\tОперанд был полностью считан. Помещаем его в строку.";
            output.pushFront(curr, -1);
            curr = "";
        }
        curr.push_back(c);
        cout << "\tПриоритет операции выше или стек пуст. Помещаем операцию в стек.";
        symbols.pushFront(curr, priority);
        curr = "";
    } else {
        if (!curr.empty()){
            if (PN) reverse(curr.begin(), curr.end());
            cout << "\tОперанд был полностью считан. Помещаем его в строку.";
            output.pushFront(curr, -1);
            curr = "";
        }
        if (PN && priority == 2 && ptr->prior == 2){
            curr.push_back(c);
            cout << "\tПриоритет операции выше или стек пуст. Помещаем операцию в стек.";
            symbols.pushFront(curr, priority);
            curr = "";
            return;
        }
        while (ptr != nullptr && ptr->prior >= priority){
            cout << "\tПриоритет операции ниже. Достаем операции из стека.";
            output.pushFront(ptr->data, priorRPN(ptr->data[0]));
            symbols.popFront();
            ptr = symbols.head;
        }
        checkSymbol(ptr, c, output, symbols, curr, PN);
    }
}


bool printOutputRPN(Stack &output, string &inputString){
    string str;
    for (Node *ptr = output.head; ptr != nullptr; ptr = ptr->next) {
        if(ptr->data == "("){
            cout << "\nОшибка ввода строки! Отсутстует закрывающая скобка.\n";
            return false;
        }
        str = ptr->data + " " + str;
    }
    cout << "\nВыходная строка: " << str << "\n";
    inputString = str;
    return true;
}


bool printOutputPN(Stack &output){
    string str;
    for (Node *ptr = output.head; ptr != nullptr; ptr = ptr->next) {
        if(ptr->data == ")"){
            cout << "\nОшибка ввода строки! Отсутстует открвающая скобка.\n";
            return false;
        }
        str += " " + ptr->data;
    }
    cout << "\nВыходная строка: " << str << "\n";
    return true;
}

//..........................................
bool convertToRPN(string &inputString, Stack &output, Stack &symbols) {
    string curr = "";
    Node *ptr = nullptr;
    for (int i = 0; i < inputString.length(); i++) {
        cout << "\nСчитали символ: " << inputString[i];
        sleepScreen();
        switch (priorRPN(inputString[i])) {
            case -1:
                cout << "\tЭто число или переменная. Помещаем в промежуточную строку.";
                sleepScreen();
                curr.push_back(inputString[i]);
                break;
            case 1:
            case 2:
                cout << "\tЭто операция.";
                sleepScreen();
                ptr = symbols.head;
                checkSymbol(ptr, inputString[i], output, symbols, curr, false);
                break;
            case 0:
                ptr = symbols.head;
                if (inputString[i] == '('){
                    cout << "\tЭто открывающая скобка. Помещаем её в стек.";
                    sleepScreen();
                    curr = '(';
                    symbols.pushFront(curr, 0);
                    curr = "";
                } else {
                    cout << "\tЭто закрывающая скобка. Переносим промежуточную строку, а затем операции в выходную строку.";
                    sleepScreen();
                    output.pushFront(curr, -1);
                    curr = "";
                    while (ptr != nullptr && ptr->data[0] != '('){
                        output.pushFront(ptr->data, priorRPN(ptr->data[0]));
                        symbols.popFront();
                        ptr = symbols.head;
                        if (ptr == nullptr){
                            cout << "\nОшибка ввода строки! Открывающая скобка не найдена.\n";
                            return false;
                        }
                    }
                    symbols.popFront();
                }
                break;
            case 404:
                cout << "\nОшибка ввода! Неправильно введён символ.\n";
                return false;
        }
    }
    if(!curr.empty()){
        cout << "\nПереносим оставшиеся операнды в строку.";
        sleepScreen();
        output.pushFront(curr, -1);
    }
    if (symbols.head != nullptr){
        cout << "\nПереносим оставшиеся операции в строку.";
        sleepScreen();
        for (Node* p = symbols.head; p != nullptr; p = p->next){
            output.pushFront(p->data, -1);
        }
    }
    cout << "\nСтек и строка пустые. Все символы считаны.";
    if (!printOutputRPN(output, inputString)) return false;
    return true;
}


bool convertToPN(string &inputString, Stack &output, Stack &symbols){
    string curr = "";
    Node *ptr = nullptr;
    for (int i = inputString.length()-1; i >= 0; i--) {
        cout << "\nСчитали символ: " << inputString[i];
        sleepScreen();
        switch (priorPN(inputString[i])) {
            case -1:
                cout << "\tЭто число или переменная. Помещаем в промежуточную строку.";
                sleepScreen();
                curr.push_back(inputString[i]);
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                cout << "\tЭто операция.";
                sleepScreen();
                ptr = symbols.head;
                checkSymbol(ptr, inputString[i], output, symbols, curr, 1);
                break;
            case 0:
                ptr = symbols.head;
                if (inputString[i] == ')'){
                    cout << "\tЭто закрывающая скобка. Помещаем её в стек.";
                    sleepScreen();
                    curr = ')';
                    symbols.pushFront(curr, 0);
                    curr = "";
                } else {
                    cout << "\tЭто открывающая скобка. Переносим промежуточную строку, а затем операции в выходную строку.";
                    sleepScreen();
                    if (!curr.empty()){
                        reverse(curr.begin(), curr.end());
                        output.pushFront(curr, -1);
                        curr = "";
                    }
                    while (ptr != nullptr && ptr->data[0] != ')'){
                        output.pushFront(ptr->data, priorPN(ptr->data[0]));
                        symbols.popFront();
                        ptr = symbols.head;
                        if (ptr == nullptr){
                            cout << "\nОшибка ввода строки! Закрывающая скобка не найдена.\n";
                            return false;
                        }
                    }
                    symbols.popFront();
                }
                break;
            case 404:
                cout << "\nОшибка ввода! Неправильно введён символ.\n";
                return false;
        }
    }
    if(!curr.empty()){
        cout << "\nПереносим оставшиеся операнды в строку.";
        sleepScreen();
        reverse(curr.begin(), curr.end());
        output.pushFront(curr, -1);
    }
    if (symbols.head != nullptr){
        cout << "\nПереносим оставшиеся операции в строку.";
        sleepScreen();
        for (Node* p = symbols.head; p != nullptr; p = p->next){
            output.pushFront(p->data, -1);
        }
    }
    cout << "\nСтек и строка пустые. Все символы считаны.";
    if (!printOutputPN(output)) return false;
    return true;
}


void clearStacks(Stack &symbols, Stack &output){
    output.clearStack();
    symbols.clearStack();
}

//.....................................................................
float operation(float a, float b, char c){
    switch (c){
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            return 0;
    }
}


bool resultRPN(string &inputString, Stack &output){
    string curr;
    float result;
    int variable;
    for (int i = 0; i < inputString.length(); i++){
        if (inputString[i] != ' '){
            cout << "\nСчитали символ: " << inputString[i];
            sleepScreen();
            switch(priorRPN(inputString[i])){
                case -1:
                    if (!isdigit(inputString[i])){
                        cout << "\tЭто переменная. Инициализируйте её: " << inputString[i] << " ==";
                        cin >> variable;
                        sleepScreen();
                        curr += to_string(variable);
                    }
                    else{
                        cout << "\tЭто цифра. Переносим в промежуточную строку.";
                        sleepScreen();
                        curr.push_back(inputString[i]);
                    }
                    break;
                case 1:
                case 2:
                    if (output.head == output.tail){
                        cout << "\nОшибка ввода строки! Недостаточно операндов.\n";
                        return false;
                    }
                    cout << "\tЭто оператор. Достаем операнд из стека: " << output.head->data;
                    sleepScreen();
                    result = stof(output.head->data);
                    output.popFront();
                    if (inputString[i] == '/' && result == 0){
                        cout << "\nДеление на ноль невозможно!\n";
                        return false;
                    }
                    cout << " Достаем операнд из стека: " << output.head->data;
                    sleepScreen();
                    result = operation(stof(output.head->data), result, inputString[i]);
                    output.popFront();
                    curr = to_string(result);
                    cout << "\tРезультат: " << curr << " Помещаем его в стек.";
                    sleepScreen();
                    output.pushFront(curr, -1);
                    curr = "";
                    break;
                case 404:
                    cout << "\nОшибка ввода строки! Неправильно введен символ.\n";
                    return false;
            }
        } else{
            if (!curr.empty()){
                cout << "\tПереносим операнд в стек.";
                sleepScreen();
                output.pushFront(curr, -1);
                curr = "";
            }
        }
    }
    if (output.head != output.tail){
        cout << "\nОшибка ввода! Недостаточно операций.\n";
        return false;
    }
    return true;
}


bool resultPN(string &inputString, Stack &output) {
    string curr = "";
    float result;
    int variable;
    for (int i = inputString.length() - 1; i >= 0; i--) {
        if (inputString[i] != ' ') {
            cout << "\nСчитали символ: " << inputString[i];
            sleepScreen();
            switch (priorPN(inputString[i])) {
                case -1:
                    if (!isdigit(inputString[i])){
                        cout << "\tЭто переменная. Инициализируйте её: " << inputString[i] << " ==";
                        cin >> variable;
                        sleepScreen();
                        curr += to_string(variable);
                    }
                    else {
                        cout << "\tЭто цифра. Переносим в промежуточную строку.";
                        sleepScreen();
                        curr.push_back(inputString[i]);
                    }
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                    if (output.head == output.tail) {
                        cout << "\nОшибка ввода строки! Недостаточно операндов.\n";
                        return false;
                    }
                    cout << "\tЭто оператор. Достаем операнд из стека: " << output.head->data;
                    sleepScreen();
                    result = stof(output.head->data);
                    output.popFront();
                    if (inputString[i] == '/' && stof(output.head->data) == 0){
                        cout << "\nДеление на ноль невозможно!\n";
                        return false;
                    }
                    cout << " Достаем операнд из стека: " << output.head->data;
                    sleepScreen();
                    result = operation(result, stof(output.head->data), inputString[i]);
                    output.popFront();
                    curr = to_string(result);
                    cout << "\tРезультат: " << curr << " Помещаем его в стек.";
                    sleepScreen();
                    output.pushFront(curr, -1);
                    curr = "";
                    break;
                case 404:
                    cout << "\nОшибка ввода строки! Неправильно введен символ.\n";
                    return false;
            }
        } else {
            if (!curr.empty()) {
                reverse(curr.begin(), curr.end());
                output.pushFront(curr, -1);
                curr = "";
            }
        }
    }
    return true;
}
//.....................................................................
int main() {
    SetConsoleOutputCP(CP_UTF8);
    Stack symbols, output;
    string inputString;
    short int choise;
    symbols.createStack();
    output.createStack();
    while (true){
        mainMenu();
        cin >> choise;
        clearStream();
        switch (choise){
            case 1:
                clearStacks(symbols, output);
                recordingMenu();
                cin >> choise;
                fillString(inputString);
                if (choise == 1){
                    convertToRPN(inputString, output, symbols);
                }
                else if (choise == 2){
                    convertToPN(inputString, output, symbols);
                }
                else cout << "Неправильно введён номер!\n";
                break;
            case 2:
                clearStacks(symbols, output);
                recordingMenu();
                cin >> choise;
                fillString(inputString);
                switch (choise){
                    case 1:
                        if (resultRPN(inputString, output)){
                            cout << "\nВыражение корректно.\n";
                        }
                        else cout << "\nВыражение не корректно.\n";
                        break;
                    case 2:
                        if (resultPN(inputString, output)){
                            cout << "\nВыражение корректно.\n";
                        }
                        else cout << "\nВыражение не корректно.\n";
                        break;
                    case 3:
                        if (!convertToRPN(inputString, output, symbols)){
                            cout << "\nВыражение не корректно.\n";
                            break;
                        }
                        clearStacks(symbols, output);
                        if (resultRPN(inputString, output)){
                            cout << "\nВыражение корректно.\n";
                        }
                        else cout << "\nВыражение не корректно.\n";
                        break;
                    default:
                        cout << "Неправильно введён номер!\n";
                }
                break;
            case 3:
                clearStacks(symbols, output);
                recordingMenu();
                cin >> choise;
                fillString(inputString);
                if (choise == 1){
                    if (resultRPN(inputString, output)){
                        cout << "\nРезультат вычислений: " << output.head->data;
                    }
                }
                else if (choise == 2){
                    if (resultPN(inputString, output)){
                        cout << "\nРезультат вычислений: " << output.head->data;
                    }
                }
                else if (choise == 3){
                    clearStacks(symbols, output);
                    convertToRPN(inputString, output, symbols);
                    clearStacks(symbols, output);
                    if (resultRPN(inputString, output)){
                        cout << "\nРезультат вычислений: " << output.head->data;
                    }
                }
                else cout << "Неправильно введён номер!\n";
                break;
            case 4:
                system("cls");
                break;
            case 5:
                clearStacks(symbols, output);
                exit(0);
            default:
                cout << "Неправильно введён номер!\n";
        }
        cout << "\n";
    }
}
