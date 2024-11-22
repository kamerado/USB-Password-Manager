console.log("Background script initialized");

let newWindowId = null;
let overlayTabId = null;
let socket;
let isRunning = false;

function isPopupOpen() {
  const contexts = chrome.runtime.getContexts({ contextTypes: ['POPUP'] });
  if (contexts.length > 0) {
    console.log("Popup is open");
  } else {
    console.log("Popup is not open");
  }
  return contexts.length > 0;
}
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
    if (data.action === "init-response") {
      console.log("Received init response:", data);
      chrome.runtime.sendMessage({ action: "init", isOn: data.isOn });
      isRunning = data.isOn;
    } if (data.action === "toggle-background") {
      console.log("Received toggle background:", data);
      if (isPopupOpen()) {
        chrome.runtime.sendMessage({ action: "init", isOn: data.isOn });
      }
      isRunning = data.isOn;
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
async function waitForSocketConnection() {
  try {
    return new Promise((resolve, reject) => {
      if (socket.readyState === WebSocket.OPEN) {
        resolve();
      } else {
        socket.addEventListener("open", resolve);
        socket.addEventListener("error", reject);
      }
    });
  } catch (error) {
    console.error('WebSocket error:', error);
  }
}
function sendMessage(message) {
  try {
    return waitForSocketConnection().then(() => {
      return new Promise((resolve, reject) => {
        if (socket.readyState === WebSocket.OPEN) {
          socket.send(message);
        } else {
          reject(new Error("WebSocket is not open"));
        }

        socket.addEventListener("message", function handler(event) {
          socket.removeEventListener("message", handler);
          resolve(event.data);
        });

        socket.addEventListener("error", (event) => {
          reject(event);
        });
      });
    });
  } catch (error) {
    console.error('WebSocket error:', error);
  }
}

try {
  connectWebSocket();
} catch (error) {
  console.error('WebSocket error:', error);
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
    }
  }
  if (message.action === "init") {
    console.log("Init request from popup script", message.data);
    socket.send(JSON.stringify({ type: "init" }));
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
