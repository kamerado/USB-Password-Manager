console.log("Content script initialization started");
(function () {
  "use strict";

  let socket;
  let isRunning = false;

  console.log("Content script IIFE started");

  const observer = new MutationObserver((mutations) => {
    console.log("Mutation observed:", mutations.length, "changes");
    if (mutations.some(mutation => mutation.type === "childList" || mutation.attributeName === "class")) {
      console.log("Relevant mutation detected, checking for login form...");
      runTask();
      observer.disconnect();
      console.log("Observer disconnected after detecting changes");
    }
  });

  function getLoginFields() {
    console.log("Starting login field detection");
    var fieldPairs = [],
      pswd = (function () {
        var inputs = document.getElementsByTagName('input'),
          len = inputs.length,
          ret = [];
        while (len--) {
          if (inputs[len].type === 'password') {
            ret[ret.length] = inputs[len];
          }
        }
        console.log("Found", ret.length, "password fields");
        return ret;
      })(),
      pswdLength = pswd.length;

    function parentForm(elem) {
      while (elem.parentNode) {
        if (elem.parentNode.nodeName.toLowerCase() === 'form') {
          return elem.parentNode;
        }
        elem = elem.parentNode;
      }
    }

    while (pswdLength--) {
      var curPswdField = pswd[pswdLength],
        form = parentForm(curPswdField);
      if (form) {
        var inputs = form.getElementsByTagName('input');
        for (var i = 0; i < inputs.length; i++) {
          if (inputs[i] !== curPswdField && inputs[i].type === 'text') {
            fieldPairs.push([window.location.hostname, inputs[i], curPswdField]);
            console.log("Found login field pair on", window.location.hostname);
            break;
          }
        }
      }
    }
    console.log("Total field pairs found:", fieldPairs.length);
    return fieldPairs;
  }

  async function checkForLoginForm() {
    console.log("Checking for login form...");
    const loginFields = getLoginFields();

    if (loginFields.length > 0) {
      console.log("Login form fields detected!");

      return new Promise((resolve, reject) => {
        console.log("Sending loginFormDetected message to background");
        chrome.runtime.sendMessage(
          {
            action: "loginFormDetected",
            type: "request",
            request: {
              type: "check-entry",
              website: window.location.host
            }
          },
          (response) => {
            if (chrome.runtime.lastError) {
              console.error("Error in chrome.runtime.sendMessage:", chrome.runtime.lastError.message);
              reject(new Error(chrome.runtime.lastError.message));
            } else {
              console.log("Response received from background:", response);
              resolve(response);
            }
          }
        );
      });
    } else {
      console.log("No login fields found yet.");
    }
  }

  async function fillForm(username, password) {
    console.log("Attempting to fill form");
    const loginFields = getLoginFields();
    if (loginFields.length > 0) {
      loginFields.forEach(([hostname, usernameField, passwordField]) => {
        if (hostname === window.location.host) {
          usernameField.value = username;
          passwordField.value = password;
          console.log("Form filled with provided credentials");
        }
      });
    } else {
      console.log("No login form detected for filling");
    }
  }

  async function runTask() {
    try {
      console.log("Starting runTask");
      const response = await checkForLoginForm();
      if (response) {
        await fillForm(response.username, response.password);
      }
    } catch (error) {
      console.error("Error in runTask:", error.message);
    }
  }

  console.log("Initial runTask execution");
  runTask();

  const formContainer = document.querySelector("body");
  if (formContainer) {
    console.log("Setting up mutation observer on body");
    observer.observe(formContainer, {
      childList: true,
      subtree: true,
      attributes: true,
      attributeFilter: ["class", "id"],
    });
  } else {
    console.error("Could not find body element for observer");
  }
})();
