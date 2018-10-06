#include <iostream>
#include <map>
#include <fstream>
#include <cassert>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <set>
#include <algorithm>
#include <locale>

using namespace std;

struct letter_only: std::ctype<char> 
{
    letter_only(): std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const* get_table()
    {
        static std::vector<std::ctype_base::mask> 
            rc(std::ctype<char>::table_size,std::ctype_base::space);

        std::fill(&rc['A'], &rc['z'+1], std::ctype_base::alpha);
        return &rc[0];
    }
};


int main(int argc, char* argv[]){
	string line;
	ifstream myfile ("../stop_words.txt");
	vector<string> strs; 
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			boost::split(strs, line, boost::is_any_of(","));
		}
		myfile.close();
	}
	std::map<std::string, int> wordCount;
	ifstream input;
	input.imbue(std::locale(std::locale(), new letter_only())); //enable reading only letters!
	input.open(argv[1]);
	std::string word;
	while(input >> word)
	{
		std::transform(word.begin(), word.end(), word.begin(), ::tolower);
		word.erase(std::remove_if(word.begin(), word.end(), [](char c) { return !std::isalnum(c); }), word.end());
		if (std::find(strs.begin(), strs.end(), word) != strs.end()){
			continue;
		}
		else{
			if (word.length() > 1){
				++wordCount[word];
			}
		}
	}
	// typedef std::function<bool(std::pair<std::string, int>, std::pair<std::string, int>)> Comparator;
	// Comparator compFunctor = [](std::pair<std::string, int> elem1 ,std::pair<std::string, int> elem2)
	// 		{
	// 			return elem1.second > elem2.second;
	// 		};

	// std::set<std::pair<std::string, int>, Comparator> setOfWords(
	// 		wordCount.begin(), wordCount.end(), compFunctor);

	// for (std::pair<std::string, int> element : setOfWords){
	// 	std::cout << element.first << " :: " << element.second << std::endl;
	// }
	vector<pair<string,int>>vec;
	for(auto x=wordCount.begin();x!=wordCount.end();x++)vec.push_back(*x);
	sort(vec.begin(),vec.end(),[](pair<string,int>elem1,pair<string,int>elem2){return elem1.second > elem2.second;});
	for(auto const& pair: vec)
	{
		std::cout << pair.first << " - " << pair.second << "\n";
	}

	return 0;
}