"use strict";

console.log("Background script initialized");

let newWindowId = null;
let overlayTabId = null;
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
      console.log(
        "Received entry action for website: ", data.website,
        "\nEntry: ", data.entry
      );
      if (!data.entry) {
        console.log("handler new credentials");
        // TODO: handle creating new login credentials.
        chrome.windows.create({
          url: chrome.runtime.getURL("src/newEntry/newEntry.html"),
          type: "popup",
          height: 147,
          width: 222,
        }, (newWindow) => {
          newWindowId = newWindow.id;
          // Polling mechanism to check if the content script is ready
          const checkContentScriptReady = setInterval(() => {
            chrome.tabs.query({ windowId: newWindowId }, (tabs) => {
              if (tabs.length > 0) {
                const newWindowTabId = tabs[0].id;
                chrome.tabs.sendMessage(newWindowTabId, { type: "init", data: { website: data.website } }, (response) => {
                  if (response && response.status === "success") {
                    clearInterval(checkContentScriptReady);
                    console.log("Content script is ready and received the message");
                  }
                });
              }
            });
          }, 100); // Check every 100ms
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
    sendResponse({
      status: "background function state changed",
      running: isRunning,
    });
  }
  if (message.action === "loginFormDetected") {
    chrome.storage.local.get("isOn", (result) => {
      if (result) {
        console.log("Login form detected being processed");
        if (socket && socket.readyState === WebSocket.OPEN) {
          const requestPayload = {
            type: "check-entry",
            website: message.request.website
          };

          socket.send(
            JSON.stringify(requestPayload)
          );
          console.log("Sent request to WebSocket:", message.request);
          const waitForWebSocketResponse = new Promise((resolve, reject) => {
            const handleMessage = (event) => {
              const data = JSON.parse(event.data);
              // if (data.type === "" && data.website === message.request.website) {
              //   socket.removeEventListener("message", handleMessage);
              //   resolve(data);
              // }
              if (data.action === "receive-entry") {
                console.log(
                  "Received entry action for website: ", data.website,
                  "\nEntry: ", data.entry
                );
                if (!data.entry) {
                  console.log("handler new credentials");
                  // TODO: handle creating new login credentials.
                  chrome.windows.create({
                    url: chrome.runtime.getURL("src/newEntry/newEntry.html"),
                    type: "popup",
                    height: 147,
                    width: 222,
                  }, (newWindow) => {
                    newWindowId = newWindow.id;
                    // Assuming you have the tab ID of the new window's tab
                    // chrome.tabs.sendMessage(newWindowTabId, {
                    //   type: "init",
                    //   data: { website: data.website }
                    // });
                    chrome.tabs.query({ active: true, currentWindow: true }, (tabs) => {
                      if (tabs.length > 0) {
                        overlayTabId = tabs[0].id;
                        // Inject the overlay content script
                        if (tabs[0].url?.startsWith("chrome://")) return undefined;
                        chrome.scripting.executeScript({
                          target: { tabId: overlayTabId },
                          files: ["src/overlay.js"]
                        });
                      };
                    });
                  });
                } else {
                  console.log("handler loggin");
                  // TODO: handle logging into website.
                }
              }
            };
          });
          sendResponse({
            status: "success",
            website: message.request.website,
          });
        } else {
          console.warn("WebSocket is not open. Cannot send message.");
          sendResponse({
            status: "error",
            message: "WebSocket is not open",
          });
        }
      } else {
        console.log("Stopping background function...");
        if (socket.readyState === WebSocket.OPEN) {
          socket.send(JSON.stringify({ type: "status", status: false }));
          console.log("Sent stop status to WebSocket");
        }
        // TODO: handle new entry or login.
        sendResponse({
          status: "success",
          entry: null, // Assuming no entry found initially
          website: message.request.website,
        });
      }
    });

    return true;
  }
  if (message.action === "get-default-username") {
    if (socket.readyState === WebSocket.OPEN) {
      // TODO: Send message to server to get default username.
      socket.send(JSON.stringify({}));
      socket.addEventListener("message", (event) => {
        const data = JSON.parse(event.data);
        if (data.action === "default-username") {
          sendResponse({ username: data.username });
        }
      });
    }
    return true;
  }
  if (message.action === "new-entry-data") {
    const username = message.username;
    const password = message.password;
    const website = message.website;
    console.log("Received new entry data:", username, password);

    // Handle the new entry data (e.g., send to server, save to storage, etc.)
    // Example: Send to WebSocket server
    socket.send(JSON.stringify({ type: "new-entry-data", website, username, password }));

    // Close the popup window and remove the overlay
    if (newWindowId !== null) {
      chrome.windows.remove(newWindowId);
      newWindowId = null;
    }
    if (overlayTabId !== null) {
      chrome.tabs.get(overlayTabId, (tab) => {
        if (chrome.runtime.lastError || !tab) {
          console.warn("Tab no longer exists:", overlayTabId);
          overlayTabId = null;
        } else {
          chrome.scripting.executeScript({
            target: { tabId: overlayTabId },
            func: () => document.getElementById('overlay').remove()
          });
        }
      });
    }
  }
  return false;
});

connectWebSocket();

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
