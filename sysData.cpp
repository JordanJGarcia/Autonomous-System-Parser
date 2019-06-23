/*

NAME: Jordan Garcia
DATE: 09/19/2018

DESCRIPTION:

This is the implementation of my header file for sysData objects representing
AS numbers. It simply defines all functions in the header while also providing 
safe access to member data of the object. 

*/



#include <vector>
#include <iostream>
#include "sysData.h"
using namespace std;

// operator overload == 
bool operator==(const sysData& s1, const sysData& s2)
{
	if(s1.getSystem() == s2.getSystem())
		return true;

	return false;	
}


// this operator overload function is somewhat reversed in meaning 
// i did this to create a simpler way of sorting my map by the number of neighbors 
bool operator<(const sysData& s1, const sysData & s2)
{
	return (s1.getNumNeighbors() >= s2.getNumNeighbors());
}

// operator overload << 
ostream& operator<<(ostream& s, const sysData& sd)
{
	s << sd.getSystem();
	return s;
}

/* constructor with no parameters */
sysData::sysData()
{

}

// constructor with systemNumber parameter
sysData::sysData(int sys) : systemNumber{sys}
{

}

// constructor with systemNumber param and vector location param
sysData::sysData(int sys, int loc) : systemNumber{sys}, location{loc}
{

}

// copy constructor
sysData::sysData(const sysData & sd)
{
	systemNumber = sd.systemNumber;
	location = sd.location;
	neighborMap = sd.neighborMap;
}

// function to add a neighbor and increase numNeighbors
void sysData::addNeighbor(const int & n)
{
	map<int, int>::iterator it;
	it = neighborMap.find(n); //search if neighbor already exists ( preventing duplicates )
	if(it == neighborMap.end())
		neighborMap.insert(make_pair(n, getSystem()));
}

// function returning the system this object represents
int sysData::getSystem() const
{
	return systemNumber;
}

// function to set the system when we have to initialize to zero-param constructor
void sysData::setSystem(const int & sys)
{
	systemNumber = sys;
}


// function to get numNeighbors ( or size of neighborMap )
int sysData::getNumNeighbors() const
{
	return neighborMap.size();
}

// function returning map of system's neighbors
map<int, int> sysData::getNeighbors() const
{
	return neighborMap;
}

// prints out system number, number of neighbors, and a list of all neighbors separated by "|" 
void sysData::printData() const
{
	cout << systemNumber << " " << neighborMap.size() << " ";
	map<int, int>::const_iterator it;
	it = neighborMap.begin();
	int index = 0;

	while( it != neighborMap.end() )
	{
		cout << it->first;
		
		if(it == neighborMap.begin() && neighborMap.size() > 1)
			cout << "|";
		else if(index < neighborMap.size() - 1) 
			cout << "|";
		it++;
		index++;
	}
	cout << endl;
}

// set location in the vector in main
void sysData::setLocation(const int & loc)
{
	location = loc;
}

// return location in vector in main 
int sysData::getLocation() const
{
	return location;
}

// operator overload = 
sysData& sysData::operator=(const sysData & sd)
{
	if(this != &sd)
	{
		systemNumber = sd.systemNumber;
		location = sd.location;
		neighborMap = sd.neighborMap;
	}
	return *this;
}
