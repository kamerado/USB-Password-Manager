#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H
#pragma once
#include "include/cef_app.h"

class SimpleApp : public CefApp {
public:
  SimpleApp();

private:
  IMPLEMENT_REFCOUNTING(SimpleApp);
};

#endif
