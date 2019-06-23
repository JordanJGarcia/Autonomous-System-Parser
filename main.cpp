/*

NAME: Jordan Garcia
DATE: 09/19/2019

DESCRIPTION:

This program goes through AS data and aquires all the unique numbers in the data.
It then calculates the amount of neighbors each AS number has and stores it as 
member data in a custom class representing each number and its neighbors. 
After that it prints the data out in an organized format.

To compile and run the program type in the command line:

make
./proj_1 < TESTFILE 		( where TESTFILE is the file you are using to test the program )

*/

	
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "sysData.h"
#include "exists.h"  
#include <ctime>
#include <map>
using namespace std;


// global variables
// vector<sysData> systems;


string                                      removeAssets    (string & path);
vector<int>                                 splitPath       (const string & path);
void                                        displayError    ();
void                                        addSystems      (const vector<sysData> & add, vector<sysData> & original, vector<map<int, sysData>::iterator> & it);
void                                        printSystems    (multimap<int, sysData , greater<int> > & sortedMap, vector<sysData> & systemsVec);
multimap<int, sysData, greater<int>>        sortSystems     (map< int, sysData > & sysMap, vector<sysData> & systems);


/************************/
/*      begin main      */
/************************/
int main()
{
	string line, as_path;
	int field, start, stop;
	
    // stores AS numbers of current line
    vector<int> as_numbers;

    // stores all systems we encounter throughout file
    vector<sysData> systems;

    // stores AS numbers to add to above vector
	vector<sysData> path;
	
    sysData s1(-1);	
	
    clock_t begin = clock(); // used to measure running time of program.

	map<int, sysData> systemsMap;
	
	while(getline(cin, line))	// while there is still data
	{
		field = 0;
		as_path = "";  // reset as_number

		for(int i = 0; i < line.size(); i++)
        {
            // separate by fields
			if(line.at(i) == '|')	
			{
				field++;

				if(field == 6)
					start = i+1;
			
				if(field == 7)
                {
					stop = i;
                    break;
                }
			}

		}

        // get 7th field only and remove duplicates
		as_path = line.substr(start, stop-start);
		as_path = move(removeAssets(as_path));
		
		// sort as_path to get new as_numbers
		as_numbers = move(splitPath(as_path));		

        // vector of iterators through our map for efficient lookup
		vector<map<int, sysData>::iterator> locations;

		for(int i = 0; i < as_numbers.size(); i++)
		{
			// map<int, sysData>::iterator it;
			auto it = systemsMap.find(as_numbers[i]);
						
			path.push_back(as_numbers[i]);
			
			// if the number has not been encountered
            if(it == systemsMap.end())
			{
                // create a new sysData object and add it to map
				sysData sd(as_numbers[i], systems.size());
				systemsMap[as_numbers[i]] = sd;
				systems.push_back(sd);
				
                // get iterator to new number to pass to addSystems()
                // much faster search with map
				it = systemsMap.find(as_numbers[i]);
				locations.push_back(it);
			}
			else
			   locations.push_back(it);
		}

		addSystems(path, systems, locations);
		
		path.clear();		
		locations.clear();
	}

	clock_t end = clock(); // measures running time of program

    auto sortingMap = sortSystems(systemsMap, systems);
    printSystems(sortingMap, systems);

	// if you want to see how long program takes to run uncomment this.
	cout << "Running time: " << double(end-begin) / CLOCKS_PER_SEC << " seconds." << endl;
	return 0;
}

// function to remove ASSETS
string removeAssets(string & path)
{
	string refinedPath = "";
	int index = 0;

	for(int i = 0; i < path.size(); i++)
	{
		if(path.at(i) == '[')
			break;
		
		refinedPath += path.at(i);
	}
	return refinedPath;
}

// place unique and new  numbers into a vector
vector<int> splitPath(const string & path)
{
	istringstream stringToSplit(path);
	vector<int> resulting_as_numbers;

	for(string str; getline(stringToSplit, str, ' '); (!exists(stoi(str), resulting_as_numbers)) ? resulting_as_numbers.push_back(stoi(str)) : displayError());

	return resulting_as_numbers;
}

//used in splitPath function
void displayError()
{
	//cout << "already exists." << endl;
}


// adds new systems to vector of sysData objects
void addSystems(const vector<sysData> & add, vector<sysData> & original, vector<map<int, sysData>::iterator> & it)
{
	auto spot = it.size();		

	for(int i = 0; i < add.size(); i++)
	{
		auto mainSystem = add[i];

		auto location = it.size() - (spot--);
		auto sysLocation = it[location]->second.getLocation();		

		if(i == 0)
		{
			const auto & system = add[1].getSystem();

			original[sysLocation].addNeighbor(system);
		}
		else if(i == add.size()-1)
		{
			const auto & system1 = add[i-1].getSystem();

			original[sysLocation].addNeighbor(system1);
		}
		else
		{
			const auto & system1 = add[i-1].getSystem();
			const auto & system2 = add[i+1].getSystem();

			original[sysLocation].addNeighbor(system1);
			original[sysLocation].addNeighbor(system2);
		}
	}	
}


multimap<int, sysData, greater<int>> sortSystems(map< int, sysData > & sysMap, vector<sysData> & systems)
{
	multimap<int, sysData, greater<int> > sortingMap;
    auto it = sysMap.begin();

    while(it != sysMap.end())
    {
        const auto & loc = it->second.getLocation();
        sortingMap.insert(make_pair(systems[loc].getNumNeighbors(), it->second));
        it++;
    }

    return sortingMap; 
}


void printSystems(multimap<int, sysData, greater<int>> & sortedMap, vector<sysData> & systemsVec)
{
    for(const auto & pair : sortedMap)
    {
        const auto & loc = pair.second.getLocation();
        systemsVec[loc].printData();
    }
}
