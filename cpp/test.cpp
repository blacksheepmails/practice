#include <iostream>     // std::cin, std:://cout
#include <fstream>      // std::ifstream
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <numeric>
#include <stack>
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

using namespace std;

class Effect;

struct GameState {
	int mana;
	int cost;
	vector<Effect*> effects;
	int myShield;
	int myHP;
	int bossHP;
	int bossDamage;
	GameState(int, int, int, int);
	GameState(const GameState&);
};

class Effect {
	public:
		int turnsLeft;
		int cost;
		virtual bool trigger(GameState *g) = 0; 			//return true when finished
		virtual Effect* clone() = 0;
		Effect(int c, int t) : cost(c), turnsLeft(t) {}
};
class Shield : public Effect {
	public:
		virtual bool trigger(GameState *g) {
			turnsLeft--;
			g->myShield = 7;
			//cout << "shields" << " b" << g->bossHP << " m" << g->myHP << endl;
			return turnsLeft == 0;
		}
		Shield():Effect(113,6){}
		Shield(const Shield& x):Effect(x.cost, x.turnsLeft){}
		virtual Shield* clone() {
			//cout << "newShield ";
			return new Shield(*this);
		}
};

class Poison : public Effect {
	public:
		virtual bool trigger(GameState *g) {
			turnsLeft--;
			g->bossHP -= 3;
			//cout << "poison" << " b" << g->bossHP << " m" << g->myHP << endl;
			return turnsLeft == 0;
		}
		Poison():Effect(173,6){}
		Poison(const Poison& x):Effect(x.cost, x.turnsLeft){}
		virtual Poison* clone() {
			//cout << "newPoison ";
			return new Poison(*this);
		}
};
class Recharge : public Effect {
	public:
		virtual bool trigger(GameState *g) {
			turnsLeft--;
			g->mana += 101;
			//cout << "recharge" << " b" << g->bossHP << " m" << g->myHP << " mana" << g->mana << endl;
			return turnsLeft == 0;
		}
		Recharge():Effect(229,5){}
		Recharge(const Recharge& x):Effect(x.cost, x.turnsLeft){}
		virtual Recharge* clone() {
			//cout << "newRecharge ";
			return new Recharge(*this);
		}
};

class Spell {
	public:
		int cost;
		virtual void trigger(GameState *g) = 0;
		Spell(int c):cost(c){}
};
class MagicMissile : public Spell {
	public:
		virtual void trigger(GameState *g) {
			g->bossHP -= 4;
			//cout << "magic missile" << " b" << g->bossHP << " m" << g->myHP << endl;
		}
		MagicMissile():Spell(53){}
};
class Drain : public Spell {
	public:
		virtual void trigger(GameState *g) {
			g->bossHP -= 2;
			g->myHP += 2;
			//cout << "draining" << " b" << g->bossHP << " m" << g->myHP << endl;
		}
		Drain():Spell(73){}
};

bool bossMove(GameState *g) { //returns if i am killed
	g->myHP = g->myHP - max(g->bossDamage-g->myShield, 1);
	return g->myHP <= 0;
}
void triggerEvents(GameState *g) {
	g->myShield = 0;

	for (int i = 0; i < g->effects.size(); i++) {					//triggers effects. 
		//cout << g->effects[i]->cost;
		if (g->effects[i]->trigger(g)) {
			//delete the effect
			g->effects.erase(g->effects.begin() + i);
			i--;
		}
	}
	//cout << "shield" << g->myShield << ' ';
}

GameState::GameState(int m, int h1, int h2, int d) {
	mana = m;
	cost = 0;
	myShield = 0;
	myHP = h1;
	bossHP = h2;
	bossDamage = d;
}
GameState::GameState(const GameState& g) {
	mana = g.mana;
	cost = g.cost;
	for (int i = 0; i < g.effects.size(); i++){
		effects.push_back(g.effects[i]->clone());
	}
	myShield = g.myShield;
	myHP = g.myHP;
	bossHP = g.bossHP;
	bossDamage = g.bossDamage;
}


