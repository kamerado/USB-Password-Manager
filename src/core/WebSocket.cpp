#include "src/core/WebSocket.h"
#include "src/gui/GUI/mainwindow.h"
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QtConcurrent/qtconcurrentrun.h>
#include <cstdint>
#include <exception>
#include <iostream>
#include <qcoreapplication.h>
#include <qglobal.h>
#include <qobject.h>
#include <qtimer.h>
#include <websocketpp/common/connection_hdl.hpp>

WebSocketServer::WebSocketServer(QObject *parent) : QObject(parent) {
  try {
    wsServer.clear_access_channels(websocketpp::log::alevel::all);
    wsServer.clear_error_channels(websocketpp::log::alevel::all);

    wsServer.init_asio();
    wsServer.set_reuse_addr(true);

    wsServer.set_message_handler(
        [this](websocketpp::connection_hdl hdl, server_t::message_ptr msg) {
          onMessage(hdl, msg);
        });
  } catch (const std::exception &e) {
    std::cerr << "WebSocketServer initialization error: " << e.what()
              << std::endl;
  }

  std::cout << "WebSocketServer: Initialized" << std::endl;
}

void WebSocketServer::start(uint16_t port) {
  if (!SocketInitialized) {
    try {
      std::cout << "WebSocketServer: Starting server on port " << port
                << std::endl;
      wsServer.listen(port);
      wsServer.start_accept();

      QtConcurrent::run([this] { wsServer.run(); });

      SocketInitialized = true;
    } catch (std::exception &e) {
      std::cerr << "WebSocketServer start error: " << e.what() << std::endl;
    }
  }
}

void WebSocketServer::stop() {
  try {
    if (!wsServer.stopped()) {
      std::cout << "WebSocketServer: Stopping server..." << std::endl;
      wsServer.stop_listening();
      wsServer.stop();
      SocketInitialized = false;
      // emit sendToggleSignal();
    }
  } catch (std::exception &e) {
    std::cerr << "WebSocketServer stop error: " << e.what() << std::endl;
  }
}

WebSocketServer::~WebSocketServer() { stop(); }

bool WebSocketServer::isInitialized() const { return this->SocketInitialized; }

void WebSocketServer::onMessage(websocketpp::connection_hdl hdl,
                                server_t::message_ptr msg) {
  QString message = QString::fromStdString(msg->get_payload());
  std::cout << message.toStdString() << std::endl;
  emit messageReceived(message);
}

// void WebSocketServer::receiveToggleSignal() {
//   // TODO: handle receiving toggle signal and sending info to extension.
//   std::cout << "WebSocketServer: Received toggle signal from MainWindow."
//             << std::endl;
//   toggleSocket();
// }
