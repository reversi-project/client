#include "utils.h"

#include <QApplication>
#include <QTimer>
#include <QWidget>

void ignoreMessageBox(int& closes) {
  QTimer::singleShot(0, [&closes]() {
    QWidget* widget = QApplication::activeModalWidget();
    if (widget) {
      widget->close();
      ++closes;
    }
  });
}

void ignoreMessageBox() {
  int closes = 0;
  ignoreMessageBox(closes);
}
