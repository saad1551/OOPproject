#ifndef CLASSESH
#define CLASSESH

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <curl/curl.h>
//#include <structures.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

struct t
{
    int hours;
    int minutes;
};

class Location {
public:
    Location();
    Location(string name, double latitude, double longitude);
    void set_description(string description);
    void SetName(const string& name);
    void SetLatitude(double latitude);
    void SetLongitude(double longitude);

    string get_name() const;
    double get_latitude() const;
    double get_longitude() const;

private:
    string name_;
    double latitude_;
    double longitude_;
};

class Country: public Location
{

};

class Attraction : public Location {
public:
    Attraction(string name, double latitude, double longitude, double avgspend);
    Attraction(const Attraction& other);
    double get_avgspend();

private:
    double avgspend;
};

class City : public Location {
    friend class UIManager;

public:
    City();
    City(string name, double latitude, double longitude, double population, string countryName);
    City(const City& other);

    double get_population() const;
    void SetPopulation(int population);
    void AddAttraction(const Attraction& attraction);
    string get_country_name() const;
    vector<Attraction> get_attractions_to_be_visited() const;

private:
    string country_name;
    double population;
    vector<Attraction> AttractionsToBeVisited;
};

class Action
{
public:
    virtual void PrintAction() = 0;
    Action();
};

class Eat: public Action
{
private:
    struct t Time;
public:
    Eat(int hours, int minutes);
    virtual void PrintAction() = 0;
    void GetTime();
    int GetHours();
    int GetMinutes();
};

class Breakfast : public Eat
{
public:
    Breakfast(int, int);
    Breakfast(const Breakfast &other);
    void PrintAction();
};

class Lunch : public Eat
{
public:
    Lunch(int, int);
    Lunch(const Lunch& other);
    void PrintAction();
};

class Dinner: public Eat
{
public:
    Dinner(int, int);
    Dinner();
    void PrintAction();
};


class TravelDay
{
private:
    Breakfast breakfast;
    Lunch lunch;
    Dinner dinner;
public:
    void AddBreakFast(Breakfast& b);
    void AddLunch(Lunch& l);
    void AddDinner(Dinner& d);
    TravelDay();
};

class Itinerary {
    friend class ItineraryPlanner;
public:
    void AddTravelDay(TravelDay travelDay);
    void add_location(const Location& location);
    void remove_location(int index);
    double get_total_distance() const;
    double get_total_travel_time() const;
    double get_total_cost() const;

private:
    vector<TravelDay> TravelDays;
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
    vector <string> getAttractionsByCity(const string& cityname);
    double get_latitude(const string& place_name);
    double get_longitude(const string& place_name);
    City get_city(string const& cityname);
    string get_location_info(const Location& location) const;
    json get_directions(const Location& start, const vector<Location>& Attractions) const;
    string get_weather(const Location& location, const tm& date) const;
};

class UIManager {
public:
    int show_list(vector<City>);
    int show_list(vector<string>);
    int show_cities_list;
    void ShowLocationDetails(City C);
    void ShowLocationDetails(Attraction A);
    Location get_location_input() const;
    tm get_date_input() const;
    double get_budget_input() const;
    void display_itinerary(const Itinerary& itinerary) const;
};


class ItineraryPlanner
{
public:
    ItineraryPlanner(City sourceCity, vector<City> destinationCities, int travelDays);
    Itinerary PlanInterCountry(City sourceCity, vector<City> destinationCities, int travelDays);
    Itinerary PlanIntraCountry(City sourceCity, vector<City> destinationCities, int travelDays);
private:
    City sourceCity;
    vector<City> destinationCities;
    int travelDays;
};

class Sorter
{
public:
    vector<City> SortByOrderOfTravel(City, vector<City>);
    vector<Attraction> Sorter::SortByOrderOfTravel(vector<Attraction>);
};

class Action
{
public:
    virtual void PrintAction() = 0;
};

class Eat: public Action
{
private:
    struct t Time;
public:
    Eat(int, int);
    Eat();
    virtual void PrintAction() = 0;
    void GetTime();
    int GetHours();
};

class Breakfast : public Eat
{
public:
    Breakfast(int, int);
    void PrintAction();
    Breakfast();
};

class Lunch : public Eat
{
public:
    Lunch(int, int);
    void PrintAction();
    Lunch();
};

class Dinner: public Eat
{
public:
    Dinner(int, int);
    void PrintAction();
    Dinner();
};






#endif
