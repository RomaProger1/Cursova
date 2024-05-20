#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <vector>
#include <cctype>
using namespace std;
namespace fs = std::filesystem;

namespace Metric {
    bool Letter(char c) {
        return isalpha(c) || c == ' ' || c == ',' || c == '.' || c == '!' || c == '?';
    }

    template<typename T>
    typename T::size_type LevenshteinDistance(const T& source, const T& target) {
        if (source.size() > target.size()) {
            return LevenshteinDistance(target, source);
        }

        using TSizeType = typename T::size_type;
        const TSizeType min_size = source.size(), max_size = target.size();
        std::vector<TSizeType> lev_dist(min_size + 1);

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
                    lev_dist[i] = std::min(std::min(lev_dist[i - 1], lev_dist[i]), previous_diagonal) + 1;
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
        double maxLength = std::max(filteredText1.size(), filteredText2.size());

        // Возвращаем процент совпадения
        double matchPercentage = (1.0 - (distance / maxLength)) * 100;
        return matchPercentage;
    }

    void searchTextFiles(const string& directory, const string& searchText) {
        try {
            if (!fs::exists(directory)) {
                cout << "Директория не существует" << endl;
                return;
            }
            for (const auto& entry : fs::directory_iterator(directory)) {
                if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                    ifstream file(entry.path());
                    string line;
                    int lineNumber = 0;
                    while (getline(file, line)) {
                        lineNumber++;
                        double matchingPercentage = calculateMatching(searchText, line);
                        if (matchingPercentage >= 50.0) {
                            cout << "найдено в файле " << entry.path().filename() << ", строка " << lineNumber << ": " << line << endl;
                            cout << "Процент совпадающих букв: " << matchingPercentage << "%" << endl;
                        }
                    }
                }
            }
        }
        catch (const std::exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

}