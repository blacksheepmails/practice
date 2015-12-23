#include <iostream>     // std::cin, std::cout
#include <fstream>      // std::ifstream
#include <string>
#include <sstream>
#include <algorithm>

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

int main () {
  cout << day1part1() << endl;
  cout << day1part2() << endl;
  cout << day2part1() << endl;
  cout << day2part2() << endl;

  return 0;
}