#ifndef PLAYER
#define PLAYER

#include<iostream>
#include<boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using std::string;

class Player{
	public :
		Player();
		Player(string id, string name);
		Player(Player &);
		string get_id();
		string get_name();
		void set_id(string id);
		void set_name(string name);
	private :
		string id, name;
		template<typename Archive>
		void serialize(Archive &ar, const unsigned int version);
};

#endif
