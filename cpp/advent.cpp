#include <iostream>     // std::cin, std::cout
#include <fstream>      // std::ifstream
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>

#include "md5.h"


using namespace std;

int day1part1 () {
  char f[256] = "input1.txt";
  ifstream stream(f);
  char c;
  int level = 0;
  while (stream.get(c)) {
    if (c == '(') level++;
    if (c == ')') level--;
   }
  stream.close();
  return level;
}

int day1part2 () {
  char f[256] = "input1.txt";
  ifstream stream(f);
  char c;
  int position;
  int level = 0;
  for (position = 1; stream.get(c); position++) {
    if (c == '(') level++;
    if (c == ')') level--;
    if (level == -1) {
      break;
    }
  }
  stream.close();
  return position;
}

int day2part1 () {
  ifstream stream("input2.txt");
  string line;
  int totalPaper = 0;
  while (getline(stream, line)) {
    stringstream sstream(line);
    int a, b, c;
    char x;
    sstream >> a >> x >> b >> x >> c;
    // cout << a << 'x' << b << 'x' << c << endl;
    int front = a*b;
    int top = b*c;
    int side = a*c;
    int sides[] = {front, top, side};
    totalPaper += 2*front + 2*top + 2*side + *min_element(sides, sides+3);
  }
  stream.close();
  return totalPaper;
}

int day2part2 () {
  ifstream stream("input2.txt");
  string line;
  int totalRibbon = 0;
  while (getline(stream, line)) {
    stringstream sstream(line);
    int a, b, c;
    char x;
    sstream >> a >> x >> b >> x >> c;
    int bow = a*b*c;
    int sides[] = {a, b, c};
    int maxSide = *max_element(sides, sides+3);
    int circ = 2*(a+b+c-maxSide);
    totalRibbon += bow + circ;
  }
  stream.close();
  return totalRibbon;
}

int day3part1 () {
  ifstream stream("input3.txt");
  char c;
  int x = 1000, y = 1000;
  bool map[2000][2000];
  for (int i = 0; i < 2000; i++)
    for (int j = 0; j < 2000; j++)
      map[i][j] = false;
  while (stream.get(c)) {
    if (c == '<') x--;
    if (c == '>') x++;
    if (c == '^') y--;
    if (c == 'v') y++;
    map[x][y] = true;
  }
  stream.close();
  int houses = 0;
  for (int i = 0; i < 2000; i++) {
    for (int j = 0; j < 2000; j++) {
      if (map[i][j]) houses++;
    }
  }
  return houses;
}

int day3part2 () {
  bool santaTurn = true;
  int santax = 1000, santay = 1000, robox = 1000, roboy = 1000;
  int *x, *y;
  bool map[2000][2000];
  for (int i = 0; i < 2000; i++)
    for (int j = 0; j < 2000; j++)
      map[i][j] = false;
  ifstream stream("input3.txt");
  char c;
  while (stream.get(c)) {
    if (santaTurn) {
      x = &santax;
      y = &santay;
    }
    else {
      x = &robox;
      y = &roboy;
    }
    if (c == '<') *x = *x-1;
    if (c == '>') *x = *x+1;
    if (c == '^') *y = *y-1;
    if (c == 'v') *y = *y+1;
    map[*x][*y] = true;
    santaTurn = !santaTurn;
  }
  stream.close();
  int houses = 0;
  for (int i = 0; i < 2000; i++) {
    for (int j = 0; j < 2000; j++) {
      if (map[i][j]) houses++;
    }
  }
  return houses;
}

bool isAdventCoin(string s) {
    for (int i = 0; i < 5; i++) {
      if (s[i] != '0') return false;
    }
    return true;
}

string intToString(int n) {
  ostringstream convert;   // stream used for the conversion
  convert << n;
  return convert.str();
}

int day4() {
  string key = "yzbqklnj";
  int n = 0;
  while (true) {
    n++;
    string msg;
    msg = key + intToString(n);
    string hash = md5(msg);
    if (isAdventCoin(hash)) return n;
  }
}

bool has3vowels(const string s) {

  char vowels[] = {'a','e','i','o','u'};
  int numVowels = 0;
  for (int i = 0; i < s.length(); i++) {
    char c = s[i];
    int vlen = sizeof(vowels)/sizeof(vowels[0]);
    for (int j = 0; j < vlen; j++) {
      char v = vowels[j];
      if (c == v) {
        numVowels ++;
        if (numVowels >= 3) {
          return true;
        }
        break;
      }
    }
  }
  return false;
}
bool hasDoubleLetter(const string s) {
  char prev = '.';
  for (int i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c == prev) return true;
    prev = c;
  }
  return false;
}
bool hasNo(vector<string> naughty, const string s) {
  for (int i = 0; i < naughty.size(); i++) {
    string n = naughty[i];
    size_t found = s.find(n);
    if (found != string::npos) return false;
  }
  return true;
}

