#pragma once

#include <fstream>
#include <iostream>
#include <stdexcept>	// std::exception
#include <string_view>	// std::string_view

#include <boost/json.hpp>
#include <boost/json/value.hpp>

namespace handyman::json {

// This helper function deduces the type and assigns the value with the matching key if found.
template<class T>
void extract_if_contains(boost::json::object const& obj, T& t, std::string_view key)
{
	const boost::json::value* v = obj.if_contains(key);
	if (v != nullptr) {
		t = value_to<T>(*v);
	}
}

template <typename T>
void to_file(const T& t, const std::string& filepath)
{
	try {
		const std::string	s = boost::json::serialize(boost::json::value_from(t));
		std::ofstream myfile;
		myfile.open(filepath);
		if (myfile.is_open()) {
			myfile << s << "\n";
			myfile.close();
		}
	}
	catch (const std::exception&) {
		// pass.
	}
}

template <typename T>
T from_file_or_default(const std::string& filepath)
{
	try {
		std::ifstream myfile;
		myfile.open(filepath);
		if (myfile.is_open()) {
			boost::json::value jv = boost::json::parse(myfile);
			myfile.close();
			T r(boost::json::value_to<T>(jv));
			return r;
		}
	}
	catch (const std::exception&) {
		// pass.
	}

	return T::default_value();
}



} // namespace handyman::json
