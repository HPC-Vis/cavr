#pragma once
#include <string>

namespace cavr {

namespace com {
#include <zmq.h>

class Socket {
public:
  Socket(void* socket);
  bool send(const std::string& data);
  bool recv(std::string& data);
  ~Socket();
private:
  void* zmq_socket_;
};

class Communications {
public:
  static bool initialize();
  static Socket* createRequester(const std::string& address, int port);
  static Socket* createReplier(int port);
  static Socket* createSubscriber(const std::string& address, int port);
  static Socket* createPublisher(int port);
private:
  struct Data {
    Data();
    void* zmq_handle;
  };
  static Data data_;
};

} // namespace com

} // namespace cavr
