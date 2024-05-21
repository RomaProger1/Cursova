#pragma once
#include <iostream>

namespace Metric {
	struct FileInfo {};
	vector<FileInfo> collectFileMetadata(const string& directory);
	bool Letter(char c);
	template<typename T>
	typename T::size_type LevenshteinDistance(const T& source, const T& target);
	double calculateMatching(const std::string& textOne, const std::string& textTwo);
	bool searchTextFiles(const string& directory, const string& searchText,/* double Percent,*/ bool metadata = false, bool recursSeh = false);
	string wstring_to_string(const wstring& wstr);
}