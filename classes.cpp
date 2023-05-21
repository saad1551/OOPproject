#include <functions.h>
#include <classes.h>
#include <string>
#include <iostream>

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
            string countryName = jsonResponse["geonames"][0]["countryName"];
            for (const auto& geoname : jsonResponse["geonames"]) {
                string name = geoname["name"];
                double latitude = geoname["latitude"];
                double longitude = geoname["longitude"];
                double population = geoname["population"];
                City city(name, latitude, longitude, population, countryName);
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

City::City(string name, double latitude, double longitude, double population, string countryName): Location(name, latitude, longitude)
{
    this->population = population;
    this->country_name = countryName;
}

void UIManager::ShowLocationDetails(City C)
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

vector<string> APIManager::getAttractionsByCity(const string& cityname)
{
    vector<string> attractions;
    CURL* curl;
    CURLcode res;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create a CURL handle
    curl = curl_easy_init();
    if (curl) {
        string apiKey = "962290E1A5934373A6DDBAC7920D4C30"; // Replace with your TripAdvisor API key
        string city = cityname;
        // Construct the URL for the TripAdvisor API
        string url = "https://api.tripadvisor.com/data/1.0/locations/search?query=" + city + "&key=" + apiKey;

        // Set the URL to send the request to
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to receive the response
        string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            cout << "Failed to retrieve data: " << curl_easy_strerror(res) << endl;
        }
        else {
            // Parse the response as JSON
            json jsonResponse = json::parse(response);

            // Extract travel attractions from the response
            if (jsonResponse.find("data") != jsonResponse.end()) {
                for (const auto& location : jsonResponse["data"]) {
                    if (location.find("result_object") != location.end()) {
                        const auto& resultObject = location["result_object"];
                        string attractionName = resultObject["name"];
                        attractions.push_back(attractionName);
                    }
                }
            }
            else {
                cout << "No travel attractions found." << endl;
            }
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }
    else {
        cout << "Failed to initialize CURL." << endl;
    }

    // Cleanup libcurl
    curl_global_cleanup();
}

void City::AddAttraction(Attraction d)
{
    AttractionsToBeVisited.push_back(d);
}

double APIManager::get_latitude(const string& place_name)
{
    CURL* curl = curl_easy_init();
    if (curl) {
        // Set the Mapbox Geocoding API endpoint
        string url = "https://api.mapbox.com/geocoding/v5/mapbox.places/";
        string place = place_name;  // Replace with the desired place
        string accessToken = "pk.eyJ1Ijoic2FhZGFzaHJhZiIsImEiOiJjbGhuczR6Y20xcDZwM2VudWh0M3hwOHg1In0.XjIfoHbX2IiyiyJsbw1euw";  // Replace with your Mapbox access token

        // Append the place and access token to the API URL
        url += place + ".json?access_token=" + accessToken;

        // Create a buffer to store the response
        string buffer;

        // Set the options for libcurl
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            // Parse the JSON response
            json response = json::parse(buffer);

            // Retrieve the latitude
            double latitude = response["features"][0]["center"][1];
            return latitude;
        } else {
            cout << "Failed to perform request for latitude: " << curl_easy_strerror(res) << endl;
            return 1;
        }

        // Clean up
        curl_easy_cleanup(curl);
    } else {
        cout << "Failed to initialize libcurl" << endl;
        return 2;
    }
}

double APIManager::get_longitude(const string& place_name)
{
     curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();

    if (curl) {
        // Set the URL for the API request
        string place = place_name;  // Specify the place name
        string url = "https://api.mapbox.com/geocoding/v5/mapbox.places/" + place + ".json?access_token=pk.eyJ1Ijoic2FhZGFzaHJhZiIsImEiOiJjbGhuczR6Y20xcDZwM2VudWh0M3hwOHg1In0.XjIfoHbX2IiyiyJsbw1euw";

        // Configure libcurl options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        // Response string
        string response_string;

        // Set the callback function to write the response into the string
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            // Parse the JSON response
            json response = json::parse(response_string);

            // Retrieve the longitude from the response
            double longitude = response["features"][0]["center"][0];
            return longitude;
        } else {
            // Error occurred during the request
            cerr << "Request failed: " << curl_easy_strerror(res) << endl;
        }

        // Clean up libcurl and resources
        curl_easy_cleanup(curl);
    }

    // Clean up libcurl global state
    curl_global_cleanup();

}

