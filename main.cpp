#include <iostream>
#include <string>
#include <vector>
#include "classes.cpp"
#include "functions.cpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cctype>

using namespace std;
using json = nlohmann::json;

int main()
{
    cout << "Welcome to the travel itinerary planner.\nSelect a country you want to visit:\n" << endl;

    APIManager Retriever;
    vector<string> Countries = Retriever.get_countries_list();
    string CountryToBeVisited = SelectCountry(Countries);
    string CountryCode = Retriever.getCountryCode(CountryToBeVisited);
    vector<City> Cities = Retriever.getCitiesByCountry(CountryCode);
    vector<City> CitiesToBeVisited = SelectCities(Cities);
    vector<Destination> DestinationsToBeVisited = SelectDestinations(CitiesToBeVisited);

    return 0;
}