#include <iostream>     // std::cin, std::cout
#include <fstream>      // std::ifstream
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

// struct replacement_t
// {
// 	string start;
// 	string end;
// };

// void substitute(map<string,bool> *table, vector<replacement_t> replacements) {
// 	vector<string> elements;
// 	for (map<string,bool>::iterator element = table->begin(); element != table->end(); ++element) {
// 		elements.push_back(element->first);
// 	}

// 	for (int j = 0; j < elements.size(); j++) {
// 		string element = elements[j];
// 		for (int i = 0; i < replacements.size(); i++) {
// 		  	size_t found = element.find(replacements[i].start);
// 		  	while (found != string::npos) {
// 		  		string newElement = element;
// 		  		newElement.replace(found, replacements[i].start.length(), replacements[i].end);
// 		  		(*table)[newElement] = true;
// 		  		found = element.find(replacements[i].start, found+1);
// 		  	}
// 		 }
// 	}
// }
// int day19part1() {
//   ifstream stream("input19.txt");
//   string line;
//   vector<replacement_t> replacements;
//   map<string,bool> table;
//   string medicine;
//   while (getline(stream, line)) {
//   	if (line == "") break;
//   	stringstream ss(line);
//   	string dummy;
//   	replacement_t r;
//   	ss >> r.start >> dummy >> r.end;
//   	replacements.push_back(r);
//   }
//   stream >> medicine;
//   stream.close();
//   table["e"] = true;
//   int step = 0;
//   while (table.find(medicine) == table.end()) {
//   	substitute(&table, replacements);
//   	step++;
//   	cout<<step<<endl;
//   }
//   return step;
// }
// int main () {
//   cout << day19part1() << endl;
//   return 0;
// }

struct player_t {
	int hp;
	int damage;
	int armor;
	player_t(int h, int d, int a):hp(h),damage(d),armor(a) {}
	player_t(int h, vector<item_t> items):hp(h),damage(0),armor(0) {
		for (int i = 0; i < items.size(); i++) {
			damage += items[i].damage;
			armor += items[i].armor;
		}
	}
};
struct item_t {
	int cost;
	int damage;
	int armor;
    item_t(int c, int d, int a):cost(c),damage(d),armor(a) {}
};


bool isWinner(player_t x, player_t y) { //player x goes first, returns if x wins.
	int xDamagePerTurn = max(1, y.damage - x.armor);
	int yDamagePerTurn = max(1, x.damage - y.armor);
	if (x.hp / xDamagePerTurn >= y.hp / yDamagePerTurn) return true;
	return false;
}

int day21part1() {
	player_t opponent(109,8,2);

	item_t weapons[] = {item_t(8,4,0), item_t(10,5,0), item_t(25,6,0), item_t(40,7,0), item_t(74,8,0)};
	item_t armor[] = {item_t(13,0,1), item_t(31,0,2), item_t(53,0,3), item_t(75,0,4), item_t(102,0,5)};
	item_t rings[] = {item_t(25,1,0), item_t(50,2,0), item_t(100,3,0), item_t(20,0,1), item_t(40,0,2), item_t(80,0,3)};

	vector<item_t> items1;
	items1.push_back(weapons[0])
	vector<item_t> items2;
	player_t x(100,items1), y(100,items2);

	cout << isWinner(x,y) << endl;
	return 0;
}

int main () {
  cout << day21part1() << endl;
  return 0;
}