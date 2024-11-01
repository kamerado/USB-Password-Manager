var h1 = document.getElementById("h1");
const _start = document.getElementById("_start");

function updateStartButton(isOn) {
  // Apply the correct class based on the new state
  if (isOn) {
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
  checkState(updateStartButton);
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
      chrome.runtime.sendMessage({ action: "toggleBackground", state: isOn }, (responce) => {
        if (responce && responce.status) {
          console.log(responce.status, "background function running: ", responce.status);
        }
      });
    });
  });
}

// Check the stored state and pass it to a callback function
function checkState(callback) {
  chrome.storage.local.get("isOn", (result) => {
    const isOn = result.isOn || false; // Default to false if "isOn" is not set
    console.log("Current state:", isOn);
    callback(isOn); // Call the callback with the retrieved state
  });
}

if (_start != null) {
  _start.addEventListener("click",
    toggle_start
  );
};

initButton();
