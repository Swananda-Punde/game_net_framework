#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<stdlib.h>
#include <bits/stdc++.h>
#include<boost/asio.hpp>
#include<boost/asio/io_context.hpp>
#include<boost/asio/ip/tcp.hpp>
#include<boost/asio/signal_set.hpp>
#include<boost/asio/write.hpp>

using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::stringstream;
using boost::asio::ip::tcp;

class client {
	public :
		client(boost::asio::io_context &io_context, string address, int port):io_context(io_context), socket(io_context), address(address), 
		port_no(port){
			recv[0] = send[0] = '\0';
			connect();
			new_connection();
		}
		void close(){
			socket.close();
		}
	private :
		boost::asio::io_context &io_context;
		tcp::socket socket;
		string address;
		char recv[1024], send[1024];
		int port_no;

		int read() {
			recv[0] = '\0';
			return socket.read_some(boost::asio::buffer(recv, 1024));
		}
		int write() {
			return socket.write_some(boost::asio::buffer(send, 1024));
			send[0] = '\0';
		}
		void connect() {
			boost::system::error_code err;
			//while(1) //if error ; connect again
			socket.connect(tcp::endpoint(boost::asio::ip::make_address(address), port_no),err);
			if(err.value() != 0){
				cout << "Error to connect ."<<endl;
				return;
			}

			do
			{
				string t1, t2;
				cout <<"Enter user Name: ";
				std::cin >>t1 ;
				cout <<"password : ";
				std::cin >> t2;
				t1.append("\0");
				t2.append("\0");
				//socket.write_some(boost::asio::buffer(t1, t1.length()));
				//socket.write_some(boost::asio::buffer(t2, t2.length()));
				strcpy(send, t1.c_str());
				strcat(send, ": ");
				strcat(send, t2.c_str());
				write();

				socket.read_some(boost::asio::buffer(recv, 1));
				//1();
				cout <<"received Buffer : "<<recv<<endl;
				int x;
				stringstream buffer(recv);
				buffer >> x;
				if(x != 0)
					break;
				else
					cout <<"Wrong Username or password."<<endl;
			}while(1);
			sleep(1);
			
			recv[0] = '\0';
			socket.read_some(boost::asio::buffer(recv, 1024));
			cout <<"received Buffer : "<<recv<<endl;
			stringstream buffer(recv);
			buffer >> port_no;
			cout << "temp : "<<recv<<"\nNew POrt no : "<<port_no<<endl;
			close();
		}
		void new_connection() { 
			boost::system::error_code err;
			socket.connect(tcp::endpoint(boost::asio::ip::make_address(address), port_no),err);
			if(err.value() != 0){
				cout << "Error to connect ."<<endl;
				return;
			}
			while(1) {
				string temp;
				cout <<"\nEnter msg to send : ";
				cin >> send;
				write();
				//socket.write_some(boost::asio::buffer(temp, temp.length()));
				//if(temp.compare("exit") == 0){
				if(strcmp(send,"exit") == 0){
					cout << "In temp compare."<< endl;
					//close();
					socket.release();
					return;
					break;
				}
				//socket.read_some(boost::asio::buffer(temp, temp.length()));
				cout << "Hiiiisdpiojsdg"<<endl;
				read();
				cout <<"Received back : "<<recv<<endl;
			}
		}
};
int main(int argc, char* argv[]){
	if(argc != 3){
		cout << "Required arguments : ./client <ip_address> <port_no>"<<endl;
		return 0;
	}
	boost::asio::io_context io_context;
	client cl(io_context, argv[1], atoi(argv[2]));
	//cl.close();
	cout<<"Executed normally"<<endl;
	//io_context.run();
}
