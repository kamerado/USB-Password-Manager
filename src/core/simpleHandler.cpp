#include "../src/core/simpleHandler.h"

#include "include/cef_browser.h"
#include <QWidget>

SimpleHandler::SimpleHandler() {}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  browser_ = browser;
}

void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                                const CefString &errorText,
                                const CefString &failedUrl) {}

bool SimpleHandler::HasBrowser() { return browser_ != nullptr; }
