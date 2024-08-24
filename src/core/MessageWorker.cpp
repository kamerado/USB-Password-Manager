#include "../../src/core/MessageWorker.h"
#include <iostream>
#include <QDebug>

NativeMessagingWorker::NativeMessagingWorker(QObject *parent) : QObject(parent) {
    // Constructor code (if needed)
}

void NativeMessagingWorker::run() {
    // Infinite loop to read and write messages
    while (true) {
        // Simulate reading a message from stdin
        std::string inputMessage = readMessage();

        // Emit signal with the received message
        emit messageReceived(QString::fromStdString(inputMessage));

        // Simulate sending a response back to the browser
        std::string response = R"({"response": "Message received!"})";
        writeMessage(response);
    }
}

// Helper function to read a message from stdin
std::string NativeMessagingWorker::readMessage() {
    uint32_t messageLength;
    std::cin.read(reinterpret_cast<char*>(&messageLength), sizeof(messageLength));

    std::string message(messageLength, 0);
    std::cin.read(&message[0], messageLength);

    return message;
}

// Helper function to write a message to stdout
void NativeMessagingWorker::writeMessage(const std::string &message) {
    uint32_t messageLength = message.size();
    std::cout.write(reinterpret_cast<char*>(&messageLength), sizeof(messageLength));
    std::cout.write(message.data(), messageLength);
    std::cout.flush();
}