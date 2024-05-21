#define _CRT_SECURE_NO_WARNINGS // Директива для отключения предупреждений о небезопасных функциях
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <vector>
#include <cctype>
#include <chrono>
using namespace std;
namespace fs = std::filesystem;

namespace Metric {

    // Функция для замены неподдерживаемых символов 
    string wstring_to_string(const wstring& wstr) {
        return string(wstr.begin(), wstr.end());
    }

    struct FileInfo {
        string path;
        chrono::time_point<chrono::system_clock> last_write_time;
        uintmax_t size;
    };

    vector<FileInfo> collectFileMetadata(const string& directory) {
        vector<FileInfo> fileInfoList;
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (fs::is_regular_file(entry.status())) {
                FileInfo info;
                info.path = entry.path().string();
                auto ftime = fs::last_write_time(entry);

                // Преобразование file_time_type в time_point типа system_clock
                auto cftime = chrono::time_point_cast<chrono::system_clock::duration>(
                    ftime - fs::file_time_type::clock::now() + chrono::system_clock::now());
                info.last_write_time = cftime;

                info.size = fs::file_size(entry);
                fileInfoList.push_back(info);
            }
        }

        // Сортируем один раз, после обработки всех файлов
        sort(fileInfoList.begin(), fileInfoList.end(),
            [](const FileInfo& a, const FileInfo& b) {
                return a.last_write_time > b.last_write_time;
            });

        return fileInfoList;
    }

    bool Letter(char c) {
        return isalpha(c) || c == ' ' || c == ',' || c == '.' || c == '!' || c == '?' ||
            c == '<' || c == '>' || c == '&' || c == '$' || c == '@' || c == ';' || c == '[' || c == ']' || c == '{' || c == '}'
            || c == '+' || c == '=' || c == '-' || c == '~' || c == '`' || c == '_' || c == '/' || c == '\\' || c == '|';
    }

    template<typename T>
    typename T::size_type LevenshteinDistance(const T& source, const T& target) {
        if (source.size() > target.size()) {
            return LevenshteinDistance(target, source);
        }

        using TSizeType = typename T::size_type;
        const TSizeType min_size = source.size(), max_size = target.size();
        vector<TSizeType> lev_dist(min_size + 1);

        for (TSizeType i = 0; i <= min_size; ++i) {
            lev_dist[i] = i;
        }

        for (TSizeType j = 1; j <= max_size; ++j) {
            TSizeType previous_diagonal = lev_dist[0], previous_diagonal_save;
            ++lev_dist[0];

            for (TSizeType i = 1; i <= min_size; ++i) {
                previous_diagonal_save = lev_dist[i];
                if (source[i - 1] == target[j - 1]) {
                    lev_dist[i] = previous_diagonal;
                }
                else {
                    lev_dist[i] = min(min(lev_dist[i - 1], lev_dist[i]), previous_diagonal) + 1;
                }
                previous_diagonal = previous_diagonal_save;
            }
        }

        return lev_dist[min_size];
    }

    double calculateMatching(const string& textOne, const string& textTwo) {
        // Создаем строки, содержащие только буквы из исходных строк
        string filteredText1, filteredText2;
        for (char c : textOne) {
            if (Letter(c)) {
                filteredText1 += tolower(c);
            }
        }
        for (char c : textTwo) {
            if (Letter(c)) {
                filteredText2 += tolower(c);
            }
        }

        // Если одна из строк пустая после фильтрации, возвращаем 0% совпадения
        if (filteredText1.empty() || filteredText2.empty()) {
            return 0.0;
        }

        // Вычисляем расстояние Левенштейна и нормализуем его
        double distance = LevenshteinDistance(filteredText1, filteredText2);
        double maxLength = max(filteredText1.size(), filteredText2.size());

        // Возвращаем процент совпадения
        double matchPercentage = (1.0 - (distance / maxLength)) * 100;
        return matchPercentage;
    }

   bool searchTextFiles(const string& directory, const string& searchText,/*double Percent,*/ bool metadata = false, bool recursSeh = false) {
        
        try {
            vector<FileInfo> fileInfoList = collectFileMetadata(directory);
            if (!fs::exists(directory)) {
                cout << "Директория не существует" << endl;
                return false;
            }
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                    ifstream file(entry.path());
                    wstring current_textname = entry.path().stem().wstring();
                    string textLine = wstring_to_string(current_textname);
                    int lineNumber = 0;
                    while (getline(file, textLine)) {
                        lineNumber++;
                        double matchingPercentage = calculateMatching(searchText, textLine);
                        if (matchingPercentage >= 50.0/*|| matchingPercentage >= Percent*/) {
                            cout << "найдено в файле " << entry.path().filename() << ", строка " << lineNumber << ": " << textLine << endl;
                            cout << "Процент совпадающих букв: " << matchingPercentage << "%" << endl;
                            if (metadata) {
                                // Находим соответствующий FileInfo
                                auto it = find_if(fileInfoList.begin(), fileInfoList.end(), [&](const FileInfo& info) {
                                    return info.path == entry.path().string();
                                    });
                                if (it != fileInfoList.end()) {
                                    // т.к функция 'ctime' является небезопасной по причине отсутствия проверок на переполнение буфера.
                                    //используют 'ctime_s', но т.к в С++17 нет стандратной функции 'ctime_s', использую функцию 'put_time' для форматирования времени.
                                    auto last_write_time = chrono::system_clock::to_time_t(it->last_write_time);
                                    time_t time = last_write_time;
                                    tm* time_info = localtime(&time);
                                    char time_buffer[80];
                                    strftime(time_buffer, sizeof(time_buffer), "%c", time_info);
                                    cout << "Дата последнего изменения: " << ctime(&last_write_time);
                                    cout << " Размер файла: " << it->size << " байт";
                                }
                            }
                        }
                    }
                }
                else if (entry.is_directory() && recursSeh){
                    if (entry.path().filename() == "System Volume Information" || entry.path().filename() == "$RECYCLE.BIN") {
                        continue;
                    }
                    searchTextFiles(entry.path().string(), searchText, metadata, recursSeh);
                }
            }
        }
        catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
        return false;
    }

}