#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <algorithm>
#include <Windows.h>
#include <conio.h>
#include <unordered_set>
#include <shellapi.h>
#include <system_error>
#include <codecvt>
#include <cctype> 
#include <locale>
#include "InteractionWTK.h"
#include "RulesProgramm.h"
#include "Distance.h"

using namespace std;
namespace fs = std::filesystem;


void logic();
// Функция проверяет запущена ли программа с правами администратора
bool runAdmin() {
	BOOL isAdmin = false;
	PSID adminGroup = NULL;
	SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

	if (AllocateAndInitializeSid(&ntAuthority,2, SECURITY_BUILTIN_DOMAIN_RID,DOMAIN_ALIAS_RID_ADMINS,
		0,0,0,0,0,0, &adminGroup)){
		if (!CheckTokenMembership(NULL, adminGroup, &isAdmin)){
			isAdmin = false;
		}
		FreeSid(adminGroup);
	}
	
	return isAdmin;

}
// Функция изменения цвета
void setColor(int color) {
	// Цвета: 0 - черный, 1 - синий, 2 - зеленый, 3 - бирюзовый, 4 - красный, 5 - фиолетовый, 6 - желтый, 7 - белый
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, color);
}
// Функция для удаления пробелов 
string trim(const string& str) {
	size_t first = str.find_first_not_of(' ');
	if (string::npos == first){
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, last - first + 1);
}
// Функция считывания знаков препинания 
bool isLetter(char c) {
	return isalpha(c) || c == ' ' || c == ',' || c == '.' || c == '!' || c == '?';
}

string toLowers(string s) {
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}


