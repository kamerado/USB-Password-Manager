#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H
#include <websocketpp/roles/server_endpoint.hpp>
#pragma once

#include <QObject>
#include <QThread> // For QThread::msleep
#include <cstdint>
#include <qobject.h>
#include <qtmetamacros.h>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
class WebSocketServer : public QObject {
  Q_OBJECT

public:
  using server_t = websocketpp::server<websocketpp::config::asio>;
  explicit WebSocketServer(QObject *parent = nullptr);
  void start(uint16_t port);
  void stop();
  bool isInitialized() const;
  ~WebSocketServer();
  void toggleSocket();

public slots:
  // void initialize();
  // void doMessageLoop();
  // void shutdown();

signals:
  void initialized(bool success);
  void messageReceived(const QString &message);
  void sendToggleSignal();

private:
  void onMessage(websocketpp::connection_hdl hdl, server_t::message_ptr msg);
  server_t wsServer;
  bool SocketInitialized = false;
  Q_DISABLE_COPY(WebSocketServer);
};

#endif // MESSAGETHREAD_H
