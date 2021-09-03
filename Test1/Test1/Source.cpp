#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>



#include "Header.h"

using namespace std;
namespace fs = std::filesystem;

float roundd(float var)
{
	// 37.66666 * 100 =3766.66
	// 3766.66 + .5 =3767.16    for rounding off value
	// then type cast to int so value is 3767
	// then divided by 100 so the value converted into 37.67
	float value = (int)(var * 100 + .5);
	return (float)value / 100;
}

int nthSubstr(int n, const string& s,
	const string& p) {
	string::size_type i = s.find(p);     // Find the first occurrence

	int j;
	for (j = 1; j < n && i != string::npos; ++j)
		i = s.find(p, i + 1); // Find the next occurrence

	if (j == n)
		return(i);
	else
		return(-1);
}

int main() {
	std::cout << "This program combines finra short interest reports into one Excel File" << endl << endl;
	std::cout << "Supported Features: Show number of trades reported to FINRA in each day" << endl << "Calculate percentage of short trades" << endl << "Remove Insignificant Data via:" << endl << "    Removal of small number of trades" << endl << "    Stock ticker comparison/removal" << endl << "Accurate representation/collection of data via ticker comparison" << endl << endl;


	std::vector <string> dates;
	std::string path = "C:/Users/jaehw/Desktop/FINRA SHORT/History/2021_08";
	int i = 0;
	for (const auto& entry : fs::directory_iterator(path)) {
		dates.push_back(entry.path().string());
		i = i + 1;
	}

	/*
	int k = dates.size();
	for (int i = k; i == 0; i--) {
		if (dates[i] == ""){
			dates.pop_back();
		}
	}
	*/

	std::cout << "Detected FINRA Short reports: " << endl << endl;
	for (int i = 0; i < dates.size(); i++) {

		//dates[i] = dates[i].substr(dates[i].find('CNM') - 2, dates[i].find('2') + 17);
		std::cout << dates[i] << endl;
	}

	std::cout << endl << endl;



	string FINRA_FILE = dates[dates.size()-1];
	string EachLine = "";
	string content = "";
	//std::cout << FINRA_FILE;
	ofstream MyFile("filename.txt");
	ifstream File(FINRA_FILE);


	getline(File, EachLine);

	DD.Date = dates[dates.size()-1].substr(64, 4);
	float shortpercentage;
	int ii = 0;
	while (getline(File, EachLine)) {
		Dtemp.DateArray.clear();
		if (EachLine[0] == '2') {
			size_t first = nthSubstr(1, EachLine, "|");
			size_t second = nthSubstr(2, EachLine, "|");
			size_t third = nthSubstr(3, EachLine, "|");
			size_t fourth = nthSubstr(4, EachLine, "|");
			size_t fifth = nthSubstr(5, EachLine, "|");
			size_t sf = second - first;
			size_t ts = third - second;
			size_t ft = fourth - third;
			size_t ff = fifth - fourth;

			shortpercentage = (stof(EachLine.substr(second + 1, ts - 1)) / stof(EachLine.substr(fourth + 1, ff - 1))) * 100;

			Dtemp.Ticker = EachLine.substr(first + 1, sf - 1);
			//std::cout << Dtemp.Ticker;
			DD.volume = int(round(stof(EachLine.substr(fourth + 1, ff - 1))));
			DD.ShortPercentage = int(round(shortpercentage));
			Dtemp.DateArray.push_back(DD);
			Datas.push_back(Dtemp);
			//std::cout << Datas.at(ii).Ticker;
		}
		ii++;
	}
	File.close();


	for (int w = dates.size()-2; w > -1; w--) {
		string FINRA_FILE2 = dates[w];
		string EachLine2 = "";
		string EachLline = "";

		ifstream File2(FINRA_FILE2);

		getline(File2, EachLine);
		DD.Date = dates[w].substr(64, 4);
		Datas2.clear();
		while (getline(File2, EachLine)) {
			DDD.DateArray.clear();
			if (EachLine[0] == '2') {
				size_t first = nthSubstr(1, EachLine, "|");
				size_t second = nthSubstr(2, EachLine, "|");
				size_t third = nthSubstr(3, EachLine, "|");
				size_t fourth = nthSubstr(4, EachLine, "|");
				size_t fifth = nthSubstr(5, EachLine, "|");
				size_t sf = second - first;
				size_t ts = third - second;
				size_t ft = fourth - third;
				size_t ff = fifth - fourth;

				shortpercentage = (stof(EachLine.substr(second + 1, ts - 1)) / stof(EachLine.substr(fourth + 1, ff - 1))) * 100;

				DDD.Ticker = EachLine.substr(first + 1, sf - 1);
				//std::cout << Dtemp.Ticker;
				DD.volume = int(round(stof(EachLine.substr(fourth + 1, ff - 1))));
				DD.ShortPercentage = int(round(shortpercentage));
				DDD.DateArray.push_back(DD);
				Datas2.push_back(DDD);
				//std::cout << Datas.at(ii).Ticker;
			}
		}

		DD.ShortPercentage = 0;
		DD.volume = 0;
		for (int p = 0; p < Datas.size(); p++) {
			Datas.at(p).DateArray.push_back(DD);
		}

		bool findflag = 0;
		for (int k = 0; k < Datas.size(); k++) {
			for (int j = 0; j < Datas2.size(); j++) {
				if (Datas.at(k).Ticker == Datas2.at(j).Ticker) {
					Datas.at(k).DateArray.at(Datas.at(k).DateArray.size()-1).ShortPercentage = Datas2.at(j).DateArray.at(0).ShortPercentage;
					Datas.at(k).DateArray.at(Datas.at(k).DateArray.size()-1).volume = Datas2.at(j).DateArray.at(0).volume;
					Datas2.erase(Datas2.begin() + j);
					j--;
					break;
				}
				if (int(Datas.at(k).Ticker[0]) < int(Datas2.at(j).Ticker[0])) {
					break;
				}
			}
		}
		File2.close();

	}
	MyFile << " ";
	for (int i = dates.size()-1; i > -1; i--) {
		MyFile << "(";
		MyFile << dates[i][64];
		MyFile << dates[i][65];
		MyFile << "/";
		MyFile << dates[i][66];
		MyFile << dates[i][67];
		MyFile << ")";
		MyFile << " ";
		MyFile << "(";
		MyFile << dates[i][64];
		MyFile << dates[i][65];
		MyFile << "/";
		MyFile << dates[i][66];
		MyFile << dates[i][67];
		MyFile << ")";
		MyFile << " ";
	}
	MyFile << "\n";
	for (int k = 0; k < Datas.size(); k++) {
		MyFile << Datas.at(k).Ticker;
		MyFile << " ";
		for (int z = 0; z < Datas.at(k).DateArray.size(); z++) {

			MyFile << Datas.at(k).DateArray.at(z).volume;
			MyFile << " ";
			MyFile << Datas.at(k).DateArray.at(z).ShortPercentage;
			MyFile << "%";
			MyFile << " ";
		}

		MyFile << "\n";
	}
	MyFile.close();
}
	