int day5part1() {
  ifstream stream("input5.txt");
  string line;
  static const string naughtyArray[] = {"ab", "cd", "pq", "xy"};
  vector<string> naughty (naughtyArray, naughtyArray + sizeof(naughtyArray) / sizeof(naughtyArray[0]) );
  int nice = 0;
  while (getline(stream, line)) {
    if (has3vowels(line) && hasDoubleLetter(line) && hasNo(naughty, line))
      nice++;
  }
  stream.close();
  return nice;
}

bool hasSandwich(const string s) {
  for (int i = 0; i < s.length()-2; i++) {
    if (s[i] == s[i+2]) return true;
  }
  return false;
}

bool hasRepeatingPair(const string s) {
  for (int i = 0; i < s.length()-3; i++) {
    for (int j = i+2; j < s.length(); j++) {
      if (s[i] == s[j] && s[i+1] == s[j+1]) return true;
    }
  }
  return false;
}

int day5part2() {
  ifstream stream("input5.txt");
  string line;
  int nice = 0;
  while (getline(stream, line)) {
    if (hasRepeatingPair(line) && hasSandwich(line))
      nice++;
  }
  stream.close();
  return nice;
}

void turnOn(bool display[][1000], int x1, int y1, int x2, int y2) {
  for (int i = x1; i <= x2; i++)
    for (int j = y1; j <= y2; j++)
      display[i][j] = true;
}
void turnOff(bool display[][1000], int x1, int y1, int x2, int y2) {
  for (int i = x1; i <= x2; i++)
    for (int j = y1; j <= y2; j++)
      display[i][j] = false;
}
void toggle(bool display[][1000], int x1, int y1, int x2, int y2) {
  for (int i = x1; i <= x2; i++)
    for (int j = y1; j <= y2; j++)
      display[i][j] = !display[i][j];
}

int day6part1() {
  ifstream stream("input6.txt");
  string line;
  bool display[1000][1000];
  for (int i = 0; i < 1000; i++)
    for (int j = 0; j < 1000; j++)
      display[i][j] = false;
  while (getline(stream, line)) {
    stringstream lineStream(line);
    string command;
    lineStream >> command;
    if (command == "turn") lineStream >> command;
    int x1, x2, y1, y2;
    char c;
    string through;
    lineStream >> x1 >> c >> y1 >> through >> x2 >> c >> y2;
    if (command == "on") turnOn(display, x1, y1, x2, y2);
    else if (command == "off") turnOff(display, x1, y1, x2, y2);
    else toggle(display, x1, y1, x2, y2);
  }
  stream.close();
  int lit = 0;
  for (int i = 0; i < 1000; i++)
    for (int j = 0; j < 1000; j++)
      if (display[i][j]) lit++;
  return lit;
}

void turnOn2(int display[][1000], int x1, int y1, int x2, int y2) {
  for (int i = x1; i <= x2; i++)
    for (int j = y1; j <= y2; j++)
      display[i][j]++;
}
void turnOff2(int display[][1000], int x1, int y1, int x2, int y2) {
  for (int i = x1; i <= x2; i++)
    for (int j = y1; j <= y2; j++)
      display[i][j] = max(0, display[i][j]-1);
}
void toggle2(int display[][1000], int x1, int y1, int x2, int y2) {
  for (int i = x1; i <= x2; i++)
    for (int j = y1; j <= y2; j++)
      display[i][j] += 2;
}

int day6part2() {
  ifstream stream("input6.txt");
  string line;
  int display[1000][1000];
  for (int i = 0; i < 1000; i++)
    for (int j = 0; j < 1000; j++)
      display[i][j] = 0;
  while (getline(stream, line)) {
    stringstream lineStream(line);
    string command;
    lineStream >> command;
    if (command == "turn") lineStream >> command;
    int x1, x2, y1, y2;
    char c;
    string through;
    lineStream >> x1 >> c >> y1 >> through >> x2 >> c >> y2;
    if (command == "on") turnOn2(display, x1, y1, x2, y2);
    else if (command == "off") turnOff2(display, x1, y1, x2, y2);
    else toggle2(display, x1, y1, x2, y2);
  }
  stream.close();
  int totalBrightness = 0;
  for (int i = 0; i < 1000; i++)
    for (int j = 0; j < 1000; j++)
      totalBrightness += display[i][j];
  return totalBrightness;
}

unsigned short calc(string s) {
  stringstream ss(s);
  if (s.find("NOT") != string::npos) {
    unsigned short x;
    string dummy;
    ss >> dummy >> x;
    return ~x;
  }
  unsigned short x, y;
  string dummy;
  ss >> x >> dummy >> y;
  if (s.find("AND") != string::npos) return x & y;
  if (s.find("OR") != string::npos) return x | y;
  if (s.find("LSHIFT") != string::npos) return x << y;
  if (s.find("RSHIFT") != string::npos) return x >> y;
  return atoi(s.c_str());
}

