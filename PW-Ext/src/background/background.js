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

    if (data.action === "receive-entry") {
      console.log("Received entry action", data.entry ? "with entry" : "without entry");
      if (data.entry === null) {
        // TODO: handle creating new login credentials.
      } else {
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
    if (_start.textContent === "ON") {
      console.log("Starting background function...");
      if (socket && socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({
          type: "check-entry",
          website: message.request.website
        }));
        console.log("Sent request to WebSocket:", message.request);
      } else {
        console.warn("WebSocket is not open. Cannot send message.");
      }
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

chrome.tabs.onUpdated.addListener((tabId, changeInfo, tab) => {
  console.log(`Tab ${tabId} updated. Status:`, changeInfo.status);
  if (changeInfo.status === "complete" && tab.active) {
    console.log("Injecting content script into tab:", tabId);
    chrome.scripting.executeScript({
      target: { tabId: tabId },
      files: ["src/content_script/content_script.js"]
    })
      .then(() => console.log("Content script injection successful"))
      .catch((error) => console.error("Error injecting content script:", error));
  }
});
