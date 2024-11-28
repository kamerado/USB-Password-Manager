var h1 = document.getElementById("h1");
const _start = document.getElementById("_start");

function updateStartButton(isOn) {
  // Apply the correct class based on the new state
  if (isOn) {
    // chrome.storage.local.set({ isOn }, () => { // Save new state
    //   console.log("New state saved:", request.isOn);
    //   updateStartButton(request.isOn);
    // });
    console.log("Switching to ON state");
    _start.classList.add("toggle_on");
    _start.classList.remove("toggle_off");
    _start.textContent = "ON";
  } else if (_start != null) {
    console.log("Switching to OFF state");
    _start.classList.add("toggle_off");
    _start.classList.remove("toggle_on");
    _start.textContent = "OFF";
  }
};

function initButton() {
  console.log("initButton...");
  // TODO: setup message handler for background script.
  // Listen for messages from the background script
  chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
    if (request.action === "toggle") {
      console.log("Toggle request from background script", request);
      updateStartButton(request.data.isOn);
    }
    if (request.action === "init") {
      console.log("Init request from background script", request);
      var isOn = request.isOn || false;
      chrome.storage.local.set({ isOn }, () => { // Save new state
        console.log("New state saved:", request.isOn);
        updateStartButton(request.isOn);
      });
    }
  });

  // TODO: handle initial check state.
  chrome.runtime.sendMessage({ action: "init" });
};


// Function to toggle button state on click
function toggle_start() {
  console.log("Toggling button state...");
  chrome.storage.local.get("isOn", (result) => {
    const isOn = !result.isOn; // Toggle the current state
    chrome.storage.local.set({ isOn }, () => { // Save new state
      console.log("New state saved:", isOn);
      updateStartButton(isOn);

      // Send message to background to toggle service.
      console.log("Sending message to background script to toggle service...");
      chrome.runtime.sendMessage({ action: "toggleBackground", status: isOn }, (responce) => {
        if (responce && responce.status) {
          console.log(responce.status, "background function running: ", responce.status);
        }
      });
      console.log("Message sent to background script.");
    });
  });
}

// Check the stored state and pass it to a callback function
// function checkState(callback) {
//   chrome.storage.local.get("isOn", (result) => {
//     const isOn = result.isOn || false; // Default to false if "isOn" is not set
//     console.log("Current state:", isOn);
//     callback(isOn); // Call the callback with the retrieved state
//   });
// }

if (_start != null) {
  _start.addEventListener("click",
    toggle_start
  );
};
// Listen for messages from popup
chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
  try {
  if (request.action === "getStatus") {
    try {
      const results = detectAuthPage();
      sendResponse(results);
    } catch (error) {
      sendResponse({
        isAuthPage: false,
        confidence: 0,
        pageType: 'unknown',
        error: error.message
      });
    }
  }
  } catch (error) {
    console.error("Error in chrome.runtime.onMessage.addListener:", error);
  }
  // return true; // Keep message channel open for async response
});
initButton();


