#include <iostream>     // std::cin, std::cout
#include <fstream>      // std::ifstream
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

struct replacement_t
{
	string start;
	string end;
};

void substitute(map<string,bool> *table, vector<replacement_t> replacements) {
	vector<string> elements;
	for (map<string,bool>::iterator element = table->begin(); element != table->end(); ++element) {
		elements.push_back(element->first);
	}

	for (int j = 0; j < elements.size(); j++) {
		string element = elements[j];
		for (int i = 0; i < replacements.size(); i++) {
		  	size_t found = element.find(replacements[i].start);
		  	while (found != string::npos) {
		  		string newElement = element;
		  		newElement.replace(found, replacements[i].start.length(), replacements[i].end);
		  		(*table)[newElement] = true;
		  		found = element.find(replacements[i].start, found+1);
		  	}
		 }
	}
}
int day19part1() {
  ifstream stream("input19.txt");
  string line;
  vector<replacement_t> replacements;
  map<string,bool> table;
  string medicine;
  while (getline(stream, line)) {
  	if (line == "") break;
  	stringstream ss(line);
  	string dummy;
  	replacement_t r;
  	ss >> r.start >> dummy >> r.end;
  	replacements.push_back(r);
  }
  stream >> medicine;
  stream.close();
  table["e"] = true;
  int step = 0;
  while (table.find(medicine) == table.end()) {
  	substitute(&table, replacements);
  	step++;
  	cout<<step<<endl;
  }
  return step;
}


int main () {
  cout << day19part1() << endl;
  return 0;
}