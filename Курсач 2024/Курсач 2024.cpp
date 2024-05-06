#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <conio.h>
using namespace std;

namespace fs = std::filesystem;

// Функция для установки позиции курсора консоли
void gotoxy(int x, int y) {
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
	char selectedDriveLetter = 'A'; // По умолчанию выбираем диск C

	// Подсчитываем количество доступных дисков
	for (int i = 0; i < 26; i++){
		if (drives & (1 << i)) {
			count++;
		}
	}
	// Выводим список дисков и выбираем первый доступный диск
	for (int i = 0; i < 26; i++){
		if (drives & (1 << i)) {
			char driveLetter = 'A' + i;
			cout << (selectedDrive == i ? "> " : " ") << driveLetter << ":\\\\" << endl;
			if (selectedDrive == 0){
				selectedDrive = i;
			}
		}
	}

	// Обрабатываем нажатия клавиш
	while (true){
		if (_kbhit()) { // Проверка, была ли нажата клавиша
			char key = _getch();// Считываем нажатую клаившу
			if (key == 72 && selectedDrive > 0){// Клавиша "Вверх"
				// Когда пользователь нажимает клавишу вверх "selectedDrive--;" уменьшается, что выбирает предыдущий диск
				selectedDrive--; 
			}
			else if (key == 80 && selectedDrive < count + 1){// Клавиша "Вниз"
				// Выбирает следующий диск
				selectedDrive++;
			}
			else if (key == 13){ //Клавиша "Enter" для выбора диска
				selectedDriveLetter = 'A' + selectedDrive;
				break;
			}
			else if (key == 27){ // Клавиша "Esc"
				break;
			}

			system("cls");
			gotoxy(0,0);
			for (int i = 0; i < 26; i++){
				if (drives & (1 << i)) {
					char driveLetter = 'A' + i;
					cout << (selectedDrive == i ? "> " : " ") << driveLetter << ":\\\\" << endl;
				}
			}
		}
	}
	return selectedDriveLetter;
}
bool fileMatching() {
	// Дописать функцию
	// Функция поиска полного совпадения файла, ответ в файле Stream Api
}
void find_files(const fs::path& directory, const std::string& filename) {
	for (const auto& entry :fs::directory_iterator(directory)){
		if (entry.is_regular_file()) {
			// Получаем имя файла без расширения
			string current_filename = entry.path().stem().string();

			string lower_filename = filename;
			transform(lower_filename.begin(), lower_filename.end(), lower_filename.begin(), ::tolower);
			// Преобразуем текущее имя файла в нижний регистр
			transform(current_filename.begin(), current_filename.end(), current_filename.begin(), ::tolower);
			// Проверяем, содержит ли имя файла искомое имя
			if (current_filename.find(lower_filename) != string::npos){
				cout << entry.path() << endl;
			}
		}
	}
}


int main()
{
	setlocale(LC_ALL, "ru");

	char selectedDrive = diskSearch();
	if (selectedDrive != '\0'){
		string directory;
		cout << "Введите путь к директории для поиска (например, " << selectedDrive << ":\\example\\: ";
		getline(cin, directory);

		string filename;
		cout << "Введите название файла для поиска: ";
		getline(cin, filename);

		find_files(directory, filename);
	}
	else{
		cout << "Поиск файлов отменен" << endl;
	}

	return 0;
}

