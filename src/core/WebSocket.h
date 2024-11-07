#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H
#include <websocketpp/roles/server_endpoint.hpp>
#pragma once

#include <QFuture>
#include <QObject>
#include <QThread> // For QThread::msleep
#include <cstdint>
#include <memory>
#include <qobject.h>
#include <qtmetamacros.h>
#include <websocketpp/common/connection_hdl.hpp>

#define ASIO_USE_TS_EXECUTOR_AS_DEFAULT
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "src/core/Logger.h"

class WebSocketServer : public QObject {
  Q_OBJECT

public:
  using server_t = websocketpp::server<websocketpp::config::asio>;
  explicit WebSocketServer(std::shared_ptr<Logger *> &logM);
  void start(uint16_t port);
  void stop();
  bool isInitialized() const;
  ~WebSocketServer();
  void toggleSocket();
  void sendEntry(websocketpp::connection_hdl hdl, server_t::message_ptr &msg,
                 std::string &message);

public slots:
  // void initialize();
  // void doMessageLoop();
  // void shutdown();

signals:
  void initialized(bool success);
  void messageReceived(const QString &message, websocketpp::connection_hdl hdl,
                       server_t::message_ptr msg);
  void sendToggleSignal();

private:
  std::shared_ptr<Logger *> logger;
  std::shared_ptr<QFuture<void>> thread;
  void onMessage(websocketpp::connection_hdl hdl, server_t::message_ptr msg);
  server_t wsServer;
  bool SocketInitialized = false;
  Q_DISABLE_COPY(WebSocketServer);
};

#endif // MESSAGETHREAD_H
