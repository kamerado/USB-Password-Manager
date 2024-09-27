#include "src/core/CefThread.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/internal/cef_linux.h"
#include "include/internal/cef_types_wrappers.h"
#include "src/core/simpleApp.h"
#include <QCoreApplication>
#include <QTimer>
#include <qcoreapplication.h>
#include <qglobal.h>
#include <qtimer.h>

CefThread::CefThread(QObject *parent)
    : QObject(parent), app(new SimpleApp), handler(new SimpleHandler) {}

CefThread::~CefThread() {}

bool CefThread::threadInitialized() { return this->isInitialized; }

void CefThread::initialize() {
  QStringList args = QCoreApplication::arguments();
  int argc = args.size();
  std::vector<const char *> argv;
  for (const QString &arg : args) {
    argv.push_back(arg.toLocal8Bit().constData());
  }

  CefMainArgs main_args(argc, const_cast<char **>(argv.data()));

  CefSettings settings;
  CefString(&settings.resources_dir_path)
      .FromString(
          "/home/meganuts/projects/USB-Password-Manager/libs/Resources/");

  CefString(&settings.locales_dir_path)
      .FromString("/home/meganuts/projects/USB-Password-Manager/libs/Resources/"
                  "locales/");

  std::string cachePath = std::string(getenv("HOME")) + "/.myapp/cache";
  CefString(&settings.root_cache_path).FromASCII(cachePath.c_str());

  settings.multi_threaded_message_loop = false;

  if (!CefInitialize(main_args, settings, app.get(), nullptr)) {
    emit initialized(false);
  } else {
    isInitialized = true;
    emit initialized(true);
    settings.multi_threaded_message_loop = true;
  }
}

void CefThread::createBrowser() {
  if (handler->HasBrowser()) {
    return;
  }
  CefWindowInfo windowInfo;
  CefBrowserSettings browserSettings;
  std::string url = "https://www.google.com";

  CefBrowserHost::CreateBrowser(windowInfo, handler, url, browserSettings,
                                nullptr, nullptr);
}

void CefThread::doMessageLoop() { CefRunMessageLoop(); }

void CefThread::shutdown() { CefShutdown(); }
