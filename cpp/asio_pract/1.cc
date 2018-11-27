#include <iostream>
#include <boost/asio.hpp>
#include <functional>
#include <ctime>
#include <string>

using namespace std;
namespace bn = boost::asio;
using namespace bn::ip;
int main()
{
  try {
    bn::io_service io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));

    for (;;) {
      tcp::socket socket(io);
      acceptor.accept(socket);

      std::string message = [] {
        time_t now = time(nullptr);
        return ctime(&now);        
      } ();

      boost::system::error_code ie;
      bn::write(socket, bn::buffer(message), ie);
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}