bool containsLower(string s) {
  for (int i = 0; i < s.length(); i++) {
    if (islower(s[i])) return true;
  }
  return false;
}

unsigned short day7part1() {
  ifstream stream("input7.txt");
  string line;
  map<string, string> lines;
  map<string, unsigned short> finalValue;
  while (getline(stream, line)) {
    size_t i = line.find(" ->");
    size_t outIndex = i + 4;
    string out = line.substr(outIndex, line.length()-outIndex);
    string in = line.substr(0, i);
    lines[out] = in;
  }
  stream.close();

  while (finalValue.find("a") == finalValue.end()) {
    unsigned short newVal;
    string wire;

    for(map<string,string>::iterator line = lines.begin(); line != lines.end(); ++line) {
      if (! containsLower(line->second)) {
        newVal = calc(line->second);
        wire = line->first;
        finalValue[line->first] = newVal;
        lines.erase(line); //might need to store temp and line++ and delete temp
        break;
      }
    }

    for(map<string,string>::iterator line = lines.begin(); line != lines.end(); ++line) {
      stringstream ss(line->second);
      string word;
      int i = 0;
      while (ss >> word) {
        if (word == wire) {
          (line->second).replace(i, wire.length(), intToString(newVal));
          break;
        }
        i += word.length()+1;
      }        
    }
  }
  return finalValue["a"];
}

unsigned short day7part2() {
  ifstream stream("input7.txt");
  string line;
  map<string, string> lines;
  map<string, unsigned short> finalValue;
  while (getline(stream, line)) {
    size_t i = line.find(" ->");
    size_t outIndex = i + 4;
    string out = line.substr(outIndex, line.length()-outIndex);
    string in = line.substr(0, i);
    lines[out] = in;
  }
  stream.close();

  while (finalValue.find("a") == finalValue.end()) {
    unsigned short newVal;
    string wire;

    for(map<string,string>::iterator line = lines.begin(); line != lines.end(); ++line) {
      if (! containsLower(line->second)) {
        newVal = calc(line->second);
        wire = line->first;
        if (wire == "b") newVal = day7part1();
        finalValue[line->first] = newVal;
        lines.erase(line); //might need to store temp and line++ and delete temp
        break;
      }
    }

    for(map<string,string>::iterator line = lines.begin(); line != lines.end(); ++line) {
      stringstream ss(line->second);
      string word;
      int i = 0;
      while (ss >> word) {
        if (word == wire) {
          (line->second).replace(i, wire.length(), intToString(newVal));
          break;
        }
        i += word.length()+1;
      }        
    }
  }
  return finalValue["a"];
}

int evaluatedLength(string line) {
  int overhead = 2;
  for (int i = 0; i < line.length(); i++) {
    if (line[i] == '\\') {
      if (line[i+1] == 'x') {
        overhead += 3;
        i+=3;
      }
      else {
        overhead += 1;
        i++;
      }
    }
  }
  return line.length() - overhead;
}
int day8part1 () {
  ifstream stream("input8.txt");
  string line;
  int code = 0;
  int literal = 0;
  while (getline(stream, line)) {
    code += line.length();
    literal += evaluatedLength(line);
  }
  stream.close();
  return code - literal;
}

int encodedLength(string line) {
  int overhead = 2;
  for (int i = 0; i < line.length(); i++)
    if (line[i] == '\\' or line[i] == '\"')
      overhead++;
  return line.length() + overhead;
}

int day8part2 () {
  ifstream stream("input8.txt");
  string line;
  int code = 0;
  int encoded = 0;
  while (getline(stream, line)) {
    code += line.length();
    encoded += encodedLength(line);
  }
  stream.close();
  return encoded - code;
}

int optPath(int source, vector<int> cities, int dist[][100]) {
  if (cities.size() == 1) return dist[source][cities[0]];
  vector<int> pathDists;
  vector<int> citiesCopy = cities;
  citiesCopy.erase(remove(citiesCopy.begin(), citiesCopy.end(), source), citiesCopy.end());

  for (int i = 0; i < citiesCopy.size(); i++) {
    pathDists.push_back(dist[source][citiesCopy[i]] + optPath(citiesCopy[i], citiesCopy, dist));
  }
  return *min_element(pathDists.begin(), pathDists.end());
}

int day9part1 () {
  ifstream stream("input9.txt");
  string line;
  map<string, int> cities;
  int numCities = 0;
  int dist[100][100];
  while (getline(stream, line)) {
    stringstream ss(line);
    string city1, city2, dummy;
    int distance;
    ss >> city1 >> dummy >> city2 >> dummy >> distance;
    if (cities.find(city1) == cities.end()) {
      cities[city1] = numCities;
      numCities++;
    }
    if (cities.find(city2) == cities.end()) {
      cities[city2] = numCities;
      numCities++;
    }
    dist[cities[city1]][cities[city2]] = distance;
    dist[cities[city2]][cities[city1]] = distance;
  }
  for (int k = 0; k < numCities; k++) { //universal source node
    dist[numCities][k] = 0;
    dist[k][numCities] = 0;
  }
  stream.close();
  vector<int> allCities;
  for (int i = 0; i < numCities+1; i++) allCities.push_back(i);

  return optPath(numCities, allCities, dist);
}

