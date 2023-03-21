#include <iostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
using namespace std;

string get_pwd();

void send_file(string filename, tcp::socket& socket);