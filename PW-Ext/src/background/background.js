console.log("Background script initialized");

let newWindowId = null;
let overlayTabId = null;
let socket;
let isRunning = false;
let isNewEntryWindowOpen = false;  // Add this flag
let popupIsOpened = false;

chrome.runtime.onMessage.addListener(function(request, sender, sendResponse) {
  if (request.action === 'popupOpened') {
    popupIsOpened = true;
  } else if (request.action === 'popupClosed') {
    popupIsOpened = false;
  }
});

function isPopupOpened() {
  return popupIsOpened;
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
      if (!data.entry && !isNewEntryWindowOpen) {
        isNewEntryWindowOpen = true;  // Set flag before creating window
        chrome.windows.create({
          url: chrome.runtime.getURL("src/newEntry/newEntry.html"),
          type: "popup",
          height: 147,
          width: 222,
        }, (newWindow) => {
          console.log("New entry window created:", newWindow);
          newWindowId = newWindow.id;
          chrome.windows.onRemoved.addListener(function windowClosedListener(windowId) {
            if (windowId === newWindowId) {
              isNewEntryWindowOpen = false;
              newWindowId = null;
              chrome.windows.onRemoved.removeListener(windowClosedListener);
            }
          });
          console.log("Checking if content script is ready at", new Date().toISOString());
          const checkContentScriptReady = setInterval(() => {
            chrome.tabs.query({ active: true, currentWindow: true }, (tabs) => {
              if (tabs.length > 0) {
                const newWindowTabId = tabs[0];
                console.log("Sending message to content script");
                chrome.tabs.sendMessage(newWindowTabId.id, { type: "init", data: { website: data.website } }, (response) => {
                  if (chrome.runtime.lastError) {
                    console.error("Error in chrome.tabs.sendMessage:", chrome.runtime.lastError.message);
                  }
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
        console.log("handling loggin");
        // TODO: fix handle logging into website.
        const checkContentScriptReady = setInterval(() => {
          chrome.tabs.query({ active: true, currentWindow: true }, (tabs) => {
            if (tabs.length > 0) {
              const activeTab = tabs[0];
              chrome.tabs.sendMessage(activeTab.id, { action: "fill-form", data: data.entry }, (response) => {
                if (chrome.runtime.lastError) {
                  console.error("Error in chrome.tabs.sendMessage:", chrome.runtime.lastError.message);
                }
                if (response && response.status === "success") {
                  clearInterval(checkContentScriptReady);
                  console.log("Content script is ready and received the message");
                }
              });
            }
          });
        }, 100); // Check every 100ms
      }
    }

    if (data.action === "init-response") {
      console.log("Received init response:", data);
      try {
        chrome.runtime.sendMessage({ action: "init", isOn: data.isOn });
        console.log("Sent init response to popup script:", data.isOn);
      } catch (error) {
        console.error("Caught error:", error);
      }
      isRunning = data.isOn;
    } 
    
    if (data.action === "toggle-background") {
      console.log("Received toggle request:", data);
      try {
        if (isPopupOpened()) {
          chrome.runtime.sendMessage({ action: "init", isOn: data.isOn }, (response) => {
            if (chrome.runtime.lastError) {
            console.log("Error in chrome.runtime.sendMessage:", chrome.runtime.lastError.message);
            console.log("this is because the popup is not open, this can be ignored.");
            } else if (response && response.status === "success") {
            console.log("Sent toggle request to popup script:", data.isOn);
            }
          });
        } else {
          // The popup is not open
          console.log("Popup is not open. Not sending message but saving state changes.");
        }
      } catch (error) {
      console.error("Caught error:", error);
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
        resolve(waitForSocketConnection());
      }
    });
  } catch (error) {
    console.error('WebSocket error:', error);
  }
}
function sendMessage(message) {
  try {
    waitForSocketConnection().then(() => {

        if (socket.readyState === WebSocket.OPEN) {
          socket.send(message);

        } else {
          console.log(" Error WebSocket is not open");
        }

        // socket.addEventListener("message", function handler(event) {
        //   socket.removeEventListener("message", handler);
        //   resolve(event.data);
        // });

        // socket.addEventListener("error", (event) => {
        //   reject(event);
        // });

    });
  } catch (error) {
    console.error('WebSocket error:', error);
  }
}

try {
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
    // Prevent processing if a window is already open
    if (isNewEntryWindowOpen) {
      sendResponse({ status: "window-already-open" });
      return true;
    }
    chrome.storage.local.get("isOn", (result) => {
      if (result.isOn) {
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
  if (message.action === "new-entry-data") {
    console.log("New entry data received:", message);
    socket.send(JSON.stringify({ type: "new-entry", website: message.website, username: message.username, password: message.password }));
  }
  if (message.action === "closeNewEntryWindow") {
    console.log("Close new entry window request received");
    if (newWindowId) {
      chrome.windows.remove(newWindowId, () => {
        isNewEntryWindowOpen = false;
        newWindowId = null;
        console.log("NewEntry window closed");
      });
    }
    sendResponse({ status: "success" });
    return true;
  }
  return false;
});
} catch (error) {
  console.error('WebSocket error:', error);
}
connectWebSocket();
// check for last error
if (chrome.runtime.lastError) {
  console.error("Error in chrome.runtime.sendMessage:", chrome.runtime.lastError.message);
}

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