int longestPath(int source, vector<int> cities, int dist[][100]) {
  if (cities.size() == 1) return dist[source][cities[0]];
  vector<int> pathDists;
  vector<int> citiesCopy = cities;
  citiesCopy.erase(remove(citiesCopy.begin(), citiesCopy.end(), source), citiesCopy.end());

  for (int i = 0; i < citiesCopy.size(); i++) {
    pathDists.push_back(dist[source][citiesCopy[i]] + longestPath(citiesCopy[i], citiesCopy, dist));
  }
  return *max_element(pathDists.begin(), pathDists.end());
}

int day9part2 () {
  ifstream stream("input9.txt");
  string line;
  map<string, int> cities;
  int numCities = 0;
  int dist[100][100];
  while (getline(stream, line)) {
    stringstream ss(line);
    string city1, city2, dummy;
    int distance;
    ss >> city1 >> dummy >> city2 >> dummy >> distance;
    if (cities.find(city1) == cities.end()) {
      cities[city1] = numCities;
      numCities++;
    }
    if (cities.find(city2) == cities.end()) {
      cities[city2] = numCities;
      numCities++;
    }
    dist[cities[city1]][cities[city2]] = distance;
    dist[cities[city2]][cities[city1]] = distance;
  }
  for (int k = 0; k < numCities; k++) { //universal source node
    dist[numCities][k] = 0;
    dist[k][numCities] = 0;
  }
  stream.close();
  vector<int> allCities;
  for (int i = 0; i < numCities+1; i++) allCities.push_back(i);

  return longestPath(numCities, allCities, dist);
}

string lookAndSay(string s) {
  int c = 0;
  int prev = -1;
  int curr;
  string ans;
  for (int i = 0; i < s.length(); i++) {
    curr = s[i] - 48;
    if (prev == -1) {
      prev = curr;
      c++;
    } 
    else if (prev == curr) {
      c++;
    }
    else {
      ans += intToString(c) + intToString(prev);
      prev = curr;
      c = 1;
    }
  }
  return ans += intToString(c) + intToString(prev);
}

int day10part1() {
  string line = "1113122113";
  for (int i = 0; i < 40; i++) {
    line = lookAndSay(line);
  }
  return line.length();
}

int day10part2() {
  string line = "1113122113";
  for (int i = 0; i < 50; i++) {
    line = lookAndSay(line);
  }
  return line.length();
}

string incrString(string s) {
  int i = s.length() - 1;
  while (i >= 0) {
    if (s[i] == 'z') {
      s[i] = 'a';
      i--;
    }
    else {
      s[i]++;
      return s;
    }
  }
  return "TOO LONG";
}

bool has3Straight(string s) {
  for (int i = 0; i < s.length() - 2; i++) {
    if (s[i] + 1 == s[i+1] && s[i] + 2 == s[i+2]) return true;
  }
  return false;
}

bool has2UniquePairs(string s) {
  char first;
  bool foundFirst = false;
  for (int i = 0; i < s.length() - 1; i++) {
    if (s[i] == s[i+1]) {
      if (foundFirst && s[i] != first) return true;
      foundFirst = true;
      first = s[i];
    }
  }
  return false;
}

string day11part1() {
  string pwd = "vzbxkghb";
  string bad[] = {"i", "o", "l"};
  vector<string> badLetters(bad, bad+3);
  while (! (has2UniquePairs(pwd) && hasNo(badLetters, pwd) && has3Straight(pwd))) 
    pwd = incrString(pwd);
  return pwd;
}

string day11part2() {
  string pwd = incrString("vzbxxyzz");
  string bad[] = {"i", "o", "l"};
  vector<string> badLetters(bad, bad+3);
  while (! (has2UniquePairs(pwd) && hasNo(badLetters, pwd) && has3Straight(pwd))) 
    pwd = incrString(pwd);
  return pwd;
}
int sumInStream(istream &stream) {
  int sum = 0;
  int num = 0;
  string numString;
  char c;
  while (stream >> c) {
    if (c >= '0' && c <= '9') {
      numString += c;
    } else {
      stringstream ss(numString);
      ss >> num;
      sum += num;
      numString = "";
      num = 0;
      if (c == '-') {
        char x;
        stream >> x;
        if (x >= '0' && c <= '9') {
          numString += c;
          numString += x;
        }
      }
    }
  }
  return sum;
}

