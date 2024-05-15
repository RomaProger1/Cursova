#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <conio.h>
#include <unordered_set>
#include "InteractionWTK.h"
using namespace std;

namespace fs = std::filesystem;

void find_files(const fs::path& directory, const std::string& filename) {
	for (const auto& entry :fs::directory_iterator(directory)){
		if (entry.is_regular_file()) {
			// Получаем имя файла без расширения

			string current_filename = entry.path().stem().string();

			// Преобразуем текущее имя файла в нижний регистр
			string lower_filename = filename;
			transform(lower_filename.begin(), lower_filename.end(), lower_filename.begin(), ::tolower);
			transform(current_filename.begin(), current_filename.end(), current_filename.begin(), ::tolower);
			auto it = search(current_filename.begin(), current_filename.end(), lower_filename.begin(), lower_filename.end());
			// Проверяем, содержит ли имя файла искомое имя
			if (current_filename == lower_filename){
				cout << entry.path() << "Искомый файл" << endl;
			}
			// Проверяем, частично ли совпадает имя файла с искомым именем
			else if (it != current_filename.end()){
				cout << entry.path() << "Название файла частично совпадает" << endl;
			}
			else
			{
				cout << entry.path() << "Нет искомого названия файлов" << endl;
			}
		}
		
	}
	
}

void find_directories(const fs::path& directory, const std::string& dirname) {
	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.is_directory()) {

			string current_dirname = entry.path().filename().string();
			// Преобразуем текущее имя директории в нижний регистр
			string lower_ariname = dirname;
			transform(lower_ariname.begin(), lower_ariname.end(), lower_ariname.begin(), ::tolower);
			transform(current_dirname.begin(), current_dirname.end(), current_dirname.begin(), ::tolower);

			auto it = search(current_dirname.begin(), current_dirname.end(), lower_ariname.begin(), lower_ariname.end());
			if (current_dirname == lower_ariname){
				cout << entry.path() << " - Искомая папка" << endl;
				if (current_dirname != lower_ariname){
					
						
				}
			}
			
		}
	}
}
void logic() {

}

int main()
{
	setlocale(LC_ALL, "ru");

	char selectedDrive = keyboard::diskSearch();
	if (selectedDrive != '\0'){
		string directory;
		cout << "Введите путь к директории для поиска (например, " << selectedDrive << ":\\example\\:";
		getline(cin, directory );
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

