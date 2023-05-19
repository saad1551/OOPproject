#include <string>
#include <iostream>
#include "classes.h"
#include "functions.h"


using namespace std;

vector<string> APIManager::get_countries_list()
{
    string url = "https://restcountries.com/v2/all";
    
    
    CURL* curl = curl_easy_init();
    vector<string> country_names;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        string response;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        

        if (res == CURLE_OK) {
            json j = json::parse(response);
            for (auto& country : j) {
                string name = country["name"].get<string>();
                country_names.push_back(name);
            }
            //for (int i = 1; i <= country_names.size(); i++)
            //{
             //   cout << "i: " << country_names[i];
            //}
        }
        else {
            cerr << "Error: " << curl_easy_strerror(res) << endl;
        }
    }
    return country_names;
}

// Function to retrieve cities for a given country using the Geonames API
vector<City> APIManager::getCitiesByCountry(const string& country) {
    vector<City> cities;

    // Set your Geonames API username
    const string username = "saadashraf";

    // Create the request URL
    string url = "http://api.geonames.org/searchJSON?"
                      "country=" + country +
                      "&maxRows=1000"  // Maximum number of rows to retrieve
                      "&featureClass=P"  // Only retrieve populated places
                      "&username=" + username;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();

    if (curl) {
        string response;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the HTTP request
        CURLcode result = curl_easy_perform(curl);

        if (result == CURLE_OK) {
            // Parse the JSON response
            json jsonResponse = json::parse(response);

            // Extract the cities from the response
            for (const auto& geoname : jsonResponse["geonames"]) {
                string name = geoname["name"];
                double latitude = geoname["latitude"];
                double longitude = geoname["longitude"];
                double population = geoname["population"];
                City city(name, latitude, longitude, population);
                cities.push_back(city);
            }
        } else {
            cout << "HTTP request failed: " << curl_easy_strerror(result) << endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
    } else {
        cout << "Failed to initialize libcurl" << endl;
    }

    // Clean up libcurl
    curl_global_cleanup();

    return cities;
}

string APIManager::getCountryCode(const string& countryName)
{
    string countryCode;

    string apiUrl = "https://restcountries.com/v3.1/name/" + countryName;

    CURL* curl = curl_easy_init();
    if (curl) {
        string response;
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            json countryData = json::parse(response);

            // Extract the country code from the JSON response
            countryCode = countryData[0]["cca2"].get<string>();
        } else {
            cout << "Error: Failed to perform HTTP request. Error code: " << res << endl;
        }

        curl_easy_cleanup(curl);
    }

    return countryCode;
}

int UIManager::show_list(vector<string> locations)
{
    int choice;
    for (int i = 0; i < locations.size(); i++)
    {
        cout << "i + 1: " << locations[i] << endl;
    }
    cin >> choice;
    return choice-1;
}

City::City(string name, double latitude, double longitude, double population): Location(name, latitude, longitude)
{
    this->population = population;
}

void UIManager::ShowCityDetails(City C)
{
    cout << "Name: " << C.Location::get_name() << endl;
    cout << "Latitude: " << C.Location::get_longitude() << endl;
    cout << "Longitude: " << C.Location::get_longitude() << endl;
    cout << "Population: " << C.get_population() << endl;
}

int UIManager::show_list(vector<City> cities)
{
    int choice;
    for (int i = 0; i < cities.size(); i++)
    {
        cout << "i + 1: " << cities[i].get_name() << endl;
    }
    cin >> choice;
    return choice-1;
}

vector<string> APIManager::getDestinationsByCity(const string& cityname)
{

}

void City::AddDestination(Destination d)
{
    DestinationsToBeVisited.push_back(d);
}