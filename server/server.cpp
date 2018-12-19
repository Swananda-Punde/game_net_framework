#include<string>
#include<iostream>
#include<sys/wait.h>
#include<boost/bind.hpp>
#include<boost/asio.hpp>
#include<boost/thread.hpp>
#include<boost/asio/io_context.hpp>
#include<boost/asio/ip/tcp.hpp>
#include<boost/asio/signal_set.hpp>
#include<boost/asio/write.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/enable_shared_from_this.hpp>

using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::to_string;
using boost::asio::ip::tcp;

class client_session
{
	public:
		typedef boost::shared_ptr<client_session> pointer;

		static pointer create(boost::asio::io_context &io, int port) {
			return pointer(new client_session(io, port));
		}

		tcp::socket& get_socket() { 
			return socket; 
		}

		void start() {
			socket.close();
			tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
			acceptor.accept(socket);
			while(1){
				read();
				cout << "Received : "<<recv<<endl;
				if(strcmp(recv,"exit") == 0)
					break;
				strcpy(send, recv);
				write();
				//boost::asio::write(socket, boost::asio::buffer( ));
			}
		}

		int read() {
			int size ;
			recv[0] = '\0';
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
			int size = socket.write_some(boost::asio::buffer(send, 1024));
			send[0] = '\0';
			return size;
		}
		int write(char *send){
			int size = socket.write_some(boost::asio::buffer(send, strlen(send)));
		//	send[0] = '\0';
			return size;
		}
		int write(string send){
			return socket.write_some(boost::asio::buffer(send, send.length()));
		}
		client_session(boost::asio::io_context &io, int port):io_context(io),socket(io), port(port) {
		}
		/*client_session(boost::asio::io_context &io, int port):socket(io), port(port) {
		}*/
		
	private:
		boost::asio::io_context& io_context;
		tcp::socket socket;
		int port;
		char recv[1024], send[1024];
};

int main(int argc, char *argv[]){
	
	if(argc != 2){
		cout << "Required arguments : ./server <port_no>"<<endl;
		return 0;
	}
	int port = atoi(argv[1]);
	boost::asio::io_context io;
	tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), port));
	acceptor.listen();
	for (;;) {
		client_session::pointer new_client = client_session::create(io,port+1);
		acceptor.accept(new_client->get_socket());
		new_client->read();
		char t[] = {'1'};
		new_client->write(t);//Authentication
		new_client->write(to_string(port+1));
		boost::thread(boost::bind(&client_session::start, new_client)).detach();
		port++;
	}
}
