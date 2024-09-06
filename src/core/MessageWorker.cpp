#include "../../src/core/MessageWorker.h"
#include <iostream>
#include <QDebug>
#include <future>
#include <thread>

NativeMessagingWorker::NativeMessagingWorker(QObject *parent) : QObject(parent) {
    // setmode(fileno(stdin),O_BINARY);
    // auto future = std::async(std::launch::async, this->run);
}

void NativeMessagingWorker::run() {
    std::cout << "Starting message server." << std::endl;
    // Infinite loop to read and write messages
    while (true) {
        // Simulate reading a message from stdin
        char* inputMessage = readMessage();

        // Emit signal with the received message
        emit messageReceived(QString::fromStdString(inputMessage));

        // Simulate sending a response back to the browser
        std::string response = R"({"response": "Message received!"})";
        writeMessage(response);
    }
}

// Helper function to read a message from stdin
char* NativeMessagingWorker::readMessage() {
    char* message;

    // read length header.
    uint32_t reqLen = 0;
    std::cin.read(reinterpret_cast<char*>(&reqLen) , 4);
    std::cin.read(reinterpret_cast<char*>(&message), reqLen);

    return message;
}

// Helper function to write a message to stdout
void NativeMessagingWorker::writeMessage(const std::string &message) {
    uint32_t messageLength = message.size();
    std::cout.write(reinterpret_cast<char*>(&messageLength), sizeof(messageLength));
    std::cout.write(message.data(), messageLength);
    std::cout.flush();
}