#pragma once
#include <iostream>
namespace Metric {
	bool Letter(char c);
	template<typename T>
	typename T::size_type LevenshteinDistance(const T& source, const T& target);
	double calculateMatching(const std::string& textOne, const std::string& textTwo);
	void searchTextFiles(const std::string& directory, const std::string& searchText);

}