#ifndef AUTHENTICATION
#define AUTHENTICATION

#include<iostream>
#include<boost/archive/text_oarchive.hpp>
#include<boost/archive/text_iarchive.hpp>

using std::string;

class Authentication {
	public :
		Authentication(string, string);
		string get_user();
		string get_pass();
	private :
		string user, pass;
		friend class boost::serialization::access;

		template <typename Archive>
		void serialize(Archive &ar, const unsigned int version);
};

#endif
