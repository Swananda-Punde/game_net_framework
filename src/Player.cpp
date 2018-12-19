#include "../headers/Player.h"

using std::string;

Player::Player(){
	id = "\0";
	name = "\0";
}

Player::Player(string id, string name):id(id), name(name){
}

Player::Player(Player &obj){
	id = obj.id;
	name = obj.name;
}

string Player::get_id(){
	return id;
}

string Player::get_name(){
	return name;
}

void Player::set_id(string id){
	this->id = id;
}

void Player::set_name(string name){
	this->name = name;
}

template<typename Archive>
void Player::serialize(Archive &ar, const unsigned int version){
	ar & id;
	ar & name;
}