int day12part1() {
  ifstream stream("input12.txt");
  int sum = sumInStream(stream);
  stream.close();
  return sum;
}

string slideWindow(string w, char c) {
  return w.substr(1) + c;
}

int delayer(istream &stream) {
  int sum = 0;
  bool hasRed = false;
  char c;
  int num = 0;
  string last6 = "123456";
  string numString;
  while(c != '}' && stream >> c) {
    last6 = slideWindow(last6, c);

    if (c >= '0' && c <= '9') {
      numString += c;
    } 
    else {
      stringstream ss(numString);
      ss >> num;
      sum += num;
      numString = "";
      num = 0;
      if (c == '-') {
        char x;
        stream >> x;
        last6 = slideWindow(last6, x);
        if (x >= '0' && c <= '9') {
          numString += c;
          numString += x;
        }
      }
      else if (c == '{') sum += delayer(stream);
      else if (last6 == ":\"red\"") hasRed = true;
    }
  }
  if (hasRed) sum = 0;
  return sum;
}

int day12part2() {
  ifstream stream("input12.txt");
  int ans = delayer(stream);
  stream.close();
  return ans;
}

vector <vector<int> > permute(const vector<int> xs) {
  vector <vector<int> > ans;
  if (xs.size() == 1) {
    ans.push_back(xs);
    return ans;
  }

  vector<int> xsTail(xs.begin() + 1, xs.end());
  vector <vector<int> > perms = permute(xsTail);
  for (int i = 0; i < xs.size(); i++) {
    for (int j = 0; j < perms.size(); j++) {
      vector<int> perm(perms[j].begin(), perms[j].end());
      perm.insert(perm.begin() + i, xs[0]);
      ans.push_back(perm);
    }
  }
  return ans;
}
int calcHappiness(vector<int> a, int rel[][100]) {
  int total = 0;
  for (int i = 0; i < a.size(); i++) {
    total += rel[ a[i] ][ a[(i-1)%a.size()] ];
    total += rel[ a[i] ][ a[(i+1)%a.size()] ];
  }
  return total;
}

int day13part1 () {
  ifstream stream("input13.txt");
  string line;
  map<string, int> guests;
  int numGuests = 0;
  int relations[100][100];
  while (getline(stream, line)) {
    stringstream ss(line);
    string guest1, guest2, change, d;
    int happiness;
    ss >> guest1 >> d >> change >> happiness >> d >> d >> d >> d >> d >> d >> guest2;
    guest2 = guest2.substr(0, guest2.length()-1);
    if (change == "lose") happiness *= -1;
    if (guests.find(guest1) == guests.end()) {
      guests[guest1] = numGuests;
      numGuests++;
    }
    if (guests.find(guest2) == guests.end()) {
      guests[guest2] = numGuests;
      numGuests++;
    }
    relations[guests[guest1]][guests[guest2]] = happiness;
  }
  stream.close();

  vector<int> allGuests;
  for (int i = 0; i < numGuests; i++) allGuests.push_back(i);
  vector <vector<int> > arrangements = permute(allGuests);
  int maxHappiness = -999999;
  for (int i = 0; i < arrangements.size(); ++i) {
    int h = calcHappiness(arrangements[i], relations);
    if (h > maxHappiness) maxHappiness = h;
  }
  return maxHappiness;
}

int calcHappiness2(vector<int> a, int rel[][100]) {
  int total = 0;
  for (int i = 0; i < a.size(); i++) {
    total += rel[ a[i] ][ a[(i-1)%a.size()] ];
    total += rel[ a[i] ][ a[(i+1)%a.size()] ];
  }
  int bestAfterISit = -99999;
  for (int i = 0; i < a.size(); i++) {
    int newTotal = total - rel[ a[i] ][ a[(i+1)%a.size()] ] - rel[ a[(i+1)%a.size()] ][ a[i] ];
    if (newTotal > bestAfterISit) bestAfterISit = newTotal;
  }
  return bestAfterISit;
}

int day13part2 () { //just swap out calcHappiness for calcHappiness2
  ifstream stream("input13.txt");
  string line;
  map<string, int> guests;
  int numGuests = 0;
  int relations[100][100];
  while (getline(stream, line)) {
    stringstream ss(line);
    string guest1, guest2, change, d;
    int happiness;
    ss >> guest1 >> d >> change >> happiness >> d >> d >> d >> d >> d >> d >> guest2;
    guest2 = guest2.substr(0, guest2.length()-1);
    if (change == "lose") happiness *= -1;
    if (guests.find(guest1) == guests.end()) {
      guests[guest1] = numGuests;
      numGuests++;
    }
    if (guests.find(guest2) == guests.end()) {
      guests[guest2] = numGuests;
      numGuests++;
    }
    relations[guests[guest1]][guests[guest2]] = happiness;
  }
  stream.close();

  vector<int> allGuests;
  for (int i = 0; i < numGuests; i++) allGuests.push_back(i);
  vector <vector<int> > arrangements = permute(allGuests);
  int maxHappiness = -999999;
  for (int i = 0; i < arrangements.size(); i++) {
    int h = calcHappiness2(arrangements[i], relations);
    if (h > maxHappiness) maxHappiness = h;
  }
  return maxHappiness;
}

