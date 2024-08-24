#ifndef NATIVEMESSAGINGWORKER_H
#define NATIVEMESSAGINGWORKER_H

#include <QObject>
#include <QString>
#include <string>

class NativeMessagingWorker : public QObject {
    Q_OBJECT

public:
    explicit NativeMessagingWorker(QObject *parent = nullptr);

signals:
    void messageReceived(const QString &message);  // Signal to send data back to the main thread

public slots:
    void run();  // Slot to start the worker thread

private:
    // Helper functions to manage communication with the native messaging protocol
    std::string readMessage();
    void writeMessage(const std::string &message);
};

#endif // NATIVEMESSAGINGWORKER_H