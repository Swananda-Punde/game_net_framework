#ifndef GAME_REQUEST_H
#define GAME_REQUEST_H

#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>
#include"my_enums.h"

using std::string;

class Game_Request {
	public :
		Game_Request();
		Game_Request(game_no , string, string, bool);
		Game_Request(Game_Request &);
		string get_player1_id();
		string get_playre2_id();
		game_no get_gameno();
		bool get_reply();
		void set_reply(bool);
		void set_gameno(game_no);
		void set_player1_id(string);
		void set_player2_id(string);
	private :
		game_no gameno;
		string player1_id, player2_id;
		bool reply;
		template<typename Archive>
		void serialize(Archive &ar, const unsigned int version);
};

#endif