int day22part2() {
	stack<GameState> q;
	vector<Effect*> effects;
	Shield shieldEffect = Shield();
	Poison poisonEffect = Poison();
	Recharge rechargeEffect = Recharge();
	effects.push_back(&shieldEffect);
	effects.push_back(&poisonEffect);
	effects.push_back(&rechargeEffect);

	vector<Spell*> spells;
	MagicMissile magicMissileSpell = MagicMissile();
	Drain drainSpell = Drain();
	spells.push_back(&magicMissileSpell);
	spells.push_back(&drainSpell);

	int minCost = 9999999;
	
	q.push(GameState(500, 50, 51, 9));
	// q.push(GameState(250, 10, 14, 8));
	// //cout << '(';
	while ( !q.empty() ) {
		// //cout << q.size() << 'm' << minCost << ' ';
		GameState g = q.top();
		q.pop();
		//cout << endl << "(b" << g.bossHP << " p" << g.myHP << endl;
		g.myHP--;
		if (g.myHP <= 0) continue;
		else {
			triggerEvents(&g);
			if (g.bossHP <= 0) {						//if boss dies now, update minCost, move next node
				if ((g.cost) < minCost) {
					minCost = g.cost;
				}
				//cout << "win. myhp: "<<g.myHP << ' ';
			} 
			else {									//ok, boss lives
				for (int i = 0; i < spells.size(); i++) {
					Spell *spell = spells[i];
					if (g.cost + spell->cost < minCost && g.mana - spell->cost >= 0) {
						GameState f(g);
						spell->trigger(&f);
						f.cost += spell->cost;
						f.mana -= spell->cost;
						//cout << "sc" << spell->cost << "gc" << f.cost << endl;

						if (f.bossHP <= 0) {				//boss dies, update minCost
							if ((f.cost) < minCost) minCost = f.cost;
							//cout << "win. myhp: "<<f.myHP << ' ';
						}
						else {
							triggerEvents(&f);
							if (f.bossHP <= 0) {				//boss dies, update minCost
								if ((f.cost) < minCost) {
									minCost = f.cost;
								}
								//cout << "win. myhp: "<<f.myHP << ' ';
							}
							else if (!bossMove(&f)) {
								//cout << "push";
								q.push(f); //if boss doesnt win, push new GameState f
							}
							//else //cout << "lostA";
						}
					}
				}
				for (int i = 0; i < effects.size(); i++) {
					Effect *effect = effects[i];
					bool active = false;
					for (int j = 0; j < g.effects.size(); j++) {
						if (effect->cost == g.effects[j]->cost) active = true;
					}
					if (!active && g.cost + effect->cost < minCost && g.mana-effect->cost >= 0) {
						GameState f(g);
						f.effects.push_back(effect->clone());
						f.cost += effect->cost;
						f.mana -= effect->cost;
						//cout << "ec" << effect->cost << "gc" << f.cost << endl;
						//cout << "numEffects" << f.effects.size() << ' ';
						triggerEvents(&f);
						if (f.bossHP <= 0) {				//boss dies, update minCost
								if ((f.cost) < minCost) {
									minCost = f.cost;
								}
								//cout << "win. myhp: "<<f.myHP << ' ';
							}
						else if (!bossMove(&f)) {
							//cout << "push";
							q.push(f);
						}
						//else //cout << "lostB. myHP:" << f.myHP<<endl;
					}
				}
			}
				//cout << ')';
		}
	}
	return minCost;
}

int main () {
  cout << day22part2() << endl;
  return 0;
}

// void print2DVector(vector <vector<int> > xs) {
// 	for (int i = 0; i < xs.size(); i++) {
// 		for (int j = 0; j < xs[i].size(); j++) //cout << xs[i][j] << ", ";
// 		//cout << endl;
// 	}
// 	//cout << endl;
// }
// void printVector(vector<int> xs) {
// 	for (int i = 0; i < xs.size(); i++) {
// 		//cout << xs[i];
// 	}
// 	//cout << endl;
// }