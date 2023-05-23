#include <classes.h>
#include <functions.h>

#define TRAVELLING_HOURS_PER_DAY 7
#define HOURS_PER_ATTRACTION 2
#define BREAKFAST_TIME 7
#define LUNCH_TIME 1
#define DINNER_TIME 8
#define TIMESPENT 2

using namespace std;

vector<string> APIManager::get_countries_list()
{
    vector<string> countries;

    // Geonames API endpoint
    string url = "http://api.geonames.org/countryInfoJSON?formatted=true&lang=en&username=saadashraf";

    // Perform the HTTP GET request
    CURL* curl = curl_easy_init();
    if (curl) {
        // Set the API URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to receive the response
        string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        CURLcode result = curl_easy_perform(curl);
        if (result == CURLE_OK) {
            // Parse the JSON response
            json data = json::parse(response);

            // Extract the list of countries
            for (const auto& country : data["geonames"]) {
                countries.push_back(country["countryName"].get<string>());
                //cout << country["countryName"].get<string>() << endl;
            }
        } else {
            cerr << "Error: " << curl_easy_strerror(result) << endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
    } else {
        cerr << "Error initializing libcurl" << endl;
    }
    //cout << "getcountries function has ended" << endl;
    return countries;
}


Location::Location(string name, double latitude, double longitude)
{
    name_ = name;
    latitude_ = latitude;
    longitude_ = longitude;
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
        cout << i + 1 << ": "  << locations[i] << endl;
    }
    cin >> choice;
    return choice-1;
}

City::City(string name, double latitude, double longitude, double population, string countryName): Location(name, latitude, longitude)
{
    this->population = population;
    this->country_name = countryName;
}

Location::Location()
{
    name_ = "";
    latitude_ = 0.0;
    longitude_ = 0.0;
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
        cout << "i + 1: " << cities[i].Location::get_name() << endl;
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

    return attractions;
}

void City::AddAttraction(const Attraction& d)
{
    AttractionsToBeVisited.push_back(d);
}

double APIManager::get_latitude(const string& place_name)
{
    double latitude = 0;
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
            latitude = response["features"][0]["center"][1];
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
     return latitude;
}

string Location::get_name() const
{
    return name_;
}

double APIManager::get_longitude(const string& place_name)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    double longitude = 0;

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
            longitude = response["features"][0]["center"][0];
        } else {
            // Error occurred during the request
            cerr << "Request failed: " << curl_easy_strerror(res) << endl;
        }

        // Clean up libcurl and resources
        curl_easy_cleanup(curl);
    }

    // Clean up libcurl global state
    curl_global_cleanup();

    return longitude;

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

string City::get_country_name() const
{
    return country_name;
}

double City::get_population() const
{
    return population;
}


double Location::get_latitude() const
{
    return latitude_;
}

double Location::get_longitude() const
{
    return longitude_;
}

ItineraryPlanner::ItineraryPlanner(City sourceCity, vector<City> destinationCities)
{
    this->sourceCity = sourceCity;
    this->destinationCities = destinationCities;
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
        distanceFromSourceCity.push_back(calculateDistance(sourceCity.Location::get_latitude(), sourceCity.Location::get_longitude(), destinationCities[i].Location::get_latitude(), destinationCities[i].Location::get_longitude()));
    }
    vector<City> SortedCities = SortByDistance(sourceCity, distanceFromSourceCity, destinationCities);
    return SortedCities;
}

vector<Attraction> Sorter::SortByOrderOfTravel(vector<Attraction> AttractionsToBeVisited)
{
    Attraction firstVisit = AttractionsToBeVisited[0];
    AttractionsToBeVisited.erase(AttractionsToBeVisited.begin());
    vector<double> distanceFromFirstVisit;
    for (int i = 0; i < AttractionsToBeVisited.size(); i++)
    {
        double distance = calculateDistance(firstVisit.Location::get_latitude(), firstVisit.Location::get_longitude(), AttractionsToBeVisited[i].Location::get_latitude(), AttractionsToBeVisited[i].Location::get_longitude());
        distanceFromFirstVisit.push_back(distance);
    }
    vector<Attraction> OrderedAttractions = SortByDistance(firstVisit, distanceFromFirstVisit, AttractionsToBeVisited);
    OrderedAttractions.insert(OrderedAttractions.begin(), firstVisit);
    return OrderedAttractions;
}

