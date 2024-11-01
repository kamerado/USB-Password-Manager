#include "src/core/WebSocket.h"
#include "src/core/Logger.h"
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QtConcurrent/qtconcurrentrun.h>
#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <qcoreapplication.h>
#include <qfuture.h>
#include <qglobal.h>
#include <qobject.h>
#include <qtimer.h>
#include <string>

WebSocketServer::WebSocketServer(std::shared_ptr<Logger *> &logM) {
  logger = logM;
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
    (*logger)->log(ERROR, "WebSocketServer initialization error: " +
                              std::string(e.what()));
  }

  (*logger)->log(INFO, "WebSocketServer: Initialized");
}

void WebSocketServer::start(uint16_t port) {
  if (!SocketInitialized) {
    try {
      (*logger)->log(INFO, ("WebSocketServer: Starting server on port ") +
                               std::to_string(port));
      wsServer.listen(port);
      wsServer.start_accept();

      thread = std::make_shared<QFuture<void>>(
          QtConcurrent::run([this] { wsServer.run(); }));

      SocketInitialized = true;
    } catch (std::exception &e) {
      (*logger)->log(ERROR,
                     "WebSocketServer start error: " + std::string(e.what()));
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
    }
  } catch (std::exception &e) {
    (*logger)->log(ERROR,
                   "WebSocketServer stop error: " + std::string(e.what()));
  }
}

void WebSocketServer::sendEntry(websocketpp::connection_hdl &hdl,
                                server_t::message_ptr &msg,
                                std::string &message) {
  wsServer.send(hdl, message, msg->get_opcode());
}

WebSocketServer::~WebSocketServer() { stop(); }

bool WebSocketServer::isInitialized() const { return this->SocketInitialized; }

void WebSocketServer::onMessage(websocketpp::connection_hdl hdl,
                                server_t::message_ptr msg) {
  QString message = QString::fromStdString(msg->get_payload());
  (*logger)->log(DEBUG, "DEBUG: WebSocketServer message sent: " +
                            message.toStdString());
  emit messageReceived(message, hdl, msg);
}
