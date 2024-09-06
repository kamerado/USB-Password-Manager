var port = chrome.runtime.connectNative("com.kamerado.native_app");

// Listen for messages from the content script
chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
  // Check if the action is "sendToNativeApp"
  console.log("before if statement.")
  if (message.action === "sendToNativeApp") {
    console.log("Message from content script: ", message.data);

    // Connect to the native app
    // const port = chrome.runtime.connectNative("com.kamerado.native_app");

    // Send the message to the native app
    port.postMessage(message.data);

    // Listen for a response from the native app
    port.onMessage.addListener((response) => {
      console.log("Response from native app: ", response);
      // Send the response back to the content script
      // sendResponse(response);
    });
    // Handle disconnection
    port.onDisconnect.addListener(() => {
      const lastError = chrome.runtime.lastError;
      if (lastError) {
        console.error("Disconneted for native app with error: ", lastError.message);
      } else {
        console.error("Disconnected from native app without an error.");
      }
      sendResponse({ error: "Failed to communicate with native app" });
    });
    // Return true to indicate that we will respond asynchronously
    return true;
  }
});



// THIS IS THE IMPROVED ERROR HANDLING
