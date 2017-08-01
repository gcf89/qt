#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QAction>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>

#ifdef Q_OS_UNIX
#include <X11/Xlib.h>
#endif

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
  Ui::Widget*           ui;
  QFileSystemWatcher*   mFileWatcher;
  qint64                mFilePos;

  QAction *minimizeAction;
  QAction *maximizeAction;
  QAction *restoreAction;
  QAction *quitAction;

  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;

  QString               mSourcePath;

  QStringList mGoodHardwareSNs;
  QStringList mBadHardwareSNs;

#ifdef Q_OS_UNIX
  Display *dpy;
#endif

  bool mIsLocked;

  Q_OBJECT

public:
  bool Load(QString path);

  void setVisible(bool visible) override;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
  void RunWatcher(const QString& path);
  bool Parse(QString data, qint64 filesize);
  void CreateActions();
  void CreateTrayIcon();

  void OldGuiEnabled(bool enabled);
  void NewGuiEnabled(bool enabled);

  void CenterWindow();

  void ConsiderLock();

private slots:
  void onTargetFileChanged();
  void on_pushButton_clicked();
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void UnlockSystem();
  void LockSystem();

public:
  explicit Widget(QWidget *parent = 0);
  ~Widget();
};

#endif // WIDGET_H
