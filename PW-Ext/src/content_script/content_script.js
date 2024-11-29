console.log("Content script initialization started");
(function () {
  "use strict";

  console.log("Content script IIFE started");

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

  async function checkForLoginForm() {
    console.log("Checking for login form...");

    const loginFields = getLoginFields();

    if (loginFields.length > 0) {
      console.log("Login form fields detected!");

      try {
        console.log("Sending loginFormDetected message to background");
        
        // check for www. in window.location.host, if not present, add it.
        let domain = window.location.host;
        if (domain.indexOf("www.") === -1) {
          console.log("www. not found in host, adding it");
          domain = "www." + window.location.host;
        }
        chrome.runtime.sendMessage(
          {
            action: "loginFormDetected",
            type: "request",
            request: {
              type: "check-entry",
              website: domain
            }
          }
          // ,
          // (response) => {
          //   if (chrome.runtime.lastError) {
          //     console.error("Error in chrome.runtime.sendMessage:", chrome.runtime.lastError.message);
          //     reject(new Error(chrome.runtime.lastError.message));
          //   } else {
          //     console.log("Response received from background:", response);
          //     resolve(response);
          //   }
          // }
        );
        console.log("Message sent to background script");
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
      checkForLoginForm();
    } catch (error) {
      console.error("Error in runTask:", error.message);
    }
  }

  chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
    console.log("Message received in content script:", request);
    if (request.action === "fill-form") {
      console.log("Filling form with provided credentials");
      fillForm(request.data.username, request.data.password);
      sendResponse({ status: "success" });
    }
  });

  console.log("Initial runTask execution");
  const results = detectAuthPage();
  console.log("Results:", results);
  if (results.isAuthPage === true) {
    console.log("Auth page detected with confidence:", results.confidence);
    runTask();
  }
})();