void UIManager::ShowLocationDetails(Attraction A)
{
    cout << "Name: " << A.Location::get_name() << endl;
    cout << "Latitude: " << A.Location::get_longitude() << endl;
    cout << "Longitude: " << A.Location::get_longitude() << endl;
    cout << "Population: " << A.get_avgspend() << endl;
};

void Location::SetName(const string& name)
{
    this->name_ = name;
}

void Location::SetLatitude(double latitude)
{
    this->latitude_ = latitude;
}

void Location::SetLongitude(double longitude)
{
    this->longitude_ = longitude;
}

void City::SetPopulation(int population)
{
    this->population = population;
}

City APIManager::get_city(string const& cityname)
{
    City C;
    string username = "saadashraf";
    string apiUrl = "http://api.geonames.org/searchJSON?q=" + cityname + "&maxRows=1&username=" + username;

    CURL* curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        string response;

        // Set the URL and callback function
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            curl_easy_cleanup(curl);
            return C;
        }

        // Parse the JSON response
        json jsonResponse = json::parse(response);

        // Extract the desired information from the JSON
        double latitude = jsonResponse["geonames"][0]["lat"];
        double longitude = jsonResponse["geonames"][0]["lng"];
        string countryName = jsonResponse["geonames"][0]["countryName"];
        int population = jsonResponse["geonames"][0]["population"];

        C.SetName(cityname);
        C.SetLatitude(latitude);
        C.SetLongitude(longitude);
        C.SetPopulation(population);

        curl_easy_cleanup(curl);
    }
    return C;
}

string City::get_country_name()
{
    return country_name;
}

double City::get_population()
{
    return population;
}

double Location::get_latitude()
{
    return latitude_;
}

double Location::get_longitude()
{
    return longitude_
}

ItineraryPlanner::ItineraryPlanner(City sourceCity, vector<City> destinationCities, int travelDays)
{
    this->sourceCity = sourceCity;
    this->destinationCities = destinationCities;
    this->travelDays = travelDays;
}

void Itinerary::AddTravelDay(TravelDay travelDay)
{
    TravelDays.push_back(travelDay);
}

vector<City> Sorter::SortByOrderOfTravel(City sourceCity, vector<City> destinationCities)
{
    vector<double> distanceFromSourceCity;
    APIManager Retriever;
    for (int i = 0; i < destinationCities.size(); i++)
    {
        //distanceFromSourceCity.push_back(Retriever.get_distance(sourceCity.get_latitude(), sourceCity.get_longitude(), destinationCities[i].get_latitude(), destinationCities[i].get_longitude())
    }
    vector<City> SortedCities = SortByDistance(sourceCity, distanceFromSourceCity, destinationCities);
}

vector<Attraction> SortByOrderOfTravel(vector<City> orderedCities)
{
    vector<Attraction> orderedAttractions;
    for (int i = 0; i < orderedCities.size(); i++)
    {
        Attraction sourceAttraction = orderedCities[i].get_attractions_to_be_visited()[0];
        
    }
};

Itinerary ItineraryPlanner::PlanIntraCountry(City sourceCity, vector<City> destinationCities, int travelDays)
{
    Itinerary itinerary;
    TravelDay travelday;
    int travellinghours = GetTravellingHours(travelDays);
    Sorter sorter;
    vector<City> OrderedCities = sorter.SortByOrderOfTravel(sourceCity, destinationCities);
    vector<Attraction> OrderedAttractions = sorter.SortByOrderOfTravel()
    itinerary.AddTravelDay(travelday);
}

vector<Attraction> City::get_attractions_to_be_visited() const
{
    return AttractionsToBeVisited;
}

Attraction::Attraction(const Attraction& other) : Location(other.get_name(), other.get_latitude(), other.get_longitude()), avgspend(other.avgspend)
{

};

City::City(const City& other) : Location(other.get_name(), other.get_latitude(), other.get_longitude()), country_name(other.country_name), population(other.population)
{
    for (int i = 0; i < other.get_attractions_to_be_visited.size(); i++)
    {
        AttractionsToBeVisited.push_back(Attraction(other.get_attractions_to_be_visited[i].get_name(), other.get_attractions_to_be_visited[i].get_latitude(), other.get_attractions_to_be_visited[i].get_longitude(), other.get_attractions_to_be_visited[i].get_avgspend()));
    }
}