#ifndef CLASSESH
#define CLASSESH

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;



class Location {
public:
    Location(string name, double latitude, double longitude);
    void set_description(string description);
    //void set_image_url(string image_url);
    //void set_wiki_summary(string wiki_summary);

    string get_name() const;
    double get_latitude() const;
    double get_longitude() const;
    //string get_description() const;
    //string get_image_url() const;
    //string get_wiki_summary() const;

private:
    string name_;
    double latitude_;
    double longitude_;
    //string description_;
    //string image_url_;
    //string wiki_summary_;
};

class Destination: public Location
{
public: 
    Destination(string name, double latitude, double longitude, double avgspend);
    double get_avgspend();
private:
    double avgspend;
};

class City: public Location
{
    friend class UIManager;
public:
    City(string name, double latitude, double longitude, double population); 
    double get_population();
    void AddDestination(Destination); 
private:
    double population;
    vector<Destination> DestinationsToBeVisited;
};


class Itinerary {
public:
    void add_location(const Location& location);
    void remove_location(int index);
    double get_total_distance() const;
    double get_total_travel_time() const;
    double get_total_cost() const;

private:
    vector<Location> locations_;
    tm start_date_;
    tm end_date_;
    double budget_;
};

class APIManager {
public:
    vector<string> get_countries_list();
    vector<City> getCitiesByCountry(const string& country);
    string getCountryCode(const string& countryName);
    vector <string> getDestinationsByCity(const string& cityname);
    double get_latitude(const string& place_name);
    double get_longitude(const string& place_name);
    string get_location_info(const Location& location) const;
    json get_directions(const Location& start, const vector<Location>& destinations) const;
    string get_weather(const Location& location, const tm& date) const;
};

class UIManager {
public:
    int show_list(vector<City>);
    int show_list(vector<string>);
    int show_cities_list;
    void ShowCityDetails(City C);
    Location get_location_input() const;
    tm get_date_input() const;
    double get_budget_input() const;
    void display_itinerary(const Itinerary& itinerary) const;
};

#endif
