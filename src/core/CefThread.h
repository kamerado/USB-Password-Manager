#ifndef CEFTHREAD_H
#define CEFTHREAD_H

#include "include/internal/cef_ptr.h"
#include "src/core/simpleApp.h"
#include "src/core/simpleHandler.h"
#include <QObject>
#include <qobject.h>
#include <qtmetamacros.h>

class CefThread : public QObject {
  Q_OBJECT

public:
  explicit CefThread(QObject *parent = nullptr);
  bool threadInitialized();
  ~CefThread();

public slots:
  void initialize();
  void createBrowser();
  void doMessageLoop();
  void shutdown();

signals:
  void initialized(bool success);

private:
  CefRefPtr<SimpleApp> app;
  CefRefPtr<SimpleHandler> handler;
  bool isInitialized = false;
};

#endif // !CEFTHREAD_H
