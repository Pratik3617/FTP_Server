#include <iostream>
#include <string>
#include <boost/asio.hpp>


using namespace std;
using boost::asio::ip::tcp;

// Authentication function for server
bool authenticate_server(tcp::socket& socket, string username, string password) {

    // Read client username and password
    boost::asio::streambuf request;
    boost::asio::read_until(socket, request, "\r\n");
    istream request_stream(&request);
    string command, client_username, client_password;
    request_stream >> command >> client_username >> client_password;


    // Check if username and password are valid
    if (command != "USER" || client_username != username) {
        boost::asio::write(socket, boost::asio::buffer("530 Login incorrect.\r\n"));
        return false;
    } else if (command == "PASS" && client_password != password) {
        boost::asio::write(socket, boost::asio::buffer("530 Login incorrect.\r\n"));
        return false;
    } else {
        boost::asio::write(socket, boost::asio::buffer("230 Logged on.\r\n"));
        return true;
    }
}


void session(tcp::socket socket)
{
  try
  {
    while (true)
    {
      boost::asio::streambuf buffer;
      boost::asio::read_until(socket, buffer, "\n");

      string command;
      istream input_stream(&buffer);
      getline(input_stream, command);

      if (command == "QUIT")
      {
        boost::asio::write(socket, boost::asio::buffer("Connection closed!!\n"));
        break;
      }else if(command == "PORT"){
        continue;
      }
      else if (command == "LIST")
      {
        boost::asio::write(socket, boost::asio::buffer("Here are the files:\n"));
        // TODO: list files in directory
        continue;
      }
      else
      {
        boost::asio::write(socket, boost::asio::buffer("Invalid command!!!\n"));
        continue;
        
      }
    }
  }
  catch (exception& e)
  {
    cerr << "Exception in session: " << e.what() << endl;
  }
}



int main() {
    try{
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));
        
        string username = "ftp_server";
        string password = "1234";


        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            if (authenticate_server(socket, username, password)) {
                thread(session, move(socket)).detach();
            } else{
            cout<<"Authentication Failed!"<<endl;
            }  
    }
  }catch (exception& e){
    cerr << "Exception in main: " << e.what() << endl;
  }
  return 0;
}
