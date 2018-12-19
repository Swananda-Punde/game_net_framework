#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>
#include"../headers/Player.h"
#include"../headers/Online_Players.h"

Online_Player::Online_Player(){
	port_no = -1;
}

Online_Player::Online_Player(Player player, int port_no):player(player), port_no(port_no){
}

int Online_Player::get_port_no(){
	return port_no;
}

Player Online_Player::get_player(){
	return player;
}

void Online_Player::set_port_no(int port_no){
	this->port_no = port_no;
}

void Online_Player::set_player(Player player){
	this->player.set_id(player.get_id());
	this->player.set_name(player.get_name());
}

template<typename Archive>
void Online_Player::serialize(Archive &ar, const unsigned int version){
	ar & player.get_id();
	ar & player.get_name();
	ar & port_no;
}
