#include "InteractionWTK.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
using namespace std;
namespace keyboard {
	void gotoxy(int x, int y)
	{
		// �.� ��� ���������� ������� ������ ������ "������������" � ������������ � ������ � ���������� ������� ����. ��� ����� ����� ��� �������
		COORD coord;
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	char diskSearch() {
		DWORD drives = GetLogicalDrives();
		int selectedDrive = 0;
		int count = 0;
		char selectedDriveLetter = 'A'; // �� ��������� �������� ���� A

		// ������������ ���������� ��������� ������
		for (int i = 0; i < 26; i++) {
			if (drives & (1 << i)) {
				count++;
			}
		}
		// ������� ������ ������ � �������� ������ ��������� ����
		for (int i = 0; i < 26; i++) {
			if (drives & (1 << i)) {
				char driveLetter = 'A' + i;
				cout << (selectedDrive == i ? "> " : " ") << driveLetter << ":\\\\" << endl;
				if (selectedDrive == 0) {
					selectedDrive = i;
				}

			}
		}

		// ������������ ������� ������
		while (true) {
			if (_kbhit()) { // ��������, ���� �� ������ �������
				char key = _getch();// ��������� ������� �������
				if (key == 72 && selectedDrive > 0) {// ������� "�����"
					// ����� ������������ �������� ������� ����� "selectedDrive--;" �����������, ��� �������� ���������� ����
					if (selectedDrive == count - 1) {// ���������, �������� �� ������� ���� ������ ��������� ������
						// ���� ��, �� �� �������� selectedDrive
						continue;
					}
					selectedDrive--;
				}
				else if (key == 80 && selectedDrive < count + 1) {// ������� "����"
					// �������� ��������� ����
					selectedDrive++;
				}
				else if (key == 13) { //������� "Enter" ��� ������ �����
					selectedDriveLetter = 'A' + selectedDrive;
					break;
				}
				else if (key == 27) { // ������� "Esc"
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
