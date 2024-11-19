#include "src/core/WebSocket.h"
#include "src/core/Logger.h"
#include <QCoreApplication>
#include <QString>
#include <QTimer>
#include <QtConcurrent/qtconcurrentrun.h>
#include <exception>
#include <memory>
#include <qcoreapplication.h>
#include <qfuture.h>
#include <qglobal.h>
#include <qobject.h>
#include <qtimer.h>
#include <qwebsocket.h>
#include <string>

WebSocketServer::WebSocketServer(std::shared_ptr<Logger> &logM) : logger(logM) {
  try {
    wsServer = std::make_unique<QWebSocketServer>(
        "WSServer", QWebSocketServer::NonSecureMode);
    if (wsServer && wsServer->listen(QHostAddress::Any, 8080)) {
      connect(wsServer.get(), &QWebSocketServer::newConnection, this,
              &WebSocketServer::onNewConnection);
      LOG_INFO(logger, "Initialized and listening on port 8080");
    } else {
      QString errorMsg = wsServer ? wsServer->errorString()
                                  : "Failed to create QWebSocketServer";
      LOG_ERROR(logger, "Error starting server: {}", errorMsg.toStdString());
    }
  } catch (const std::exception &e) {
    LOG_ERROR(logger, "WebSocketServer initialization error: {}", e.what());
  }
}

void WebSocketServer::onNewConnection() {
  QWebSocket *socket = wsServer->nextPendingConnection();
  clients.push_back(socket);

  connect(socket, &QWebSocket::textMessageReceived, this,
          &WebSocketServer::onTextMessageReceived);
  connect(socket, &QWebSocket::disconnected, this,
          &WebSocketServer::onDisconnected);

  LOG_DEBUG(logger, "New connection established");
}

void WebSocketServer::onTextMessageReceived(const QString &message) {
  QWebSocket *client = qobject_cast<QWebSocket *>(sender());
  LOG_INFO(logger, "Message received: {}", message.toStdString());
  emit messageReceived(message, client);

  // TODO: Process the message and send a response if needed
  // socket->sendTextMessage("Echo: " + message);
}

void WebSocketServer::onDisconnected() {
  QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
  clients.erase(std::remove(clients.begin(), clients.end(), socket),
                clients.end());
  LOG_DEBUG(logger, "Client disconnected");
  // TODO: stop background service if no clients are connected.
  socket->deleteLater();
}

/*
  Handle start and stop of the WebSocket server if nessicary.
*/
void WebSocketServer::start(uint16_t port) {}

void WebSocketServer::stop() {}

void WebSocketServer::sendMessage(const QWebSocket *client, std::string &data) {
  QString qMessage = QString::fromStdString(data);
  QWebSocket *nonConstClient = const_cast<QWebSocket *>(client);
  if (client->isValid()) {
    nonConstClient->sendTextMessage(qMessage);
  }
  LOG_DEBUG(logger, "WebSocketServer: Sent message to all clients: {}", data);
}

WebSocketServer::~WebSocketServer() {}

bool WebSocketServer::isInitialized() const { return this->SocketInitialized; }
