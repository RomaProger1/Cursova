#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <conio.h>
#include <unordered_set>
#include "InteractionWTK.h"
#include "RulesProgramm.h"
using namespace std;



namespace fs = std::filesystem;
void find_extension(const fs::path& directory, const string& extensionName) {
	// Проверка, существует ли директория и является ли она директорией 
	if (!fs::exists(directory) || !fs::is_directory(directory)) {
		cerr << "Директория не существует или не является директорией: " << directory << endl;
		return;
	}

	for (const auto& entry : fs::directory_iterator(directory)){
		// Проверка, является ли текущий путь к фалам и соответствует ли его расширение заданному
		if (fs::is_regular_file(entry.status()) && entry.path().extension() == extensionName){
			cout << entry.path() << endl;
		}
	}

}

void find_files(const fs::path& directory, const string& filename) {
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
				// Проверяем, частично ли совпадает имя файла с искомым именем

			}
			else if (it != current_filename.end()){
				cout << entry.path() << "Частично совпадающий файл" << endl;
			}

		
		}
		
	}
	
}
void find_directories(const fs::path& directory, const string& dirname) {
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

				if (it != current_dirname.end()) {

					cout << "Вывести частично совпадающие файлы ?" << endl;
					char choice = keyboard::keyYesOrNo();
					if (choice == 'y'){
						cout << entry.path() << "Частично совпадающие файлы" << endl;
					}
					else if (choice == 'n'){
						int x = 0;
						for (int i = 0; i < entry.file_size(); i++){
							i = x;
						}
						cout << "Всего проверено файлов" << x << endl;
					}
					else if (choice)
					{

					}
				}
			}
			
		}

	}

}
void logic() {

	string tilda = "\\";
	string tildaColon = ":\\";
	rulesProgramm();
	cout << "Что вы хотите найти ?" << "\n\n";
	char choice = keyboard::keyChoice();

	if (choice == 'a'){
		char selectedDrive = keyboard::diskSearch();
		if (selectedDrive != '\0'){
			string extensions;

		}
	}
	else if (choice == 'b'){
		char selectedDrive = keyboard::diskSearch();
		if (selectedDrive != '\0') {
			string directory;
			string conStr;
			string filenameOr;
			string filename;
			string drivetild = selectedDrive + tildaColon;
			cout << "Введите путь к директории для поиска (например, " << selectedDrive << ":\\example\\:" << endl;
			cout << drivetild;
			getline(cin, directory);
			if (directory == " "){
				 conStr = selectedDrive + tildaColon;
			}
			else
			{
				 conStr = selectedDrive + tildaColon + directory;
			}
			cout << "Введите название файла для поиска: ";
			getline(cin, filenameOr);
			if (filenameOr == " "){
				filename = conStr;
			}
			else
			{
				filename = filenameOr;
			}

			find_files(conStr, filename);
		}
		else {
			cout << "Поиск файлов отменен" << endl;
		}
	}
	else if (choice == 'c'){
		char selectedDrive = keyboard::diskSearch();
		if (selectedDrive != '\0') {
			string directory;
			cout << "Введите путь к директории или диск для поиска (например, " << selectedDrive << ":\\example\\:";
			getline(cin, directory);
			string filename;
			cout << "Введите название директории для поиска: ";
			getline(cin, filename);

			find_directories(directory, filename);
		}
	}

}

int main()
{
	setlocale(LC_ALL, "ru");
	logic();
	
	// Сделать поиск файла по 1 совпадающей букве
	return 0;
}

