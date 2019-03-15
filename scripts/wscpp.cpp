#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <map>

using namespace std;

class WS {
public:
	string name;
	string left;
	string right;
	//WS(const string& name): name(name) {}
	void setName(const string& name_){
		name = name_;
	}
	void addTag(const string& left_, const string& right_) {
		left = left_ + left;
		right = right + right_;
	}
	inline string gen(){
		return left + " " + name + " " + right;
	}
};

FILE* f;
char buff[64];
auto readline() {
	auto res = fgets(buff, 64, f);
	if(!res) return res;
	uint8_t l = strlen(buff);
	if(buff[l-1] == '\n')
		buff[l-1] = '\0';
	return res;
}
int main() {
	map<string, WS> wss;
	f = popen("bspc query -D --names", "r");
	while (readline()) {
		wss[buff].setName(buff);
	}
	pclose(f);
	f = popen("bspc query -D --names -d .urgent", "r");
	while (readline()) {
		wss[buff].addTag("%{F#ff000000}", "%{F-}");
	}
	pclose(f);
	f = popen("bspc query -D --names -d .occupied", "r");
	while (readline()) {
		wss[buff].addTag("%{F#77ccff}", "%{F-}");
	}
	pclose(f);
	f = popen("bspc query -D --names -d .focused", "r");
	while (readline()) {
		wss[buff].addTag("%{U#99bbff}", "%{U-}");
	}
	pclose(f);

	for (auto& ws : wss) {
		cout << ws.second.gen();
		cout << " ";
	}
}

