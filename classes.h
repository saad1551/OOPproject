#ifndef APIHEADER_H
#define APIHEADER_H

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
    Eat();
    virtual void PrintAction() = 0;
    void GetTime();
    int GetHours() const;
    int GetMinutes() const;
    void SetTime(int , int);
};

class Travel : public Action
{
private:
    string origin;
    string destination;
    struct t dep_time;
    struct t arr_time;
public:
    Travel(string, string, int, int, int, int);
    void PrintAction();
};

class Breakfast : public Eat
{
public:
    Breakfast(int, int);
    Breakfast();
    Breakfast(const Breakfast& other);
    void PrintAction();
};

class Lunch : public Eat
{
public:
    Lunch(int, int);
    Lunch(const Lunch& other);
    void PrintAction();
    Lunch();
};

class Dinner: public Eat
{
public:
    Dinner(int, int);
    Dinner();
    Dinner(const Dinner& other);
    void PrintAction();
};


class TravelDay
{
private:
    vector<Action*>Actions;
public:
    void AddAction(Action*);
    void DisplayDay();
    TravelDay();
};

class Itinerary {
    friend class ItineraryPlanner;
public:
    void AddTravelDay(TravelDay travelDay);
    void DisplayItinerary();

private:
    vector<TravelDay> TravelDays;
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
    int get_travelling_time(double, double, double, double);
};

class UIManager {
public:
    int show_list(vector<City>);
    int show_list(vector<string>);
    int show_cities_list;
    void ShowLocationDetails(City C);
    void ShowLocationDetails(Attraction A);
};


class ItineraryPlanner
{
public:
    ItineraryPlanner(City sourceCity, vector<City> destinationCities);
    Itinerary PlanInterCountry(City sourceCity, vector<City> destinationCities, int travelDays);
    Itinerary PlanIntraCountry(City sourceCity, vector<City> destinationCities);
private:
    City sourceCity;
    vector<City> destinationCities;
    int travelDays;
};

class Sorter
{
public:
    vector<City> SortByOrderOfTravel(City, vector<City>);
    vector<Attraction>SortByOrderOfTravel(vector<Attraction>);
};

#endif // APIHEADER_H