//Функция перезапуска программы
void restartProgramm() {
	char restart = keyboard::keyYesOrNoMod();
	if (restart == 'y'){
		return;
	}
	else if(restart == 'n'){
		logic();
	}
	else
	{
		cerr << "Ошибка -_-" << endl;
	}
}
//Функция выбора параметров поиска
bool searchParameters(bool SearchSelection,bool recursSeh) {

	bool SearchSelection1 = false; // Частично совпадающие переменные 
	bool recursSeh1 = false;// Поиск по всем директориям

	cout << "Выводить ли частично совпадающие файлы ? " << endl;
	char yesOrno = keyboard::keyYesOrNo();
	if (yesOrno == 'y') {
		SearchSelection = true;
	}
	cout << "Искать во всех директориях диска ?" << endl;

	char yesOrNoDirect = keyboard::keyYesOrNo();
	if (yesOrNoDirect == 'y') {
		recursSeh = true;
	}
	return false;
}
bool metaRecursivDistance(bool SearchSelection, bool recursSeh) {
	bool SearchSelection1 = false; // Метаданные
	bool recursSeh1 = false;// Поиск по всем директориям
	cout << "Выводить ли метаданные ?" << endl;
	char yesOrno = keyboard::keyYesOrNo();
	if (yesOrno == 'y') {
		SearchSelection = true;
	}

	cout << "Искать во всех директориях диска ?" << endl;

	char yesOrNoDirect = keyboard::keyYesOrNo();
	if (yesOrNoDirect == 'y') {
		recursSeh = true;
	}
	return false;
}
vector<fs::path> find_extension(const fs::path& directory, const string& extensionName) {
	
	vector<fs::path> filesExtension;
	try {
		for (const auto& entry : fs::recursive_directory_iterator(directory)) {
			// Проверка, является ли текущий путь к фалам и соответствует ли его расширение заданному
			try
			{
				if (entry.is_regular_file() && entry.path().extension() == extensionName) {
					filesExtension.push_back(entry.path().string());
				}
			}
			catch (const fs::filesystem_error& e ){
				cerr << "Ошибка файловой системый" << e.what() << endl;
				continue;
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		cerr << "Ошибка файловой системый" << e.what()<< endl;
	}
	for (const auto& patch : filesExtension) {
		setColor(2);
		cout << patch.string() << " - Искомое расширение" << endl;
		setColor(7);
	}
	
	return filesExtension;
}

 vector<fs::path> find_files(const fs::path& directory, const string& filename, bool sehSelection, bool recursSeh ) {

	vector<fs::path> exact_matches; // Вектор для сохранения искомых файлов
	vector<fs::path> partial_matches; // Частично совпадающие

	string lower_filename = filename;

	transform(lower_filename.begin(), lower_filename.end(), lower_filename.begin(), ::tolower);
	try{
		
		for (const auto& entry : fs::recursive_directory_iterator(directory)) {
			if (entry.is_regular_file()) {

				wstring current_wfilename = entry.path().stem().wstring();
				string current_filename = Metric::wstring_to_string(current_wfilename);
				// Преобразуем текущее имя файла в нижний регистр
				transform(current_filename.begin(), current_filename.end(), current_filename.begin(), ::tolower);

				// Проверяем, содержит ли имя файла искомое имя
				if (current_filename == lower_filename) {
					exact_matches.push_back(entry.path()); //Сохраняем путь найденных файлов
				}
				else if (sehSelection) {
					auto it = search(current_filename.begin(), current_filename.end(), lower_filename.begin(), lower_filename.end());
					if (it != current_filename.end())
					{
						partial_matches.push_back(entry.path());
					}
				}
			}
			if (recursSeh && entry.is_directory()) {
				if (entry.path().filename() == "System Volume Information" || entry.path().filename() == "$RECYCLE.BIN") {
					continue;
				}

				find_files(entry.path(), filename, sehSelection, recursSeh);

			}

		}

		for (const auto&patch :partial_matches){
			setColor(7);
			cout << patch.string() << " - Частично совпадающий файл" << endl;
		}
	
		for (const auto& patch : exact_matches ){
				setColor(2);
				cout << patch.string() << " - Искомый файл(ы)" << endl;
				setColor(7);
		}
	}
	catch (const fs::filesystem_error& e) {
		setColor(4);
		cerr << "" << e.what() << " (" << directory <<")" << endl;
		setColor(7);
	}

	return exact_matches, partial_matches;
}

 vector<fs::path> find_files_and_extension(const fs::path& directory, const string& filename, const string& extension,bool sehSelection, bool recursSeh) {

	 vector<fs::path> fileWithExtension = find_extension(directory, extension);

	 vector<fs::path> fileWithName = find_files(directory, filename, sehSelection, recursSeh);

	 vector<fs::path> intersection;
	 set_intersection(fileWithExtension.begin(), fileWithExtension.end(), fileWithName.begin(), fileWithName.end(), back_inserter(intersection));
	 try {
		 for (const auto& patch : intersection) {
			 cout << patch.string() << "- Файл с искомым именем и расширением" << endl;

			
		 }
	 }
	 catch (const fs::filesystem_error& e) {
		 cerr << "Ошибка файловой системый" << e.what() << endl;
		 
	 }
	 catch (const exception& e) {
		 cerr << "Ошибка файловой системый" << e.what() << endl;
	 }
	 return intersection;
 }
 
 vector<fs::path> find_directories(const fs::path& directory, const string& dirname, bool sehSelection, bool recursSeh) {

	vector<fs::path> exact_matches;
	vector<fs::path> partial_matches;

	string lower_dirname = toLowers(dirname);
	transform(lower_dirname.begin(), lower_dirname.end(), lower_dirname.begin(), ::tolower);
	try
	{
		for (const auto& entry : fs::recursive_directory_iterator(directory)) {
			if (entry.is_directory()) {
				string current_dirname = entry.path().filename().string();
				// Преобразуем текущее имя директории в нижний регистр
				transform(current_dirname.begin(), current_dirname.end(), current_dirname.begin(), ::tolower);

				if (current_dirname == lower_dirname) {
					exact_matches.push_back(entry.path());

				}else if (sehSelection && current_dirname.find(lower_dirname)!= string::npos) {
					partial_matches.push_back(entry.path());
				}
				if (recursSeh) {
					find_directories(entry.path(), dirname, sehSelection, recursSeh);
				}
			}

		}
		for (const auto& patch:partial_matches){
			setColor(7);
			cout << patch.string() << "- Частично совпадающая директория" << endl;
		}
		for (const auto& patch: exact_matches){
			setColor(2);
			cout << patch.string() << "- Искомая директория" << endl;
			setColor(7);
		}
	}
	catch (const fs::filesystem_error& e) {
		setColor(4);
		cerr << "" << e.what() << " (" << directory << ")" << endl;
		setColor(7);
	}

	return exact_matches, partial_matches;
}

void logic() {

	string tilda = "\\";
	string tildaColon = ":\\";
	string directory, strWhyChoice, filenameOr, extensionsWay, filenameOrDeltExtens;
	/*double procent = 50.0;*/

	rulesProgramm();
	if (runAdmin()){
		setColor(2);
		cout << "Программа запущена с правами администратора" << endl;
	}
	else{
		setColor(4);
		cout << "Предупреждение: программа  не имеет прав администратора. Некоторые функции будут ограничены" << endl;
	}
	setColor(7);

	bool SearchSelection = true; // Частично совпадающие переменные 
	bool recursSeh = true;// Поиск по всем директориям


	//Что искать, файл по расширению, название файла, папку.

	cout << "Что вы хотите найти ?" << "\n\n";
	char choice = keyboard::keyChoice();
	
	if (choice == 'a'|| choice == 'b' || choice == 'c'|| choice == 'd') {
		char selectedDrive = keyboard::diskSearch();
		if (selectedDrive !='\0'){
			string drivetild = string(1, selectedDrive) + tildaColon;
			cout << "Введите путь к директории для поиска (например, " << selectedDrive << ":\\example\\):" << endl;
			cout << drivetild;
			getline(cin, directory);

				if (directory.empty()) {// Проверка на пустую строку 
					// Если ничего не ввести в поиске директории, то будет вестись поиск по всему диску
					strWhyChoice = drivetild;
				}
				else
				{
					// Если ввести путь к директории
					strWhyChoice = drivetild + directory;
				}
			
			if (choice == 'a') {
				cout << "Введите расширение для поиска: ";
				
			}
			else if (choice == 'b') {
				searchParameters(SearchSelection, recursSeh);
				cout << "Введите название файла  для поиска: ";
				
			}
			else if (choice == 'c'){
				searchParameters(SearchSelection, recursSeh);
				cout << "Введите название и расширение файла  для поиска: ";
			}
			else if (choice =='d'){
				metaRecursivDistance(SearchSelection, recursSeh);
				cout << "Введите искомый текст: ";
			}
			else{
				searchParameters(SearchSelection, recursSeh);
				cout << "Введите название директории для поиска: ";
			}

			getline(cin, filenameOr);
			size_t found = filenameOr.find_last_of(".");
			if (found !=string::npos){
				extensionsWay = filenameOr.substr(found);
				filenameOrDeltExtens = filenameOr.substr(0, found);
			}
			else{
				filenameOrDeltExtens = filenameOr;
			}
			if (filenameOr.empty()){
				cerr << "Ошибка: Имя для поиска не может быть пустым" << endl; 
				return;
			}
			try{
				if (choice == 'a') {
					find_extension(strWhyChoice, filenameOr);
					restartProgramm();
				}
				else if (choice == 'b') {
					find_files(strWhyChoice, filenameOr, SearchSelection, recursSeh);
					restartProgramm();
				}
				else if (choice == 'c'){
					find_files_and_extension(strWhyChoice, filenameOrDeltExtens, extensionsWay, SearchSelection, recursSeh);
				}
				else if (choice == 'd') {
					/*cout << "Введите процент совпадений (процент можно вводить до десятых):" << endl;
				char prozentor = keyboard::keyYesOrNo();
				if (prozentor == 'y') {
					cin >> procent;
					if (procent > 100.0 || procent < 0.0)
					{
						cerr << "Ошибка: Процент совпадений не может быть больше 100.0 или меньше 0.0" << endl;
					}
				}*/

					Metric::searchTextFiles(strWhyChoice, filenameOr,/* procent,*/ SearchSelection, recursSeh);
				}
				else {
					find_directories(strWhyChoice, filenameOr, SearchSelection, recursSeh);
					restartProgramm();
				}
			}
			catch (const fs::filesystem_error& e) {
				cerr << "Ошибка файловой системы: " << e.what() << endl;
				ignore;
			}
			catch (const exception& e) {
				cerr << "Исключение: " << e.what() << endl;
			}
		}
		else {
			cout << "Поиск файлов отменен" << endl;
		}
	}

}


int main()
{
	SetConsoleOutputCP(CP_UTF8);
	wcout.imbue(std::locale(""));
	setlocale(LC_ALL, "ru");
	logic();
	
	
		return 0;
}