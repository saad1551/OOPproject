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