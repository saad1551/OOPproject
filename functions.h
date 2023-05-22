#ifndef FUNCTIONSH
#define FUNCTIONSH

#include <cstddef>
#include <classes.h>

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
vector<City> SelectCities(vector<string>);
string SelectCountry(vector<string>);
double calculateDistance(double lat1, double lon1, double lat2, double lon2);
vector<Attraction> SelectAttractions(vector<City>);
City FindCurrentCity(void);
Itinerary CreateItinerary(City, vector<City>);
int GetTravelDays();
int GetTravellingHours(int TravelDays);
vector<City> SortByDistance(City, vector<double>, vector<City>);
vector<Attraction> SortByDistance(Attraction, vector<double>, vector<Attraction>);

#endif