Itinerary ItineraryPlanner::PlanIntraCountry(City sourceCity, vector<City> destinationCities)
{
    APIManager Retriever;
    Itinerary itinerary;

    int travellinghoursperday = TRAVELLING_HOURS_PER_DAY;
    int traveltimeperday = travellinghoursperday * 60;
    Sorter sorter;
    vector<City> OrderedCities = sorter.SortByOrderOfTravel(sourceCity, destinationCities);
    vector<Attraction> OrderedAttractions;
    for (int i = 0; i < OrderedCities.size(); i++)
    {
        vector<Attraction> atts = sorter.SortByOrderOfTravel(OrderedCities[i].get_attractions_to_be_visited());
        OrderedAttractions.insert(OrderedAttractions.end(), atts.begin(), atts.end());
    }

    vector<int> travelMinutes;

    for (int i = 0; i < OrderedAttractions.size() - 1; i++)
    {
        travelMinutes.push_back(Retriever.get_travelling_time(OrderedAttractions[i].Location::get_latitude(), OrderedAttractions[i].Location::get_longitude(), OrderedAttractions[i+1].Location::get_latitude(), OrderedAttractions[i+1].Location::get_longitude()));
    }

    int ttlMinutes = 0;
    for (int i = 0; i < travelMinutes.size(); i++)
    {
        ttlMinutes += travelMinutes[i];
    }

    int ttlDays = ttlMinutes / traveltimeperday;

    int depHrs = 8;
    int depMin = 0;
    int Time = Retriever.get_travelling_time(sourceCity.Location::get_latitude(), sourceCity.Location::get_longitude(), OrderedAttractions[0].Location::get_latitude(), OrderedAttractions[0].Location::get_longitude());
    int arrHrs = depHrs + (Time / 60);
    int arrMin = depMin + (Time % 60);

    int travelledTime = Time;
    int remMins = traveltimeperday - Time;

    Action* breakfastAction = new Breakfast(BREAKFAST_TIME, 0);
    Action* lunchAction = new Lunch(LUNCH_TIME, 0);
    Action* dinnerAction = new Dinner(DINNER_TIME, 0);

    TravelDay travelday;
    Action* travel = new Travel(sourceCity.Location::get_name(), OrderedAttractions[0].Location::get_name(), depHrs, depMin, arrHrs, arrMin);
    travelday.AddAction(travel);

    for (int i = 0; i < ttlDays; i++)
    {
        travelday.AddAction(breakfastAction);
        travelday.AddAction(lunchAction);
        travelday.AddAction(dinnerAction);

        for (int j = 0; j < OrderedAttractions.size() - 1; j++)
        {
            Time = Retriever.get_travelling_time(OrderedAttractions[j].Location::get_latitude(), OrderedAttractions[j].Location::get_longitude(), OrderedAttractions[j+1].Location::get_latitude(), OrderedAttractions[j+1].Location::get_longitude());
            if (remMins < 40)
            {
                depHrs = arrHrs + TIMESPENT;
                depMin = arrMin;
                arrHrs = depHrs + (Time / 60);
                arrMin = depMin + (Time % 60);
                travel = new Travel(OrderedAttractions[j].Location::get_name(), OrderedAttractions[j+1].Location::get_name(), depHrs, depMin, arrHrs, arrMin);
                travelday.AddAction(travel);
                travelledTime += Time;
                remMins = traveltimeperday - travelledTime;
            }
            else
            {
                break;
            }
        }
        itinerary.AddTravelDay(travelday);
    }
    return itinerary;
}

vector<Attraction> City::get_attractions_to_be_visited() const
{
    return AttractionsToBeVisited;
}

int APIManager::get_travelling_time(double sourceLat, double sourceLng, double destLat, double destLng)
{
    std::string baseUrl = "https://api.mapbox.com/directions/v5/mapbox/driving/";
    std::string accessToken = "pk.eyJ1Ijoic2FhZGFzaHJhZiIsImEiOiJjbGhuczR6Y20xcDZwM2VudWh0M3hwOHg1In0.XjIfoHbX2IiyiyJsbw1euw"; // Replace with your Mapbox access token

    std::string url = baseUrl + std::to_string(sourceLng) + "," + std::to_string(sourceLat) + ";" + std::to_string(destLng) + "," + std::to_string(destLat) + "?access_token=" + accessToken;

    std::string response;
    CURL* curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            std::cerr << "Failed to perform request: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    // Parse the JSON response
    json j = json::parse(response);

    // Extract the travel time in minutes
    int travelTime = j["routes"][0]["duration"].get<int>() / 60;

    return travelTime;
}
Attraction::Attraction(const Attraction& other) : Location(other.Location::get_name(), other.Location::get_latitude(), other.Location::get_longitude()), avgspend(other.avgspend)
    {

    };