struct deer_t
{
  string name;
  int speed, flightTime, rest, dist, pts;
};

int day14part1() {
  int stopTime = 2503;
  ifstream stream("input14.txt");
  string line;
  vector<deer_t> deers;
  while (getline(stream, line)) {
    stringstream ss(line);
    deer_t deer;
    string d;
    ss >> deer.name >> d >> d >> deer.speed >> d >> d >> deer.flightTime >> d >> d >> d >> d >> d >> d >> deer.rest;
    deer.dist = 0;
    deer.pts = 0;
    deers.push_back(deer);
  }
  for (int t = 0; t < stopTime; t++) {
    for (int i = 0; i < deers.size(); i++) {
      bool canFly = t % (deers[i].flightTime + deers[i].rest) < deers[i].flightTime;
      if (canFly) deers[i].dist += deers[i].speed;
    }
  }
  int maxDist = 0;
  for (int i = 0; i < deers.size(); i++)
    if (deers[i].dist > maxDist) maxDist = deers[i].dist;
  return maxDist;
}

int day14part2() {
  int stopTime = 2503;
  ifstream stream("input14.txt");
  string line;
  vector<deer_t> deers;
  while (getline(stream, line)) {
    stringstream ss(line);
    deer_t deer;
    string d;
    ss >> deer.name >> d >> d >> deer.speed >> d >> d >> deer.flightTime >> d >> d >> d >> d >> d >> d >> deer.rest;
    deer.dist = 0;
    deer.pts = 0;
    deers.push_back(deer);
  }
  for (int t = 0; t < stopTime; t++) {
    for (int i = 0; i < deers.size(); i++) {
      bool canFly = t % (deers[i].flightTime + deers[i].rest) < deers[i].flightTime;
      if (canFly) deers[i].dist += deers[i].speed;
    }
    deer_t *winner = &deers[0];
    for (int i = 0; i < deers.size(); i++)
      if (deers[i].dist > winner->dist) winner = &deers[i];
    (winner->pts)++;
  }
  int maxPts = 0;
  for (int i = 0; i < deers.size(); i++)
    if (deers[i].pts > maxPts) maxPts = deers[i].pts;
  return maxPts;
}

class Ingredient
{
   public:
      string name;
      int capacity;
      int durability;
      int flavor;
      int texture;
      int calories;
      int teaspoons;
      Ingredient(string, int, int, int, int, int, int);
      Ingredient();
      Ingredient add(Ingredient);
      Ingredient times(int);
      int score();
};
Ingredient::Ingredient(string n, int c, int d, int f, int t, int cal, int tsp) {
  name = n;
  capacity = c;
  durability = d;
  flavor = f;
  texture = t;
  calories = cal;
  teaspoons = tsp;
}
Ingredient::Ingredient() {
  Ingredient("",0,0,0,0,0,0);
}
Ingredient Ingredient::add(Ingredient x) {
  // string n = name + x.name;
  int c = capacity + x.capacity;
  int d = durability + x.durability;
  int f = flavor + x.flavor;
  int t = texture + x.texture;
  int cal = calories + x.calories;
  int tsp = teaspoons + x.teaspoons;
  return Ingredient("",c,d,f,t,cal,tsp);
}
Ingredient Ingredient::times(int k) {
  // string n = name + intToString(k);
  int c = capacity * k;
  int d = durability * k;
  int f = flavor * k;
  int t = texture * k;
  int cal = calories * k;
  int tsp = teaspoons * k;
  return Ingredient("",c,d,f,t,cal,tsp);
}
int Ingredient::score() {
  return max(0,capacity) * max(0,durability) * max(0,flavor) * max(0,texture);
}

vector<Ingredient> combos(vector<Ingredient> ingredients, int teaspoons) {
  vector<Ingredient> ans;
  if (ingredients.size() == 1) {
    ans.push_back(ingredients[0].times(teaspoons));
    if(ans[0].teaspoons>100) cout <<"here"<<endl;
    return ans;
  }
  vector<Ingredient> tail(ingredients.begin() + 1, ingredients.end());

  for (int i = 0; i <= teaspoons; i++) {
    Ingredient remainingRecipe = ingredients[0].times(teaspoons-i);
    vector<Ingredient> partialRecipes = combos(tail, i);

    for (int j = 0; j < partialRecipes.size(); j++) {
      Ingredient newRecipe = partialRecipes[j].add(remainingRecipe);
      ans.push_back(newRecipe);
    }
  }
  return ans;
}

