#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QString>

namespace Ui {
class FormCustomSearchResult;
}

class FormCustomSearchResult : public QWidget
{
  Ui::FormCustomSearchResult*         ui;
  QString                             mResultTxt;
  Q_OBJECT

public:
  explicit FormCustomSearchResult(const QString& html,
                                  const QString& txt,
                                  QWidget *parent = 0);
  ~FormCustomSearchResult();

  QString GetResultText() const { return mResultTxt; }

signals:
  void MouseReleased(QMouseEvent* event);
};

