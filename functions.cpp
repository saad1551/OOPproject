#include <functions.h>
#include <classes.h>

using namespace std;


double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    double earthRadius = 6371.0; // Radius of the Earth in kilometers

    // Convert latitude and longitude from degrees to radians
    double dLat = (lat2 - lat1) * 3.141592653589793 / 180.0;
    double dLon = (lon2 - lon1) * 3.141592653589793 / 180.0;

    // Apply Haversine formula
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1 * 3.141592653589793 / 180.0) * cos(lat2 * 3.141592653589793 / 180.0) *
               sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = earthRadius * c;

    return distance;
}

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

vector<Attraction> SelectAttractions(vector<City> citiesToVisit)
{
    UIManager Displayer;
    APIManager Retriever;
    vector <string> Attractions;
    vector <Attraction> AttractionsToVisit;
    for (int i = 0; i < citiesToVisit.size(); i++)
    {
        Attractions = Retriever.getAttractionsByCity(citiesToVisit[i].get_name());
        vector<int> dest_choices;
        cout << "Choose the desinations you want to visit in " << citiesToVisit[i].get_name() << ". Press " << Attractions.size() + 1 << " to stop.\n";
        int choice;
        while(1)
        {
            Displayer.show_list(Attractions);
            cin >> choice;
            if (choice <= 0 || choice > Attractions.size() + 1)
            {
                cout << "Invalid Choice. Try again\n";
            }
            else if (choice == Attractions.size() + 1)
            {
                break;
            }
            dest_choices.push_back(choice-1);
        }

        for (int j = 0; j < dest_choices.size(); j++)
        {
            double latitude = Retriever.get_latitude(Attractions[dest_choices[j]]);
            double longitude = Retriever.get_longitude(Attractions[dest_choices[j]]);
            double avgspend;
            cout << "Enter the average amount of money expected to be spent at " << Attractions[dest_choices[j]] << ": ";
            cin >> avgspend;
            Attraction D (Attractions[dest_choices[j]], latitude, longitude, avgspend);
            citiesToVisit[i].AddAttraction(D);
            AttractionsToVisit.push_back(D);
        }
    }
    return AttractionsToVisit;
}

City FindCurrentCity(void)
{
    APIManager Retriever;
    string currentCityName;
    cout << "Enter the name of the city you are currently in: ";
    getline(cin, currentCityName);
    City currentCity = Retriever.get_city(currentCityName);
    return currentCity;
}

int GetTravelDays()
{
    int travelDays;
    while(1)
    {
        cout << "How many Days do you want to travel ? ";
        cin >> travelDays;
        if (travelDays <= 0)
        {
            cout << "Invalid Input";
            continue;
        }
        else 
        {
            break;
        }
    }
    return travelDays;
}

int GetTravellingHours(int TravelDays)
{
    int travelHours;
    int flag =  1;
    while(flag)
    {
        cout << "How many hours per day do you want to spend in commuting ? ";
        cin >> travelHours;
        if (travelHours < 3)
        {
            char c;
            while (c != 'y' && c != 'n')
            {
                cout << travelHours << " hours are too less. Do you want to change it? (y/n)";
                cin >> c;
                if (toupper(c) == 'Y')
                {
                    break;
                }
                else if (toupper(c) == 'N')
                {
                    flag = 0;
                    break;
                }
            }
            continue;
        }
        else if (travelHours > 14)
        {
            char c;
            while (c != 'y' && c != 'n')
            {
                cout << travelHours << " hours are too much. Do you want to change it? (y/n)";
                cin >> c;
                if (toupper(c) == 'Y')
                {
                    break;
                }
                else if (toupper(c) == 'N')
                {
                    flag = 0;
                    break;
                }
            }
            continue;
        }
        else 
        {
            break;
        }
    }
    return travelHours;
}

vector<City> SortByDistance(City sourceCity, vector<double> distanceFromSourceCity, vector<City> destinationCities)
{
    if (destinationCities.size() == 1) {
        return destinationCities;
    } else {
        double min = distanceFromSourceCity[0];
        int minIndex = 0;
        for (int i = 0; i < destinationCities.size(); i++) {
            if (distanceFromSourceCity[i] < min) {
                min = distanceFromSourceCity[i];
                minIndex = i;
            }
        }
        
        // Swap the city and distance with the minimum distance
        swap(destinationCities[0], destinationCities[minIndex]);
        swap(distanceFromSourceCity[0], distanceFromSourceCity[minIndex]);

        // Clear the distances and calculate new distances from the updated source city
        distanceFromSourceCity.clear();
        sourceCity = destinationCities[0];
        for (int j = 1; j < destinationCities.size(); j++) {
            distanceFromSourceCity.push_back(calculateDistance(
                sourceCity.get_latitude(), sourceCity.get_longitude(),
                destinationCities[j].get_latitude(), destinationCities[j].get_longitude()));
        }
        
        // Recursive call to sort the remaining cities
        vector<City> sortedCities = SortByDistance(sourceCity, distanceFromSourceCity,
            vector<City>(destinationCities.begin() + 1, destinationCities.end()));

        // Insert the source city at the beginning
        sortedCities.insert(sortedCities.begin(), sourceCity);
        
        return sortedCities;
    }
}

vector<Attraction> SortByDistance(Attraction firstVisit, vector<double> distanceFromFirstVisit, vector<Attraction> AttractionsToBeVisited)
{
    if (AttractionsToBeVisited.size() == 1) {
        return AttractionsToBeVisited;
    } else {
        double min = distanceFromFirstVisit[0];
        int minIndex = 0;
        for (int i = 0; i < AttractionsToBeVisited.size(); i++) {
            if (distanceFromFirstVisit[i] < min) {
                min = distanceFromFirstVisit[i];
                minIndex = i;
            }
        }
        
        // Swap the city and distance with the minimum distance
        swap(AttractionsToBeVisited[0], AttractionsToBeVisited[minIndex]);
        swap(distanceFromFirstVisit[0], distanceFromFirstVisit[minIndex]);

        // Clear the distances and calculate new distances from the updated source city
        distanceFromFirstVisit.clear();
        firstVisit = AttractionsToBeVisited[0];
        for (int j = 1; j < AttractionsToBeVisited.size(); j++) {
            distanceFromFirstVisit.push_back(calculateDistance(
                firstVisit.get_latitude(), firstVisit.get_longitude(),
                AttractionsToBeVisited[j].get_latitude(), AttractionsToBeVisited[j].get_longitude()));
        }
        
        // Recursive call to sort the remaining cities
        vector<Attraction> sortedCities = SortByDistance(firstVisit, distanceFromFirstVisit,
            vector<Attraction>(AttractionsToBeVisited.begin() + 1, AttractionsToBeVisited.end()));

        // Insert the source Attraction at the beginning
        sortedCities.insert(sortedCities.begin(), firstVisit);
        
        return sortedCities;
    }
}



Itinerary CreateItinerary(City sourceCity, vector<City> AttractionsToBeVisited)
{
    Itinerary itinerary;
    ItineraryPlanner PlanCreator(sourceCity, AttractionsToBeVisited);
    if (sourceCity.get_country_name().compare(AttractionsToBeVisited[0].get_country_name()) == 0)
    {
        itinerary = PlanCreator.PlanIntraCountry(sourceCity, AttractionsToBeVisited);
    }
    else
    {
        //itinerary = PlanCreator.PlanInterCountry();
    }
    return itinerary;
}