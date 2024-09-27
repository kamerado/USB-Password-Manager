#ifndef CEFMGR_H
#define CEFMGR_H

#pragma once
#include "include/cef_client.h"
#include <QWidget>

class SimpleHandler : public CefClient,
                      public CefLifeSpanHandler,
                      public CefLoadHandler {
public:
  SimpleHandler();

  // CefClient methods
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
    return this;
  }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }

  // CefLifeSpanHandler methods
  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

  // CefLoadHandler methods
  void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                   ErrorCode errorCode, const CefString &errorText,
                   const CefString &failedUrl) override;
  bool HasBrowser();

private:
  CefRefPtr<CefBrowser> browser_;
  IMPLEMENT_REFCOUNTING(SimpleHandler);
};
#endif
