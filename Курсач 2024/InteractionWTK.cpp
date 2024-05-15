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

}
