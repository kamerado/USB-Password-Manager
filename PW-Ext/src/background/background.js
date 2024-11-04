"use strict";

console.log("Background script initialized");

let socket;
let isRunning = false;

function connectWebSocket() {
  console.log("Attempting WebSocket connection...");
  socket = new WebSocket("ws://localhost:8080");

  socket.addEventListener("open", () => {
    console.log("WebSocket connection opened");
  });

  socket.addEventListener("message", (event) => {
    console.log("Message from server:", event.data);
    const data = JSON.parse(event.data);

    if (data.action === "receive-null-entry") {
      console.log(
        "Received entry action",
        data.entry ? "with entry" : "without entry"
      );
      if (!data.entry) {
        console.log("handler new credentials");
        // TODO: handle creating new login credentials.
        chrome.windows.create({
          url: chrome.runtime.getURL("src/newEntry/newEntry.html"),
          type: "popup",
          height: 147,
          width: 222
        });
      } else {
        console.log("handler loggin");
        // TODO: handle logging into website.
      }
    }
  });

  socket.addEventListener("close", () => {
    console.log("WebSocket connection closed. Reconnecting...");
    setTimeout(connectWebSocket, 1000);
  });

  socket.addEventListener("error", (event) => {
    console.log("WebSocket error:", event);
  });
}

connectWebSocket();

chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
  console.log("Background received message:", message);
  if (message.action === "toggleBackground") {
    isRunning = message.status;
    if (isRunning) {
      console.log("Starting background function...");
      if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ type: "status", status: true }));
        console.log("Sent status to WebSocket:", isRunning);
      } else {
        console.warn("WebSocket is not open. Cannot send message.");
      }
    } else {
      console.log("Stopping background function...");
      if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ type: "status", status: false }));
        console.log("Sent stop status to WebSocket");
      }
    }
    sendResponse({ status: "background function state changed", running: isRunning });
  }
  if (message.action === 'loginFormDetected') {
    chrome.storage.local.get("isOn", (result) => {
      if (result) {
        console.log("Login form retected being processed");
        if (socket && socket.readyState === WebSocket.OPEN) {
          socket.send(JSON.stringify({
            type: "check-entry",
            website: message.request.website
          }));
          console.log("Sent request to WebSocket:", message.request);
        } else {
          console.warn("WebSocket is not open. Cannot send message.");
        }
        console.log("Sending check entry response.")
        sendResponse({
          status: "Sent check entry", entry: JSON.stringify({
            type: "check-entry",
            website: message.request.website
          })
        });
      } else {
        console.log("Stopping background function...");
        if (socket.readyState === WebSocket.OPEN) {
          socket.send(JSON.stringify({ type: "status", status: false }));
          console.log("Sent stop status to WebSocket");
        }
        sendResponse({
          status: "Sent check entry", entry: JSON.stringify({
            type: "check-entry",
            website: message.request.website
          })
        });
      }
    });

    return true;
  }
  if (message.action === "get-default-username") {
    if (socket.readyState === WebS.OPEN) {
      socket.send(JSON.stringify({}))
    }
  }
  return false;
});

function getHostname(callback) {
  chrome.tabs.query({ currentWindow: true, active: true }, (tabs) => {
    if (tabs[0] && tabs[0].url) {
      const hostname = new URL(tabs[0].url).hostname;
      console.log("Retrieved hostname:", hostname);
      callback(hostname);
    } else {
      console.warn("No active tab found or URL is undefined");
    }
  });
}
