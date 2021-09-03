#include <vector>
using namespace std;
struct Dates {
	string Date;
	int volume;
	int ShortPercentage;
}DD;

struct Data {
	string Ticker;
	vector <Dates> DateArray;
}DDD,Dtemp;


vector <Data> Datas;
vector <Data> Datas2;