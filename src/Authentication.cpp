#include "../headers/Authentication.h"

using std::string;

Authentication::Authentication(string user, string pass):user(user), pass(pass){
}

string Authentication::get_user(){
	return user;
}

string Authentication::get_pass(){
	return pass;
}

template <typename Archive>
void Authentication::serialize(Archive &ar, const unsigned int version){
	ar & user;
	ar & pass;
}
