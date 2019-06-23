/*

NAME: Jordan Garcia
DATE: 09/19/2018

DESCRIPTION:

This is the header file to my custom class representing AS numbers and their neighbors.
Each AS number is represented by a sysData object and that object is used to store its
recorded neighbors as well as printing out the data for each number in an organized
format. 

*/



#ifndef SYSDATA_H
#define SYSDATA_H

#include <vector>
#include <map>
using namespace std;

class sysData
{
	friend bool operator==(const sysData& s1, const sysData& s2);
	friend bool operator<(const sysData& s1, const sysData& s2);
	friend ostream& operator<<(ostream& s, const sysData& sd);

	public:
		sysData();
		sysData(int sys);
		sysData(int sys, int loc);  
		sysData(const sysData & sd);
		void addNeighbor(const int & n);
		int getSystem() const; 
		void setSystem(const int & sys);	
		int getNumNeighbors() const;
		map<int, int> getNeighbors() const;
		void printData() const;		
		int getLocation() const;
		void setLocation(const int & loc);
		sysData& operator=(const sysData& sd);
	private:
		int systemNumber; 	//stores which AS we are collecting data for
		int location;  		//stores location in vector ( systems vector in main ).
		map<int, int> neighborMap; 	//stores neighbors as keys and sysNumber as value
};					


#endif