double Attraction::get_avgspend()
{
    return avgspend;
}

Attraction::Attraction(string name, double latitude, double longitude, double avgspend) : Location(name, latitude, longitude)
{
    this->avgspend = avgspend;
}

void Itinerary::DisplayItinerary()
{
    for (int i = 0; i < TravelDays.size(); i++)
    {
        TravelDays[i].DisplayDay();
    }
}


void TravelDay::AddAction(Action* a)
{
    Actions.push_back(a);
}

void TravelDay::DisplayDay()
{
    for (int i = 0; i < Actions.size(); i++)
    {
        Actions[i]->PrintAction();
    }
}



Travel::Travel(string o, string d, int dt_hrs, int dt_min = 0,  int at_hrs = 0, int at_min = 0)
{
    origin = o;
    destination = d;
    dep_time.hours = dt_hrs;
    dep_time.minutes = dt_min;
    arr_time.hours = at_hrs;
    arr_time.minutes = at_min;
}

void Travel::PrintAction()
{
    cout << "Departure from " << origin << " at " << dep_time.hours << ":";
    if (dep_time.minutes < 10)
    {
        cout << "0" << dep_time.minutes << endl;
    }
    else
    {
        cout << dep_time.minutes << endl;
    }
    cout << "Arrival at " << destination << " at " << arr_time.hours << ":";
    if (arr_time.minutes < 10)
    {
        cout << "0" << arr_time.minutes << endl;
    }
    else
    {
        cout << arr_time.minutes << endl;
    }
    
}

City::City() : Location(" ", 0, 0)
{
    population = 0;
}


TravelDay::TravelDay()
{
}

City::City(const City& other) : Location(other.Location::get_name(), other.get_latitude(), other.Location::get_longitude()), country_name(other.country_name), population(other.population)
{
    for (int i = 0; i < other.get_attractions_to_be_visited().size(); i++)
    {
        AttractionsToBeVisited.push_back(Attraction(other.get_attractions_to_be_visited()[i].Location::get_name(), other.get_attractions_to_be_visited()[i].get_latitude(), other.get_attractions_to_be_visited()[i].Location::get_longitude(), other.get_attractions_to_be_visited()[i].get_avgspend()));
    }
}


int Eat::GetHours() const
{
    return Time.hours;
}

int Eat::GetMinutes() const
{
    return Time.minutes;
}



Action::Action()
{

}

Eat::Eat(int hours, int minutes = 0)
{
    Time.hours = hours;
    Time.minutes = minutes;
}

Eat::Eat()
{
    Time.hours = 0;
    Time.minutes = 0;
}

void Eat::SetTime(int hours, int minutes)
{
    Time.hours = hours;
    Time.minutes = minutes;
}

Breakfast::Breakfast(int hours, int minutes = 0) : Eat(hours, minutes)
{

}


Breakfast::Breakfast() : Eat()
{

}

Breakfast::Breakfast(const Breakfast &other) : Eat()
{
    Eat::SetTime(other.Eat::GetHours(), other.Eat::GetMinutes());
}

Lunch::Lunch(const Lunch &other) : Eat()
{
    Eat::SetTime(other.Eat::GetHours(), other.Eat::GetMinutes());
}

Dinner::Dinner(const Dinner &other) : Eat()
{
    Eat::SetTime(other.Eat::GetHours(), other.Eat::GetMinutes());
}

Lunch::Lunch(int hours, int minutes = 0) : Eat(hours, minutes)
{

}

Lunch::Lunch() : Eat()
{

}

Dinner::Dinner(int hours, int minutes = 0) : Eat(hours, minutes)
{

}

Dinner::Dinner()
{

}

void Breakfast::PrintAction()
{
    cout << "Have breakfast at ";
    Eat::GetTime();
}

void Lunch::PrintAction()
{
    cout << "Have lunch at ";
    Eat::GetTime();
}

void Dinner::PrintAction()
{
    cout << "Have dinner at ";
    Eat::GetTime();
}

void Eat::GetTime()
{
    cout << Time.hours << ":";
    if (Time.minutes < 10)
    {
        cout << "0" << Time.minutes << endl;
    }
    else
    {
        cout << Time.minutes << endl;
    }
}