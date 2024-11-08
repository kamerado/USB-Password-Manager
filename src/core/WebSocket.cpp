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
#include <string>

WebSocketServer::WebSocketServer(std::shared_ptr<Logger> &logM) : logger(logM){
    try {
        wsServer = std::make_unique<QWebSocketServer>("WSServer", QWebSocketServer::NonSecureMode);
        if (wsServer && wsServer->listen(QHostAddress::Any, 8080)) {
            qDebug() << "Server listening on port 8080";
            connect(wsServer.get(), &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
            logger->log(INFO, "WebSocketServer: Initialized and listening on port 8080");
        } else {
            QString errorMsg = wsServer ? wsServer->errorString() : "Failed to create QWebSocketServer";
            qDebug() << "Error starting server:" << errorMsg;
            logger->log(ERROR, "Error starting server: " + errorMsg.toStdString());
        }
    } catch (const std::exception &e) {
        logger->log(ERROR, "WebSocketServer initialization error: " + std::string(e.what()));
    }
}

void WebSocketServer::onNewConnection() {
    QWebSocket *socket = wsServer->nextPendingConnection();

    connect(socket, &QWebSocket::textMessageReceived, this, &WebSocketServer::onTextMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketServer::onDisconnected);

    qDebug() << "New connection established";
}

void WebSocketServer::onTextMessageReceived(const QString &message) {
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());

    qDebug() << "Message received:" << message;
    emit messageReceived(message);

    // TODO: Process the message and send a response if needed
    // socket->sendTextMessage("Echo: " + message);
}

void WebSocketServer::onDisconnected() {
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Client disconnected";
    socket->deleteLater();
}

void WebSocketServer::start(uint16_t port) {
  // if (!SocketInitialized) {
  //   try {
  //     (*logger)->log(INFO, ("WebSocketServer: Starting server on port ") +
  //                              std::to_string(port));
  //     wsServer.listen(port);
  //     wsServer.start_accept();

  //     thread = std::make_shared<QFuture<void>>(
  //         QtConcurrent::run([this] { wsServer.run(); }));

  //     SocketInitialized = true;
  //   } catch (std::exception &e) {
  //     (*logger)->log(ERROR,
  //                    "WebSocketServer start error: " + std::string(e.what()));
  //   }
  // }
}

void WebSocketServer::stop() {
  // try {
  //   if (!wsServer.stopped()) {
  //     std::cout << "WebSocketServer: Stopping server..." << std::endl;
  //     wsServer.stop_listening();
  //     wsServer.stop();
  //     SocketInitialized = false;
  //   }
  // } catch (std::exception &e) {
  //   (*logger)->log(ERROR,
  //                  "WebSocketServer stop error: " + std::string(e.what()));
  // }
}

void WebSocketServer::sendEntry(std::string &message) {
  // wsServer.send();
}

WebSocketServer::~WebSocketServer() {}

bool WebSocketServer::isInitialized() const { return this->SocketInitialized; }

void WebSocketServer::onMessage() {
  // QString message = QString::fromStdString();
  // (*logger)->log(DEBUG, "DEBUG: WebSocketServer message sent: " +
                            // message.toStdString());
  // emit messageReceived(message);
}
