console.log("Content script initialization started");
(function () {
  "use strict";

  // let socket;
  // let isRunning = false;

  console.log("Content script IIFE started");

  // const observer = new MutationObserver((mutations) => {
  //   console.log("Mutation observed:", mutations.length, "changes");
  //   if (mutations.some(mutation => mutation.type === "childList" || mutation.attributeName === "class")) {
  //     console.log("Relevant mutation detected, checking for login form...");
  //     runTask();
  //     observer.disconnect();
  //     console.log("Observer disconnected after detecting changes");
  //   }
  // });
  // content.js
  function detectAuthPage() {
    try {
      // Common login/signup form selectors
      const authSelectors = [
        'form[action*="login"]',
        'form[action*="signin"]',
        'form[action*="signup"]',
        'form[action*="register"]',
        '#login',
        '#signup',
        '#signin',
        '#register'
      ];

      // Common input field types
      const authInputTypes = [
        'input[type="password"]',
        'input[name*="password"]',
        'input[name*="email"]',
        'input[name*="username"]'
      ];

      // Common auth-related text in buttons or headings
      const authKeywords = [
        'login',
        'sign in',
        'signup',
        'sign up',
        'register',
        'create account'
      ];

      // Check for auth form selectors
      const hasAuthForm = authSelectors.some(selector =>
        document.querySelector(selector) !== null
      );

      // Check for auth input types
      const hasAuthInputs = authInputTypes.some(selector =>
        document.querySelector(selector) !== null
      );

      // Check page content for auth keywords
      const pageText = document.body ? document.body.innerText.toLowerCase() : '';
      const hasAuthKeywords = authKeywords.some(keyword =>
        pageText.includes(keyword.toLowerCase())
      );

      // Additional checks for common auth patterns
      const hasPasswordField = document.querySelector('input[type="password"]') !== null;
      const hasEmailField = document.querySelector('input[type="email"]') !== null;

      // Calculate confidence score
      let confidence = 0;
      if (hasAuthForm) confidence += 0.4;
      if (hasAuthInputs) confidence += 0.3;
      if (hasAuthKeywords) confidence += 0.2;
      if (hasPasswordField) confidence += 0.3;
      if (hasEmailField) confidence += 0.2;

      // Determine page type
      let pageType = 'unknown';
      if (confidence > 0.5) {
        if (pageText.includes('sign up') || pageText.includes('register') ||
          pageText.includes('create account')) {
          pageType = 'signup';
        } else {
          pageType = 'login';
        }
      }

      return {
        isAuthPage: confidence > 0.5,
        confidence: Math.min(confidence, 1.0),
        pageType: pageType,
        error: null
      };
    } catch (error) {
      console.error('Error in detectAuthPage:', error);
      return {
        isAuthPage: false,
        confidence: 0,
        pageType: 'unknown',
        error: error.message
      };
    }
  } function getLoginFields() {
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
  // // More targeted MutationObserver
  // const observer = new MutationObserver((mutations) => {
  //   for (const mutation of mutations) {
  //     if (mutation.addedNodes.length) {
  //       // Only check if new nodes were added
  //       detectAuthPage();
  //       break;
  //     }
  //   }
  // });

  // observer.observe(document.documentElement, {
  //   childList: true,
  //   subtree: true
  // });

  // document.addEventListener('DOMContentLoaded', detectAuthPage)
  async function checkForLoginForm() {
    console.log("Checking for login form...");

    const loginFields = getLoginFields();

    if (loginFields.length > 0) {
      console.log("Login form fields detected!");

      try {
        let response = await new Promise((resolve, reject) => {
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

        // Handle the response
        if (response.status === "success") {
          if (response.entry) {
            console.log("Entry found for website:", response.website);
            // Handle existing entry (e.g., fill form)
            fillForm(response.entry.username, response.entry.password);
          } else {
            console.log("No entry found for website:", response.website);
            // Handle no entry case
          }
        } else {
          console.error("Error checking for entry:", response.error);
        }
        console.log("Responce2:", response);
      } catch (error) {
        console.error("Error in checkForLoginForm:", error);
      }
    } else {
      console.log("No login fields found yet.");
    }
  }

  function fillForm(username, password) {
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

        // fillForm(response.username, response.password);
      }
    } catch (error) {
      console.error("Error in runTask:", error.message);
    }
  }

  console.log("Initial runTask execution");
  const results = detectAuthPage();
  console.log("Results:", results);
  if (results.isAuthPage === true) {
    console.log("Auth page detected with confidence:", results.confidence);
    runTask();
  }
})();