int day15part1() {
  ifstream stream("input15.txt");
  string line;
  vector<Ingredient> ingredients;
  while (getline(stream, line)) {
    Ingredient x;
    char name[30];
    sscanf (line.c_str(),"%s%*s %d %*s%*s %d %*s%*s %d %*s%*s %d %*s%*s %d", name, &x.capacity, &x.durability, &x.flavor, &x.texture, &x.calories);
    
    x.name = string(name);
    x.teaspoons = 1;
    ingredients.push_back(x);
  }
  stream.close();
  vector<Ingredient> allCombos = combos(ingredients, 100);
  int maxScore = 0;
  for (int i = 0; i < allCombos.size(); i++) {
    int score = allCombos[i].score();
    if (score > maxScore) maxScore = score;
  }
  return maxScore;
}

int day15part2() {
  ifstream stream("input15.txt");
  string line;
  vector<Ingredient> ingredients;
  while (getline(stream, line)) {
    Ingredient x;
    char name[30];
    sscanf (line.c_str(),"%s%*s %d %*s%*s %d %*s%*s %d %*s%*s %d %*s%*s %d", name, &x.capacity, &x.durability, &x.flavor, &x.texture, &x.calories);
    
    x.name = string(name);
    x.teaspoons = 1;
    ingredients.push_back(x);
  }
  stream.close();
  vector<Ingredient> allCombos = combos(ingredients, 100);
  int maxScore = 0;
  for (int i = 0; i < allCombos.size(); i++) {
    int score = allCombos[i].score();
    if (score > maxScore && allCombos[i].calories == 500) maxScore = score;
  }
  return maxScore;
}

 bool isAunt(map<string,int> evidence, map<string,int> memory) {
  for(map<string,int>::iterator thing = memory.begin(); thing != memory.end(); ++thing) {
    if (evidence[thing->first] != thing->second) return false;
  }
  return true;
}

int day16part1() {
  map<string, int> evidence;
  evidence["children:"] = 3;
  evidence["cats:"] = 7;
  evidence["samoyeds:"] = 2;
  evidence["pomeranians:"] = 3;
  evidence["akitas:"] = 0;
  evidence["vizslas:"] = 0;
  evidence["goldfish:"] = 5;
  evidence["trees:"] = 3;
  evidence["cars:"] = 2;
  evidence["perfumes:"] = 1;

  ifstream stream("input16.txt");
  string line;
  int ans;
  while (getline(stream, line)) {
    stringstream ss(line);
    int id, val;
    string key, d;
    map<string,int> aunt;

    ss >> d >> id >> d;

    while (ss >> key) {
      ss >> val >> d;
      aunt[key] = val;
    }
    if (isAunt(evidence, aunt)) {
      ans = id;
      break;
    }
  }
  stream.close();
  return ans;
}

bool isAunt2(map<string,int> evidence, map<string,int> memory) {
  for(map<string,int>::iterator thing = memory.begin(); thing != memory.end(); ++thing) {
    string key = thing->first;
    int val = thing->second;
    if (key == "cats:" || key == "trees:") {
      if ( !(val > evidence[key]) ) return false;
    }
    else if (key == "pomeranians:" || key == "goldfish:") {
      if ( !(val < evidence[key]) ) return false;
    }
    else if (evidence[key] != val) return false;
  }
  return true;
}

int day16part2() {
  map<string, int> evidence;
  evidence["children:"] = 3;
  evidence["cats:"] = 7;
  evidence["samoyeds:"] = 2;
  evidence["pomeranians:"] = 3;
  evidence["akitas:"] = 0;
  evidence["vizslas:"] = 0;
  evidence["goldfish:"] = 5;
  evidence["trees:"] = 3;
  evidence["cars:"] = 2;
  evidence["perfumes:"] = 1;

  ifstream stream("input16.txt");
  string line;
  int ans = -1;
  while (getline(stream, line)) {
    stringstream ss(line);
    int id, val;
    string key, d;
    map<string,int> aunt;

    ss >> d >> id >> d;

    while (ss >> key) {
      ss >> val >> d;
      aunt[key] = val;
    }
    if (isAunt2(evidence, aunt)) {
      ans = id;
      break;
    }
  }
  stream.close();
  return ans;
}

int fitExact(int liters, vector<int> containers) {
  int sumContainers = 0;
  for (int i = 0; i < containers.size(); i++) sumContainers += containers[i];
  if (sumContainers == liters) return 1;
  if (sumContainers < liters) return 0;
  if (liters == 0) return 1;
  if (liters < 0) return 0;
  vector<int> tail(containers.begin() + 1, containers.end());
  return fitExact(liters-containers[0], tail) + fitExact(liters, tail);
}

int day17part1() {
  ifstream stream("input17.txt");
  vector<int> containers;
  int n;
  while (stream >> n) containers.push_back(n);
  stream.close();
  return fitExact(150, containers);
}

