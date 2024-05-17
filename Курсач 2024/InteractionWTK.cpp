#include "InteractionWTK.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;

namespace keyboard {

	void gotoxy(int x, int y)
	{
		// Т.к при обновлении консоли курсор выбора "сбрасывается" и возвращается в начало и невозможно выбрать диск. Для этого нужна эта функция
		COORD coord;
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	char diskSearch() {
		DWORD drives = GetLogicalDrives();
		int selectedDrive = 0;
		int count = 0;
		char selectedDriveLetter = 'A'; // По умолчанию выбираем диск A

		// Подсчитываем количество доступных дисков
		for (int i = 0; i < 26; i++) {
			if (drives & (1 << i)) {
				count++;
			}
		}
		// Выводим список дисков и выбираем первый доступный диск
		for (int i = 0; i < 26; i++) {
			if (drives & (1 << i)) {
				char driveLetter = 'A' + i;
				cout << (selectedDrive == i ? "> " : " ") << driveLetter << ":\\\\" << endl;
				if (selectedDrive == 0) {
					selectedDrive = i;
				}

			}
		}

		// Обрабатываем нажатия клавиш
		while (true) {
			if (_kbhit()) { // Проверка, была ли нажата клавиша
				char key = _getch();// Считываем нажатую клаившу
				if (key == 72 && selectedDrive > 0) {// Клавиша "Вверх"
					// Когда пользователь нажимает клавишу вверх "selectedDrive--;" уменьшается, что выбирает предыдущий диск
					if (selectedDrive == count - 1) {// Проверяем, является ли текущий диск первым доступным диском
						// Если да, то не изменяем selectedDrive
						continue;
					}
					selectedDrive--;
				}
				else if (key == 80 && selectedDrive < count + 1) {// Клавиша "Вниз"
					// Выбирает следующий диск
					selectedDrive++;
				}
				else if (key == 13) { //Клавиша "Enter" для выбора диска
					selectedDriveLetter = 'A' + selectedDrive;
					break;
				}
				else if (key == 27) { // Клавиша "Esc"
					break;
				}

				system("cls");
				gotoxy(0, 0);
				for (int i = 0; i < 26; i++) {
					if (drives & (1 << i)) {
						char driveLetter = 'A' + i;
						cout << (selectedDrive == i ? "> " : " ") << driveLetter << ":\\\\" << endl;
					}
				}
			}
		}
		return selectedDriveLetter;
	}
	char keyYesOrNo() { // Выбор 'Да' 'нет'

		const char options[] = { 'y', 'n' };
		int selectedOption = 0;
		int optionCount = sizeof(options) / sizeof(options[0]);

		cout << (selectedOption == 0 ? "> Да" : " Да") << endl;
		cout << (selectedOption == 1 ? "> Нет" : " Нет") << endl;

		while (true) {
			if (_kbhit) {
				char key = _getch();

				if (key == 72 && selectedOption > 0) { // Клавиша "Вверх"
					selectedOption--;
				}
				else if (key == 80 && selectedOption < optionCount - 1) { // Клавиша "Вниз"
					selectedOption++;
				}
				else if (key == 13) { // Клавиша "Enter" для выбора варианта
					return options[selectedOption];
				}
				else if (key == 27) { // Клавиша "Esc"
					return '\0'; // Возвращаем '\0' в качестве символа, если пользователь отменил выбор
				}
				system("cls");
				gotoxy(0, 0);
				cout << (selectedOption == 0 ? "> Да" : " Да") << endl;
				cout << (selectedOption == 1 ? "> Нет" : " Нет") << endl;
			}
		}
	}
	char keyYesOrNoMod() {

		const char options[] = { 'y', 'n' };
		int selectedOption = 0;
		int optionCount = sizeof(options) / sizeof(options[0]);

		cout << (selectedOption == 0 ? "> Да завершить" : " Да завершить") << endl;
		cout << (selectedOption == 1 ? "> Нет, начать новый поиск" : " Нет, начать новый поиск") << endl;

		while (true) {
			if (_kbhit) {
				char key = _getch();

				if (key == 72 && selectedOption > 0) { // Клавиша "Вверх"
					selectedOption--;
				}
				else if (key == 80 && selectedOption < optionCount - 1) { // Клавиша "Вниз"
					selectedOption++;
				}
				else if (key == 13) { // Клавиша "Enter" для выбора варианта
					return options[selectedOption];
				}
				else if (key == 27) { // Клавиша "Esc"
					return '\0'; // Возвращаем '\0' в качестве символа, если пользователь отменил выбор
				}
				system("cls");
				gotoxy(0, 0);
				cout << (selectedOption == 0 ? "> Да завершить" : " Да завершить") << endl;
				cout << (selectedOption == 1 ? "> Нет, начать новый поиск" : " Нет, начать новый поиск") << endl;
			}
		}
	}
	char keyChoice() { // Выбор поиска

		const char options[] = { 'a', 'b','c'};
		int selectedOption = 0;
		int optionCount = sizeof(options) / sizeof(options[0]);

		cout << (selectedOption == 0 ? "> Поиск файла по расширению" : " Поиск файла по расширению") << endl;
		cout << (selectedOption == 1 ? "> Поиск фала по названию" : " Поиск фала по названию") << endl;
		cout << (selectedOption == 2 ? "> Поиск директории (папки)" : " Поиска директории (папки)") << endl;

		while (true) {
			if (_kbhit) {
				char key = _getch();

				if (key == 72 && selectedOption > 0) { // Клавиша "Вверх"
					selectedOption--;
				}
				else if (key == 80 && selectedOption < optionCount - 1) { // Клавиша "Вниз"
					selectedOption++;
				}
				else if (key == 13) { 
					return options[selectedOption];
				}
				else if (key == 27) { 
					return '\0'; 
				}
				system("cls");
				gotoxy(0, 0);
				cout << "Что вы хотите найти ?" << endl;
				cout << (selectedOption == 0 ? "> Поиск файла по расширению" : " Поиск файла по расширению") << endl;
				cout << (selectedOption == 1 ? "> Поиск фала по названию" : " Поиск фала по названию") << endl;
				cout << (selectedOption == 2 ? "> Поиск директории (папки)" : " Поиска директории (папки)") << endl;
			}
		}
	}
	char keyYesOrNoOrAll() {
		const char options[] = { 'y', 'n','a'};
		int selectedOption = 0;
		int optionCount = sizeof(options) / sizeof(options[0]);

		cout << (selectedOption == 0 ? "> Да" : " Да") << endl;
		cout << (selectedOption == 1 ? "> Нет" : " Нет") << endl;
		cout << (selectedOption == 2 ? "> Вывести всё" : " Вывести всё") << endl;

		while (true) {
			if (_kbhit) {
				char key = _getch();

				if (key == 72 && selectedOption > 0) { // Клавиша "Вверх"
					selectedOption--;
				}
				else if (key == 80 && selectedOption < optionCount - 1) { // Клавиша "Вниз"
					selectedOption++;
				}
				else if (key == 13) { // Клавиша "Enter" для выбора варианта
					return options[selectedOption];
				}
				else if (key == 27) { // Клавиша "Esc"
					return '\0'; // Возвращаем '\0' в качестве символа, если пользователь отменил выбор
				}
				system("cls");
				gotoxy(0, 0);
				cout << (selectedOption == 0 ? "> Да" : " Да") << endl;
				cout << (selectedOption == 1 ? "> Нет" : " Нет") << endl;
				cout << (selectedOption == 2 ? "> Вывести всё" : " Вывести всё") << endl;
			}
		}
	}
}
