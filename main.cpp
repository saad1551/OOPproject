#include <iostream>
#include <string>
#include <vector>
#include <classes.h>
#include <functions.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cctype>

using namespace std;
using json = nlohmann::json;

int main()
{
    APIManager Retriever; //an object that handles API calls
    vector<string> Countries = Retriever.get_countries_list(); //get the names of all countries of the world
    
    string CountryToBeVisited = SelectCountry(Countries); // name of the country that is to be visited
    string CountryCode = Retriever.getCountryCode(CountryToBeVisited); //code of the country that is to be visited
    vector<City> Cities = Retriever.getCitiesByCountry(CountryCode); //objects of all the cities of that country
    vector<City> CitiesToBeVisited = SelectCities(Cities); //objects of the specific cities that are to be visited
    vector<Attraction> AttractionsToBeVisited = SelectAttractions(CitiesToBeVisited); //objects of all the attractions that are to be visited
    City currentCity = FindCurrentCity(); //object of the current city of the user
    int travelDays = GetTravelDays();
    Itinerary TravelItinerary = CreateItinerary(currentCity, CitiesToBeVisited);

    return 0;
}