vector<int> fitExact2(int liters, vector<int> containers) {
  vector<int> ans;
  int sumContainers = 0;
  for (int i = 0; i < containers.size(); i++) sumContainers += containers[i];
  if (sumContainers < liters || liters < 0) return ans;
  if (sumContainers == liters) {
    ans.push_back(containers.size());
    return ans;
  }
  if (liters == 0) {
    ans.push_back(0);
    return ans;
  }
  vector<int> tail(containers.begin() + 1, containers.end());

  vector<int> usingHead = fitExact2(liters - containers[0], tail);
  vector<int> notUsingHead = fitExact2(liters, tail);
  for (int i = 0; i < usingHead.size(); i++)
    ans.push_back(usingHead[i] + 1);
  for (int i = 0; i < notUsingHead.size(); i++)
    ans.push_back(notUsingHead[i]);
  return ans ;
}

int day17part2() {
  ifstream stream("input17.txt");
  vector<int> containers;
  int n;
  while (stream >> n) containers.push_back(n);
  stream.close();
  vector<int> solutions = fitExact2(150, containers);
  int minSol = 9999;
  int minSolCount = 0;
  for (int i = 0; i < solutions.size(); i++) {
    if (solutions[i] < minSol) {
      minSol = solutions[i];
      minSolCount = 1;
    }
    else if (solutions[i] == minSol) {
      minSolCount++;
    }
  }
  return minSolCount;
}


int numNeighbors(int (*display)[100][100], int x, int y) {
  int ans = 0;
  for (int i = x-1; i <= x+1; i++) {
    for (int j = y-1; j <= y+1; j++) {
      if (i < 100 && j < 100 && i >= 0 && j >= 0) {
        ans += (*display)[i][j];
      }
    }
  }
  return ans - (*display)[x][y];
}

void gameOfLifeStep(int (*display)[100][100], int (*oldDisplay)[100][100]) {
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      int n = numNeighbors(oldDisplay, i, j);
      if (n == 3) (*display)[i][j] = 1;
      else if ( (*oldDisplay)[i][j] == 1 && n == 2) (*display)[i][j] = 1;
      else (*display)[i][j] = 0;
    }
  }
}

int day18part1() {
  ifstream stream("input18.txt");
  int display[100][100];
  int oldDisplay[100][100];
  int (*display_ptr)[100][100] = &display;
  int (*oldDisplay_ptr)[100][100] = &oldDisplay;
  char c;
  int i = 0;
  while (stream >> c) {
    if (c == '.') display[i/100][i%100] = 0;
    else display[i/100][i%100] = 1;
    i++;
  }
  stream.close();

  for (int i = 0; i < 100; i++) {
    int (*temp)[100][100] = oldDisplay_ptr;
    oldDisplay_ptr = display_ptr;
    display_ptr = temp;
    gameOfLifeStep(display_ptr, oldDisplay_ptr);
  }
  int totalOn = 0;
  for (int i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++)
      if ((*display_ptr)[i][j] == 1)
        totalOn++;

  return totalOn;
}

void gameOfLifeStep2(int (*display)[100][100], int (*oldDisplay)[100][100]) {
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      int n = numNeighbors(oldDisplay, i, j);
      if (n == 3) (*display)[i][j] = 1;
      else if ( (*oldDisplay)[i][j] == 1 && n == 2) (*display)[i][j] = 1;
      else (*display)[i][j] = 0;
    }
  }
  (*display)[0][0] = 1;
  (*display)[0][99] = 1;
  (*display)[99][0] = 1;
  (*display)[99][99] = 1;
}

int day18part2() {
  ifstream stream("input18.txt");
  int display[100][100];
  int oldDisplay[100][100];
  int (*display_ptr)[100][100] = &display;
  int (*oldDisplay_ptr)[100][100] = &oldDisplay;
  char c;
  int i = 0;
  while (stream >> c) {
    if (c == '.') display[i/100][i%100] = 0;
    else display[i/100][i%100] = 1;
    i++;
  }
  stream.close();

  for (int i = 0; i < 100; i++) {
    int (*temp)[100][100] = oldDisplay_ptr;
    oldDisplay_ptr = display_ptr;
    display_ptr = temp;
    gameOfLifeStep2(display_ptr, oldDisplay_ptr);
  }
  int totalOn = 0;
  for (int i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++)
      if ((*display_ptr)[i][j] == 1)
        totalOn++;

  return totalOn;
}


struct replacement_t
{
  string start;
  string end;
};

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

  for (int i = 0; i < replacements.size(); i++) {
    size_t found = medicine.find(replacements[i].start);
    while (found != string::npos) {
      string newElement = medicine;
      newElement.replace(found, replacements[i].start.length(), replacements[i].end);
      table[newElement] = true;
      found = medicine.find(replacements[i].start, found+1);
    }
  }

  return table.size();
}


int main () {
  cout << day19part1() << endl;
  return 0;
}