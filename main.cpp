/**********************************************************************/
/*                                                                    */
/* NAME: Jordan Garcia                                                */
/* DATE: 09/19/2019                                                   */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*                                                                    */
/* This program goes through AS data and aquires all the unique       */
/* numbers in the data. It then calculates the amount of neighbors    */
/* each AS number has and stores it as member data in a custom class  */
/* representing each number and its neighbors. After that it prints   */
/* the data out in an organized format.                               */
/*                                                                    */
/* To compile and run the program type in the command line:           */
/*                                                                    */
/* make                                                               */
/* ./proj_1 < TESTFILE 		                                          */
/*   ( where TESTFILE is the file you are using to test the program ) */   
/**********************************************************************/

	
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include "sysData.h"
#include "exists.h"  
#include <ctime>
#include <map>
using namespace std;

// function declarations
int                                         process_data( void );
vector<int>                                 get_start_stop( string line );
string                                      remove_assets( string & );
vector<int>                                 sort_path( const string & );
void 										put_systems_in_map( vector<int> & as_numbers, map<int, sysData> & systems_map, vector<sysData> & systems );
void                                        add_systems( const vector<sysData> &, vector<sysData> &, vector<map<int, sysData>::iterator> & );
multimap<int, sysData, greater<int>>        sort_systems( map< int, sysData > &, vector<sysData> & );
void                                        print_systems( multimap<int, sysData , greater<int>> &, vector<sysData> & );
void                                        display_error( void );



/* begin main */
int main()
{
    clock_t begin = clock();
	if( process_data() != 0 )
		return EXIT_FAILURE;

    clock_t end = clock(); // measures running time of program

    cout << fixed << showpoint << setprecision(2) << "Running time: " 
         << double(end-begin) / CLOCKS_PER_SEC << " seconds." << endl;

    return EXIT_SUCCESS;
} /* end main */


/* processes the data in the files
  		returns 1 if failure, 0 otherwise
*/	
int process_data( void )
{
	string line, as_path;
    int start, stop;
    vector<int> as_numbers, start_stop;  
    vector<sysData> systems; // stores all systems we encounter throughout file
    map<int, sysData> systems_map;	

    int counter = 0;

	// while there is still data
    while ( getline( cin, line ) )
    {
    	// reset as_path
        as_path = "";

        // collect start/stop points in line. 
        start_stop = get_start_stop( line );
        start = start_stop[0];
        stop = start_stop[1];
    
        // error checking
        if( start_stop.size() < 2 )
        {
            cerr << "Could not collect start/stop. Terminating program." << endl;
            return 1;
        }

        // get entire AS Path from line
        as_path = line.substr(start, stop-start);
        
        // remove duplicates
        as_path = move(remove_assets(as_path));

        // sort as_path to get new as_numbers
        as_numbers = move(sort_path(as_path));		

        // vector of iterators through our map for efficient lookup
        vector<map<int, sysData>::iterator> locations;

        // add new as_numbers to systems_map
     	put_systems_in_map( as_numbers, systems_map, systems );

        counter++;
    }

    // sort & print systems
    auto sorting_map = sort_systems(systems_map, systems);
    print_systems(sorting_map, systems);

    return 0;
}

/* returns a vector with 2 integers
	 vec[0] = start
	 vec[1] = stop
*/
vector<int> get_start_stop( string line )
{
    vector<int> start_stop;
    int line_size = line.size();
    int field;

    for( int i = 0; i < line_size; i++ )
    {
        if( line.at( i ) == '|' )
        {
            field++;

            if( field  == 6 )
                start_stop.push_back( (i + 1) );

            if( field == 7 )
            {
                start_stop.push_back( i );
                //return start_stop;
                break;
            }
        }
    }

    return start_stop;
}

/* removes assets from as_path */
string remove_assets( string & path )
{
    string refined_path = "";
    int index = 0;

    for(int i = 0; i < path.size(); i++)
    {
        if(path.at(i) == '[')
            break;

        refined_path += path.at(i);
    }
    return refined_path;
}

/* place new as_numbers into a vector<int> */
vector<int> sort_path( const string & path )
{
    istringstream stringToSplit(path);
    vector<int> resulting_as_numbers;

    for(string str; 
        getline(stringToSplit, str, ' ');
        (!exists(stoi(str), resulting_as_numbers)) ? resulting_as_numbers.push_back(stoi(str)) : display_error()
       );

    return resulting_as_numbers;
}

/* puts new systems into map for sorting */
void put_systems_in_map( vector<int> & as_numbers, map<int, sysData> & systems_map, vector<sysData> & systems )
{
	// vector of iterators through our map for efficient lookup
    vector<map<int, sysData>::iterator> locations;

    // stores AS numbers to add to above vector
    vector<sysData> path;

    for(int i = 0; i < as_numbers.size(); i++)
    {
        auto it = systems_map.find(as_numbers[i]);
						
        path.push_back(as_numbers[i]);
			
        // if the number has not been encountered
        if(it == systems_map.end())
        {
            // create a new sysData object and add it to map
            sysData sd(as_numbers[i], systems.size());
            systems_map[as_numbers[i]] = sd;
            systems.push_back(sd);
				
            // get iterator to new number to pass to addSystems()
            it = systems_map.find(as_numbers[i]);
            locations.push_back(it);
        }
        else
            locations.push_back(it);
    }

    add_systems(path, systems, locations);
		
    path.clear();		
    locations.clear();
}

/* adds new systems to vector of sysData objects */
void add_systems( const vector<sysData> & add, vector<sysData> & original, vector<map<int, sysData>::iterator> & it )
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

/* sorts systems so they are in correct order to print to console */
multimap<int, sysData, greater<int>> sort_systems( map< int, sysData > &sys_map, vector<sysData> & systems )
{
    multimap<int, sysData, greater<int> > sorting_map;
    auto it = sys_map.begin();

    while(it != sys_map.end())
    {
        const auto & loc = it->second.getLocation();
        sorting_map.insert(make_pair(systems[loc].getNumNeighbors(), it->second));
        it++;
    }

    return sorting_map; 
}

/* prints systems and their data to console */
void print_systems( multimap<int, sysData, greater<int>> & sorted_map, vector<sysData> & systems_vec )
{
    for(const auto & pair : sorted_map)
    {
        const auto & loc = pair.second.getLocation();
        systems_vec[loc].printData();
    }
}

/* used in sort_path() */
void display_error( void )
{
    //cout << "already exists." << endl;
}