//	std::cout << Datas.size();
	//std::cout << Datas.at(i).DateArray.size();


	
	/*for (int i = 0; i < 100; i++) {
		std::cout << Datas.at(i).Ticker << endl;
		for (int e = 0; e < Datas.at(i).DateArray.size(); e++) {
			std::cout << "    " << "Date :" << Datas.at(i).DateArray.at(0).Date << endl;
			std::cout << "    " << "Vol  :" << Datas.at(i).DateArray.at(0).volume << endl;
			std::cout << "    " << "Short:" << Datas.at(i).DateArray.at(0).ShortPercentage << endl << endl;

		}
	}*/

	/*
		std::cout << Datas2.at(i).Ticker << endl;
		std::cout << "    " << "Date :" << Datas2.at(i).DateArray.at(0).Date << endl;
		std::cout << "    " << "Vol  :" << Datas2.at(i).DateArray.at(0).volume << endl;
		std::cout << "    " << "Short:" << Datas2.at(i).DateArray.at(0).ShortPercentage << endl << endl << endl;

	}
	*/






	/*
	for (int k = 0; k < Datas.size(); k++) {
		while (getline(File2, EachLine)) {
			if (k == 0){
				if (EachLine[0] == '2') {
					size_t first = nthSubstr(1, EachLine, "|");
					size_t second = nthSubstr(2, EachLine, "|");
					size_t third = nthSubstr(3, EachLine, "|");
					size_t fourth = nthSubstr(4, EachLine, "|");
					size_t fifth = nthSubstr(5, EachLine, "|");
					size_t sf = second - first;
					size_t ts = third - second;
					size_t ft = fourth - third;
					size_t ff = fifth - fourth;

					shortpercentage = (stof(EachLine.substr(second + 1, ts - 1)) / stof(EachLine.substr(fourth + 1, ff - 1))) * 100;

					Dtemp.Ticker = EachLine.substr(first + 1, sf - 1);
					//std::cout << Dtemp.Ticker;
					DD.volume = int(round(stof(EachLine.substr(fourth + 1, ff - 1))));
					DD.ShortPercentage = int(round(shortpercentage));
					Dtemp.DateArray.push_back(DD);
					Datas.push_back(Dtemp);
					//std::cout << Datas.at(ii).Ticker;
				}
			}

			size_t first = nthSubstr(1, EachLine, "|");
			size_t second = nthSubstr(2, EachLine, "|");
			size_t third = nthSubstr(3, EachLine, "|");
			size_t fourth = nthSubstr(4, EachLine, "|");
			size_t fifth = nthSubstr(5, EachLine, "|");
			size_t sf = second - first;
			size_t ts = third - second;
			size_t ft = fourth - third;
			size_t ff = fifth - fourth;
				//std::cout << ticker << '\n';
			//	std::cout << EachLine.substr(first + 1, sf - 1) << '\n';



				if (EachLine.substr(first + 1, sf - 1) == Datas.at(k).Ticker) {


					shortpercentage = (stof(EachLine.substr(second + 1, ts - 1)) / stof(EachLine.substr(fourth + 1, ff - 1))) * 100;


					DD.volume = int(round(stof(EachLine.substr(fourth + 1, ff - 1))));
					DD.ShortPercentage = int(round(shortpercentage));
					break;
				}


			}
		}
	}
	if (findflag == false) {
		content += "N/A";
		content += " ";
		content += "N/A";
		content += "\n";;
		skipcount = 0;
		skipcountflag = true;
	}

	File2.close();

	TargetFile << content;

	TargetFile.close();

	if (SourceFile == "filename.txt") {
		SourceFile = "filename2.txt";
	}

	if (SourceFile == "filename2.txt") {
		SourceFile = "filename.txt";
	}

	//	}

}*/



