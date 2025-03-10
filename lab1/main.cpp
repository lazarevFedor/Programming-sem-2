#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int choise;
const int SIZE = 30;

struct Student {
	string name;
	char sex;
	int group;
	int id;
	int grades[8];
	float averageScore;
};

Student dataBase[SIZE];

void clearStream() {
	cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void menu() {
	cout << "1) Создать новую запись\n" <<
		"2) Внести изменения в запись\n" <<
		"3) Вывод всех данных\n" <<
		"4) Вывод информации обо всех студентах группы N\n" <<
		"5) Студенты с наивысшим баллом\n" <<
		"6) Количество студентов мужского и женского пола\n" <<
		"7) Вывод данных успеваемости студентов\n" <<
		"8) Вывод данных о студентах, имеющих номер в списке – k\n" <<
		"9) Выход\n" << "-->> ";
}

float countScore(int grades[8]) {
	float score = 0;
	for (int i = 0; i < 8; i++) {
		score += grades[i];
	}
	score /= 8;
	return score;
}

Student createStudent() {
	cout << "Введите имя: ";
	clearStream();
	Student student;
	getline(cin, student.name);
	cout << "Введите пол: ";
	cin >> student.sex;
	cout << "Введите группу: ";
	cin >> student.group;
	cout << "Введите номер в группе: ";
	cin >> student.id;
	cout << "Введите оценки через пробел: ";
	for (int i = 0; i < 8; i++) cin >> student.grades[i];
	countScore(student.grades);
	return student;
}

void showAllStudents(Student dataBase[], int& count) {
	for (int j = 0; j < count; j++) {
	cout << "ФИО: " << dataBase[j].name;
	cout << "\tПол: " << dataBase[j].sex;
	cout << "\tГруппа: " << dataBase[j].group;
	cout << "\tНомер: " << dataBase[j].id << "\tОценки: ";
	for (int i = 0; i < 8; i++) {
		cout << dataBase[j].grades[i] << " ";
	}
	cout << "\tСредний балл: " << dataBase[j].averageScore;
	cout << "\n\n";
}
}

void showStudent(int n, Student base[SIZE]) {
	cout << "ФИО: " << base[n].name;
	cout << "\tПол: " << base[n].sex;
	cout << "\tГруппа: " << base[n].group;
	cout << "\tНомер: " << base[n].id << "\tОценки: ";
	for (int i = 0; i < 8; i++) {
		cout << base[n].grades[i] << " ";
	}
	cout << "\tСредний балл: " << base[n].averageScore;
	cout << "\n\n";
}

void topStudents(Student top[SIZE], int& count) {
	int j;
	Student buff;
	for (int i = 1; i < count; i++) {
		buff = top[i];
		j = i - 1;
		while (j >= 0 && top[j].averageScore > buff.averageScore) {
			top[j + 1] = top[j];
			j = j - 1;
		}
		top[j + 1] = buff;
	}
	for (int j = count - 1; j >= count-4; j--) {
		showStudent(j, top);
	}
}

Student changeId(Student student, int numGroup, int& numId, int& count) {
	if (numId == 1) {
		for (int i = 0; i < count; i++) {
			if (dataBase[i].group == numGroup && dataBase[i].id >= numId) {
				++dataBase[i].id;
			}
		}
		student.id = numId;
	}
	else {
		for (int i = 0; i < count; i++) {
			if (dataBase[i].group == numGroup && dataBase[i].id >= numId && dataBase[i].id < student.id) ++dataBase[i].id;
		}
		student.id = numId;
	}
	return student;
}

void changeStudent(int& numGroup, int& numId, int& count) {
	Student student;
	int check = -1;
	for (int i = 0; i < count; i++) {
		if (dataBase[i].group == numGroup && dataBase[i].id == numId) {
			student = dataBase[i];
			check = i;
			break;
		}
	}
	if (check == -1) {
		cout << "\nТакого студента нет.\n\n";
	}
	else {
		cout << "\nВыберите данные:\n" <<
			"1) ФИО\n" <<
			"2) Пол\n" <<
			"3) Группа\n" <<
			"4) Номер\n" <<
			"5) Оценки\n" << "-->> ";
		cin >> choise;
		switch (choise) {
		case 1:
			cout << "\nВведите данные -->> ";
			student.name = "";
			clearStream();
			getline(cin, student.name);
			break;
		case 2:
			cout << "\nВведите данные -->> ";
			cin >> student.sex;
			break;
		case 3:
			cout << "\nВведите данные -->> ";
			numGroup = student.group;
			cin >> student.group;
			//Цикл для сдвига номеров в группе из которой убрали человека
			for (int i = 0; i < count; i++) {
				if (dataBase[i].group == numGroup && dataBase[i].id > numId) {
					--dataBase[i].id;
				}
			}
			//Вставка номера студента в новую группу
			student = changeId(student, student.group, numId, count);
			break;
		case 4:
			cout << "\nВведите данные -->> ";
			cin >> numId;
			student = changeId(student, numGroup, numId, count);
			break;
		case 5:
			cout << "\nВведите данные -->> ";
			for (int i = 0; i < 8; i++) {
				cin >> student.grades[i];
			}
			break;
		}
		dataBase[check] = student;
	}
}

void gradesSort(int& count) {
	int gradesCount = 0;
	cout << "Троечники:\n";
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < 8; j++) {
			if (dataBase[i].grades[j] == 3) {
				showStudent(i, dataBase);
				break;
			}
		}
	}
	cout << "Хорошисты:\n";
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < 8; j++) {
			if (dataBase[i].grades[j] == 3) {
				gradesCount = -1;
				break;
			}
			else if (dataBase[i].grades[j] == 5) {
				++gradesCount;
			}
		}
		if (gradesCount != 8 && gradesCount != -1) {
			showStudent(i, dataBase);
		}
	}
	cout << "Отличники:\n";
	gradesCount = 0;
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < 8; j++) {
			if (dataBase[i].grades[j] == 3 || dataBase[i].grades[j] == 4) {
				gradesCount = -1;
				break;
			}
		}
		if (gradesCount != -1) {
			showStudent(i, dataBase);
		}
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	ofstream fout;
	ifstream fin;
	int count = 0, numGroup, numId, countM = 0, countW = 0, countG = 0;

	fin.open("LETI.txt");
	if (!fin.is_open()) {
		cout << "Ошибка открытия файла!!!";
		exit(404);
	}
	else {
		while (!fin.eof()) {
			getline(fin, dataBase[count].name);
			fin >> dataBase[count].sex;
			fin >> dataBase[count].group;
			fin >> dataBase[count].id;
			for (int i = 0; i < 8; i++) {
				fin >> dataBase[count].grades[i];
			}
			count++;
			fin.get();
		}
		for (int i = 0; i < count; i++) {
			dataBase[i].averageScore = countScore(dataBase[i].grades);
		}
		cout << "Данные из файла считаны.\n\n";
	}
	fin.close();
	while (true) {
		menu();
		cin >> choise;
		switch (choise) {
		case 1:
			fout.open("LETI.txt", ofstream::app);
			if (!fout.is_open()) {
				cout << "Ошибка открытия файла!!!";
				exit(0);
			}
			else if ((count + 1) > SIZE) {
				cout << "Закончилось место для записи!\n";
			}
			else {
				dataBase[count] = createStudent();
				fout << '\n' << dataBase[count].name << '\n';
				fout << dataBase[count].sex << '\n';
				fout << dataBase[count].group << '\n';
				fout << dataBase[count].id << '\n';
				for (int i = 0; i < 7; i++) {
					fout << dataBase[count].grades[i] << " ";
				}
				fout << dataBase[count].grades[7];
				count++;
			}
			fout.close();
			cout << "\nЗапись создана!\n";
			break;
		case 2:
			cout << "\nВведите номер группы -->> ";
			cin >> numGroup;
			cout << "Введите номер студента в группе -->> ";
			cin >> numId;
			changeStudent(numGroup, numId, count);
			fout.open("LETI.txt");
			if (!fout.is_open()) {
				cout << "Ошибка открытия файла!!!";
				exit(0);
			}
			else {
				for (int i = 0; i < count; i++) {
					fout << dataBase[i].name << '\n';
					fout << dataBase[i].sex << '\n';
					fout << dataBase[i].group << '\n';
					fout << dataBase[i].id << '\n';
					for (int j = 0; j < 7; j++) {
						fout << dataBase[i].grades[j] << " ";
					}
					fout << dataBase[i].grades[7];
					if (i != count - 1) fout << '\n';
				}
			}
			fout.close();
			break;
		case 3:
			system("cls");
			showAllStudents(dataBase, count);
			break;
		case 4:
			cout << "Введите номер группы -->> ";
			cin >> numGroup;
			for (int j = 0; j < count; j++) {
				if (dataBase[j].group == numGroup) {
					cout << dataBase[j].name << "\t";
					cout << dataBase[j].sex << "\t";
					cout << dataBase[j].group << "\t";
					cout << dataBase[j].id << "\t";
					for (int i = 0; i < 8; i++) {
						cout << dataBase[j].grades[i] << " ";
					}
					cout << "\n\n";
					++countG;
				}
			}
			if (countG == 0) {
				cout << "Ни одного студента не найдено.\n\n";
			}
			break;
		case 5:
			topStudents(dataBase, count);
			break;
		case 6:
			for (int i = 0; i < count; i++) {
				if (dataBase[i].sex == 'M') {
					countM++;
				}
				else {
					countW++;
				}
			}
			cout << "\nКоличество пацанов: " << countM;
			cout << "\tКоличество девчат: " << countW << "\n\n";
			break;
		case 7:
			system("cls");
			gradesSort(count);
			break;
		case 8:
			clearStream();
			cout << "\nВведите номер в списке -->> ";
			cin >> numId;
			for (int j = 0; j < count; j++) {
				if (dataBase[j].id == numId) {
					showStudent(numId, dataBase);
				}
			}
			break;
		case 9:
			exit(0);
			break;
		default:
			cout << "\nНеправильно введен номер!\n";
		}
	}
	return 0;
}
