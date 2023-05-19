#include <classes.h>
#include <functions.h>
#include <iostream>
#include <string>

using namespace std;

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    ((std::string*)userdata)->append(ptr, size * nmemb);
    return size * nmemb;
}

vector<City> SelectCities(vector<City> Cities)
{
    UIManager Displayer;
    cout << "Select the cities you want to visit. To stop, press " << Cities.size() + 1;
    Displayer.show_list(Cities);
    vector<City> citychoices;
    int choice;
    while(1)
    {
        cin >> choice;
        if (choice <= 0 || choice > Cities.size() + 1)
        {
            cout << "Invalid Choice";
            continue;
        }
        else if (choice == Cities.size() + 1)
        {
            char a;
            while (toupper(a) != 'Y' && toupper(a) != 'N')
            {
                cout << "Are you sure you wish to stop selecting cities ? (y/n) ";
                cin >> a;
            }
            if (toupper(a) == 'Y')
            {
                break;
            }
        }
        citychoices.push_back(Cities[choice-1]);
    }
    return citychoices;
}

string SelectCountry(vector<string> Countries)
{
    UIManager Displayer;
    cout << "Select the country you want to visit: ";
    Displayer.show_list(Countries);
    string countrychoice;
    int choice;
    while(1)
    {
        cin >> choice;
        if (choice <= 0 || choice > Countries.size())
        {
            cout << "Invalid Choice";
            continue;
        }
        else
        {
            break;
        }
    }
    countrychoice = Countries[choice-1];
    return countrychoice;
}

vector<Destination> SelectDestinations(vector<City> citiesToVisit)
{
    UIManager Displayer;
    APIManager Retriever;
    vector <string> destinations;
    vector <Destination> destinationsToVisit;
    for (int i = 0; i < citiesToVisit.size(); i++)
    {
        destinations = Retriever.getDestinationsByCity(citiesToVisit[i].get_name());
        vector<int> dest_choices;
        cout << "Choose the desinations you want to visit in " << citiesToVisit[i].get_name() << ". Press " << destinations.size() + 1 << " to stop.\n";
        int choice;
        while(1)
        {
            Displayer.show_list(destinations);
            cin >> choice;
            if (choice <= 0 || choice > destinations.size() + 1)
            {
                cout << "Invalid Choice. Try again\n";
            }
            else if (choice == destinations.size() + 1)
            {
                break;
            }
            dest_choices.push_back(choice-1);
        }

        for (int j = 0; j < dest_choices.size(); j++)
        {
            double latitude = Retriever.get_latitude(destinations[dest_choices[j]]);
            double longitude = Retriever.get_longitude(destinations[dest_choices[j]]);
            double avgspend;
            cout << "Enter the average amount of money expected to be spent at " << destinations[j] << ": ";
            cin >> avgspend;
            Destination D (destinations[dest_choices[j]], latitude, longitude, avgspend);
            citiesToVisit[i].AddDestination(D);
            destinationsToVisit.push_back(D);
        }
    }

}