#include <cavr/com/communications.h>
#include <glog/logging.h>
#include <zmq.h>

namespace cavr {

namespace com {

Socket::Socket(void* socket)
  : zmq_socket_(socket) {
}

bool Socket::send(const std::string& data) {
  zmq_msg_t msg;
  if (0 != zmq_msg_init_size(&msg, data.length())) {
    LOG(ERROR) << "Could not initialize ZMQ message";
    return false;
  }
  std::copy(data.begin(), data.end(), (char*)zmq_msg_data(&msg));
  bool result = 0 == zmq_send(zmq_socket_, &msg, 0);
  if (!result) {
    LOG(ERROR) << "zmq_send failed";
  }
  zmq_msg_close(&msg);
  return result;
}

bool Socket::recv(std::string& data) {
  zmq_msg_t msg;
  if (0 != zmq_msg_init(&msg)) {
    LOG(ERROR) << "Could not initialize ZMQ message";
    return false;
  }
  bool result = 0 == zmq_recv(zmq_socket_, &msg, 0);
  if (result) {
    data = std::string((char*)zmq_msg_data(&msg), zmq_msg_size(&msg));
  } else {
    LOG(ERROR) << "zmq_recv failed";
  }
  zmq_msg_close(&msg);
  return result;
}

Socket::~Socket() {
  zmq_close(zmq_socket_);
}

bool Communications::initialize() {
  if (!data_.zmq_handle) {
    data_.zmq_handle = zmq_init(1);
  }
  return nullptr != data_.zmq_handle;
}

Socket* Communications::createRequester(const std::string& address, int port) {
  void* socket = zmq_socket(data_.zmq_handle, ZMQ_REQ);
  if (!socket) {
    LOG(ERROR) << "Failed to create ZMQ socket" << zmq_strerror(errno);
    return nullptr;
  }
  std::string endpoint = "tcp://" + address + ":" + std::to_string(port);
  if (0 != zmq_connect(socket, endpoint.c_str())) {
    LOG(ERROR) << "Failed to create requester: " << zmq_strerror(errno);
    zmq_close(socket);
    return nullptr;
  }
  return new Socket(socket);
}

Socket* Communications::createReplier(int port) {
  void* socket = zmq_socket(data_.zmq_handle, ZMQ_REP);
  if (!socket) {
    LOG(ERROR) << "Failed to create ZMQ socket" << zmq_strerror(errno);
    return nullptr;
  }
  std::string bindpoint = "tcp://*:" + std::to_string(port);
  if (0 != zmq_bind(socket, bindpoint.c_str())) {
    LOG(ERROR) << "Failed to bind replier: " << zmq_strerror(errno);
    LOG(ERROR) << "bindpoint: " << bindpoint;
    zmq_close(socket);
    return nullptr;
  }
  return new Socket(socket);
}

Socket* Communications::createSubscriber(const std::string& address, int port) {
  void* socket = zmq_socket(data_.zmq_handle, ZMQ_SUB);
  if (!socket) {
    LOG(ERROR) << "Failed to create ZMQ socket" << zmq_strerror(errno);
    return nullptr;
  }
  std::string endpoint = "tcp://" + address + ":" + std::to_string(port);
  if (0 != zmq_connect(socket, endpoint.c_str())) {
    LOG(ERROR) << "Failed to create subscriber: " << zmq_strerror(errno);
    zmq_close(socket);
    return nullptr;
  }
  zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "", 0);
  return new Socket(socket);
}

Socket* Communications::createPublisher(int port) {
  void* socket = zmq_socket(data_.zmq_handle, ZMQ_PUB);
  if (!socket) {
    LOG(ERROR) << "Failed to create ZMQ socket" << zmq_strerror(errno);
    return nullptr;
  }
  std::string bindpoint = "tcp://*:" + std::to_string(port);
  if (0 != zmq_bind(socket, bindpoint.c_str())) {
    LOG(ERROR) << "Failed to bind publisher: " << zmq_strerror(errno);
    LOG(ERROR) << "bindpoint: " << bindpoint;
    zmq_close(socket);
    return nullptr;
  }
  return new Socket(socket);
}

Communications::Data Communications::data_;

Communications::Data::Data()
  : zmq_handle(nullptr) {
}

} // namespace com

} // namespace cavr
