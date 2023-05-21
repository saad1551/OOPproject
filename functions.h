#ifndef FUNCTIONSH
#define FUNCTIONSH

#include <cstddef>
#include <classes.h>

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
vector<City> SelectCities(vector<string>);
string SelectCountry(vector<string>);
vector <Attraction> SelectAttractions(vector<City>);
City FindCurrentCity(void);
Itinerary CreateItinerary(City, vector<City>);
int GetTravelDays();
int GetTravellingHours(int TravelDays);
vector<City> SortByDistance(City, vector<double>, vector<City>);
vector<Attraction>

#endif