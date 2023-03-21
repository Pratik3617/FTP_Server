#include <iostream>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

bool authenticate_client(tcp::socket& socket, string username,string password) {
    // Send username and password to server for authentication
    string message = "USER " + username + "\r\n";
    boost::asio::write(socket, boost::asio::buffer(message));
    getline(std::cin, message);
    message = "PASS " + password + "\r\n";
    boost::asio::write(socket, boost::asio::buffer(message));

    // Wait for server response
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");
    istream response_stream(&response);
    string status;
    response_stream >> status;
    return (status == "230");
}



int main(int argc, char* argv[]) {
  try{
        if(argc != 3){
          cerr<<"Requirement: ftp_client <host> <port>"<<endl;
          return 1;
        }

        // all IO happens inside a context
        boost::asio::io_context io_context;

        // Converting hostname port to endpoint address(es)
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);

        // create a socket
        tcp::socket socket(io_context);

        // connect to server
        boost::asio::connect(socket, endpoints);

        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (authenticate_client(socket, username, password)) {
                
              cout << "Client Authenticated !!!.\n";
              while (true) {
                    cout<<"FTP server >>>> ";
                    string command;
                    getline(cin, command);

                    boost::asio::write(socket, boost::asio::buffer(command + "\n"));

                    boost::asio::streambuf buffer;
                    boost::asio::read_until(socket, buffer, "\n");

                    istream input_stream(&buffer);
                    string response;
                    getline(input_stream, response);

                    cout << response << endl;

                    if (command == "QUIT") {
                      break;
                    }else if(command == "PORT"){
                      continue;
                    }else if(command == "LIST"){
                      continue;
                    }else{
                      cout<<"Invalid Command!"<<endl;
                      continue;
                    }
              }
        }else {
            cout << "Authentication failed\n";
        }
  }catch (exception& e)
  {
    cerr << "Exception in main: " << e.what() << endl;
  }

  return 0;
}

