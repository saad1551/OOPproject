#ifndef FUNCTIONSH
#define FUNCTIONSH

#include <cstddef>

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
vector<City> SelectCities(vector<string>);
string SelectCountry(vector<string>);

#endif