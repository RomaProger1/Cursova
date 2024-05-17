#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <thread>
#include <algorithm>
#include <Windows.h>
#include <conio.h>
#include <unordered_set>
#include <shellapi.h>
#include <system_error>
#include <codecvt>
#include <locale>
#include "InteractionWTK.h"
#include "RulesProgramm.h"

using namespace std;
namespace fs = std::filesystem;


void logic();
// функция проверяет запущена ли программа с правами администратора
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
// функция для замены неподдерживаемых символов 
wstring swapErrorsUtf8(const string& utf8) {
	int wstr_len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
	wstring utf16(wstr_len, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &utf16[0], wstr_len);
	return utf16;
}
string swapErrorsUtf16(const wstring& utf16) {
	int str_len = WideCharToMultiByte(CP_UTF8, 0, utf16.c_str(), -1, nullptr, 0, nullptr, nullptr);
	string utf8(str_len, 0);
	WideCharToMultiByte(CP_UTF8, 0, utf16.c_str(), -1, &utf8[0], str_len, nullptr, nullptr);
	return utf8;
}

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

void find_files(const fs::path& directory, const string& filename, bool sehSelection, bool recursSeh /*,bool thr*/) {

	string lower_filename = filename;
	transform(lower_filename.begin(), lower_filename.end(), lower_filename.begin(), ::tolower);
	try{
		/*vector<thread> threads;*/
		for (const auto& entry : fs::directory_iterator(directory)) {
			if (entry.is_regular_file()) {

				wstring current_wfilename = entry.path().stem().wstring();
				string current_filename = swapErrorsUtf16(current_wfilename);
				// Преобразуем текущее имя файла в нижний регистр
				transform(current_filename.begin(), current_filename.end(), current_filename.begin(), ::tolower);

				// Проверяем, содержит ли имя файла искомое имя
				if (current_filename == lower_filename) {
					setColor(2);
					cout << entry.path() << " Искомый файл" << endl;

				}
				else if (sehSelection) {
					auto it = search(current_filename.begin(), current_filename.end(), lower_filename.begin(), lower_filename.end());
					if (it != current_filename.end())
					{
						setColor(7);
						cout << entry.path() << " Частично совпадающий файл" << endl;
					}
				}


			}
			else if (recursSeh && entry.is_directory()) {
				//if (thr) {
				//	threads.emplace_back([&entry, &filename, sehSelection, recursSeh, thr]() {
				//		find_files(entry.path(), filename, sehSelection, recursSeh, thr);
				//		});
				//}
				if (entry.path().filename() == "System Volume Information" || entry.path().filename() == "$RECYCLE.BIN") {
					continue;
				}

				find_files(entry.path(), filename, sehSelection, recursSeh /*,thr*/);

			}

		}
		//for (auto& thread : threads) {
		//	thread.join();
		//}
	}
	catch (const fs::filesystem_error& e) {
		setColor(4);
		cerr << "" << e.what() << " (" << directory <<")" << endl;
		setColor(7);
	}
}

void find_directories(const fs::path& directory, const string& dirname ) {

	string lower_ariname = dirname;
	transform(lower_ariname.begin(), lower_ariname.end(), lower_ariname.begin(), ::tolower);

	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.is_directory()) {
			string current_dirname = entry.path().filename().string();
			// Преобразуем текущее имя директории в нижний регистр
			transform(current_dirname.begin(), current_dirname.end(), current_dirname.begin(), ::tolower);

			auto it = search(current_dirname.begin(), current_dirname.end(), lower_ariname.begin(), lower_ariname.end());
			if (current_dirname == lower_ariname){
				cout << entry.path() << " - Искомая папка" << endl;

			
			}
			else if (it != current_dirname.end()) {
				cout << entry.path() << " Частично совпадающие файлы" << endl;
			}
			
		}

	}

}
void logic() {

	string tilda = "\\";
	string tildaColon = ":\\";
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
	cout << "Выводить ли частично совпадающие файлы ? " << endl;

	bool SearchSelection;
	char yesOrno = keyboard::keyYesOrNo();
	if (yesOrno == 'y'){
		SearchSelection = true;
	}
	else if (yesOrno == 'n') {
		SearchSelection = false;
	}

	cout << "Искать во всех директориях диска ?" << endl;

	bool recursSeh;
	char yesOrNoDirect = keyboard::keyYesOrNo();
	if (yesOrNoDirect == 'y') {
		recursSeh = true;
	}
	else if (yesOrNoDirect == 'n'){
		recursSeh = false;
	}

	//cout << "Включить функцию параллельности ? " << endl;
	//bool parall;
	//char yesOrNoParall = keyboard::keyYesOrNo();
	//if (yesOrNoParall == 'y') {
	//	parall = true;
	//}
	//else if (yesOrNoParall == 'n'){
	//	parall = false;
	//}

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

			try{
				find_files(conStr, filename, SearchSelection, recursSeh/*, parall*/);
			}
			catch (const fs::filesystem_error& e){
				cerr << "Ошибка файловой системы: " << e.what() << endl;
			}
			catch (const exception& e) {
				cerr << "Исключение: " << e.what() << endl;
			}
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
	SetConsoleOutputCP(CP_UTF8);
	wcout.imbue(std::locale(""));
	setlocale(LC_ALL, "ru");
	logic();
	
	return 0;
}

