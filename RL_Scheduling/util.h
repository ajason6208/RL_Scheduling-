#pragma once

#include <map>
#include <iostream>

using namespace std;

template<typename KeyType, typename ValueType>
std::pair<KeyType, ValueType> get_max_map_value(const std::map<KeyType, ValueType>& x) {
	using pairtype = std::pair<KeyType, ValueType>;
	return *std::max_element(x.begin(), x.end(), [](const pairtype & p1, const pairtype & p2) {
		return p1.second < p2.second;
	});
}