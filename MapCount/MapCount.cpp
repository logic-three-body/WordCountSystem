#include <map>   
#include <fstream>   
#include <iostream>   
#include <string>   

using namespace std;

void display_map(map<string, int> &wmap);

int main()
{
	const char *szInputFileName = "data.txt";
	ifstream ifs(szInputFileName);
	string szTemp;
	map<string, int> wmap;

	while (ifs >> szTemp)
		wmap[szTemp]++;

	display_map(wmap);

	return false;
}

void display_map(map<string, int> &wmap)
{
	map<string, int>::const_iterator map_it;
	for (map_it = wmap.begin(); map_it != wmap.end(); map_it++)
	{
		cout << "map_it->first: "<<map_it->first<<"\t"<<"map_it->second" << map_it->second <<  endl;
	}
}