void send_file(string filename, tcp::socket& socket)
{
    ifstream file(filename, ios::binary);
    char buffer[1024];
    while (file.read(buffer, 1024)) {
        boost::asio::write(socket, boost::asio::buffer(buffer, 1024));
    }
    boost::asio::write(socket, boost::asio::buffer(buffer, file.gcount()));
    file.close();
}