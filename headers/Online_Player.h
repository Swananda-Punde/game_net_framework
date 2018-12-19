#ifndef ONLINE_PLAYERS_H
#define ONLINE_PLAYERS_H

#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>
#include"Player.h"

class Online_Player {
	public :
		Online_Player();
		Online_Player(Player player, int port_no);
		int get_port_no();
		Player get_player();
		void set_port_no(int port_no);
		void set_player(Player player);
	private :
		Player player;
		int port_no;
		template<typename Archive>
		void serialize(Archive &ar, const unsigned int version);
};

#endif
