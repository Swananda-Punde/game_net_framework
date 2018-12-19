#include<iostream>
#include<string.h>
#include<cstdlib>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include <boost/bind.hpp>
#include<boost/asio.hpp>
#include<boost/asio/io_context.hpp>
#include<boost/asio/ip/tcp.hpp>
#include<boost/asio/signal_set.hpp>
#include<boost/asio/write.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::to_string;
using boost::asio::ip::tcp;

class child_server {
	public :
		child_server(boost::asio::io_context &io_context, int port): io_context(io_context), socket(io_context), port(port) {
			recv[0] = send[0] = '\0';
			accept();
		}
		int read() {
			int size ;
			
			try {
				size = socket.read_some(boost::asio::buffer(recv, 1024));
				return size;
			}
			catch(std::exception& e){
				cout << "Error Occured at read some in child server."<<endl;
				return 0;
			}
		}
		int write() {
			return socket.write_some(boost::asio::buffer(send, 1024));
		}
	private :
		boost::asio::io_context &io_context;
		tcp::socket socket;
		int port;
		char recv[1024], send[1024];

		void accept() {
		try{
			boost::system::error_code err;
			
			tcp:: acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
			acceptor.set_option(tcp::acceptor::reuse_address(true));
			acceptor.accept(socket, err);
			while(read() > 0){
				cout << "Received : "<<recv<<endl;
				strcpy(send, recv);
				cout<<"\nTo send : "<<send<<endl;
				if(strcmp(recv,"exit") == 0){
					cout<<"in if strcmp : "<<endl;
					socket.close();
				//	socket.cancel();
				//	break;
					return ;
				}

				write();
				recv[0] = send[0] = '\0';
			}
			//io_context.run();
		}
		//catch(boost::system::system_error e){
		catch(std::exception &e){
			cout <<"\nError occured : "<<e.what()<<endl;
			socket.cancel();
		}
		}
};

class server {
	public :
		server(boost::asio::io_context &io_context, unsigned short int port):io_context (io_context), signal(io_context,SIGCHLD),
		acceptor(io_context,{tcp::v4(), port}), socket(io_context){
			new_port = port+1;
			wait_for_signal();
			//accept();
			acceptor.set_option(tcp::acceptor::reuse_address(true));
			acceptor.async_accept(socket, boost::bind(&server::accept, this));
			io_context.run();
		}
		void accept(){
			boost::system::error_code err;
			
			
			//try{
			//tcp::socket *socket;	
			
			while(1) {
		//	boost ::shared_ptr<tcp::socket> socket(new tcp::socket(io_context));
			tcp::socket *socket = &this->socket;//new tcp::socket(io_context);
			try {
				//acceptor.listen();
				//acceptor.accept((*socket), err);
			cout << "Connected to : " << (*socket).remote_endpoint() << endl;
			
				(*socket).read_some(boost::asio::buffer(buffer,1024), err);
				if(err){
					cout << "In error code : "<<err.value()<<endl;
					break;
				}
			
				cout << "\nbuffer : "<<buffer<<endl;
				if(validate()){
					// Notify client the username and password is right 
					(*socket).write_some(boost::asio::buffer(to_string(true),2));
					
					string t = std::to_string(new_port);
					t.append("\0");
					cout<<"\nNew Port : "<<new_port<<endl;
					//Send client new Port number
					(*socket).write_some(boost::asio::buffer(t, t.length()));

					io_context.notify_fork(boost::asio::io_context::fork_prepare);
					
					if(fork() == 0) {
						io_context.notify_fork(boost::asio::io_context::fork_child);
						acceptor.close();
						signal.cancel();
						child_server child(io_context, new_port);
					}
					else{
						io_context.notify_fork(boost::asio::io_context::fork_parent);
						(*socket).close();
					}
					new_port++;
				}
				else{ //validation failed
					string t = std::to_string(false);
					(*socket).write_some(boost::asio::buffer(t, t.length()));
				}
			}
			catch(std::exception &e){
				cout << "Caught exception : "<< e.what() << endl;
				cout << "Hiiiiiiiiiiiiiiiiiiiiii"<< endl;
				delete socket;
				socket = NULL;
			}
			
			}
			
			/*}//end of try
			catch(std::exception &e){
				cout << "Caught exception : "<< e.what() << endl;
				cout << "Hiiiiiiiiiiiiiiiiiiiiii"<< endl;
				accept();
			}*/

		}
	private :
		boost::asio::io_context &io_context;
		boost::asio::signal_set signal;
		tcp::acceptor acceptor;
		tcp::socket socket;
		char buffer[1024];
		int new_port;
		
		void wait_for_signal(){
			signal.async_wait(
				[this] (boost::system::error_code , int ) {
					if(acceptor.is_open()){
						cout<<"In signal handler"<<endl;
						int status = 0;
						while(waitpid(-1, &status, WNOHANG) > 0);
						wait_for_signal();
					}
				}
			);
		}

		bool validate(){
			
			return true;
		}
};
int main(int argc, char *argv[]){
	if(argc != 2){
		cout << "Required arguments : ./server <port_no>"<<endl;
		return 0;
	}
	boost::asio::io_context io_context;
	server sr(io_context, atoi(argv[1]));
	cout<<"Executed normally"<<endl;
}
