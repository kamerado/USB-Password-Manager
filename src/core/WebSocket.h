#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H
#pragma once
#include <QFuture>
#include <QObject>
#include <QThread> // For QThread::msleep
#include <QtWebSockets>
#include <memory>
#include <qobject.h>
#include <qt6/QtCore/QFuture>
#include <qtmetamacros.h>

#include "src/core/Logger.h"

class WebSocketServer : public QObject {
  Q_OBJECT

public:
  /*using server_t = websocketpp::server<websocketpp::config::asio>;*/
  explicit WebSocketServer(std::shared_ptr<Logger> &logM);
  void start(uint16_t port);
  void stop();
  bool isInitialized() const;
  ~WebSocketServer();
  void toggleSocket();
  void sendEntry(std::string &message);

private slots:
  void onNewConnection();
  void onTextMessageReceived(const QString &message);
  void onDisconnected();
  // void initialize();
  // void doMessageLoop();
  // void shutdown();

signals:
  void initialized(bool success);
  void messageReceived(const QString &message);
  void sendToggleSignal();

private:
  std::shared_ptr<Logger> logger;
  std::shared_ptr<QFuture<void>> thread;
  void onMessage();
  std::unique_ptr<QWebSocketServer> wsServer;
  bool SocketInitialized = false;
  Q_DISABLE_COPY(WebSocketServer);
};

#endif // MESSAGETHREAD_H
