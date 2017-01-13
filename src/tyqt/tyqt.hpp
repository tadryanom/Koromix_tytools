/* Teensy Tools - public domain
   Niels Martignène <niels.martignene@gmail.com>
   https://neodd.com/teensytools

   This software is in the public domain. Where that dedication is not
   recognized, you are granted a perpetual, irrevocable license to copy,
   distribute, and modify this file as you see fit.

   See the LICENSE file for more details. */

#ifndef TYQT_HH
#define TYQT_HH

#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QSystemTrayIcon>

#include <memory>

#include "database.hpp"
#include "monitor.hpp"
#include "session_channel.hpp"

class LogDialog;
class MainWindow;
class SelectorDialog;

#define tyQt (TyQt::instance())

class TyQt : public QApplication {
    Q_OBJECT

    int argc_;
    char **argv_;
    QString command_;
    bool wait_ = false;

    SessionChannel channel_;

    Monitor monitor_;

    SettingsDatabase tyqt_db_;
    SettingsDatabase monitor_db_;
    SettingsDatabase monitor_cache_;

    DatabaseInterface db_;

    QAction *action_visible_;
    QAction *action_quit_;
    QSystemTrayIcon tray_icon_;
    QMenu tray_menu_;

#ifdef _WIN32
    bool client_console_ = false;
#else
    bool client_console_ = true;
#endif

    bool show_tray_icon_;
    bool hide_on_startup_;

    std::unique_ptr<LogDialog> log_dialog_;

public:
    TyQt(int &argc, char *argv[]);
    virtual ~TyQt();

    static QString clientFilePath();

    void setDatabase(DatabaseInterface db) { db_ = db; }
    DatabaseInterface database() const { return db_; }
    void loadSettings();

    static int exec();

    static TyQt *instance() { return static_cast<TyQt *>(QCoreApplication::instance()); }
    Monitor *monitor() { return &monitor_; }

    bool visible() const { return action_visible_->isChecked(); }

    void setClientConsole(bool console) { client_console_ = console; }
    bool clientConsole() const { return client_console_; }

    bool showTrayIcon() const { return show_tray_icon_; }
    bool hideOnStartup() const { return hide_on_startup_; }

    int run(int argc, char *argv[]);
    int runMainInstance(int argc, char *argv[]);
    int executeRemoteCommand(int argc, char *argv[]);
    int integrateArduino(int argc, char *argv[]);
    int fakeAvrdudeUpload(int argc, char *argv[]);

    void resetMonitor();
    void clearSettingsAndReset();
    void clearSettingsAndResetWithConfirmation(QWidget *parent = nullptr);

public slots:
    void showLogWindow();

    void reportError(const QString &msg, const QString &ctx = QString());
    void reportDebug(const QString &msg, const QString &ctx = QString());

    void setShowTrayIcon(bool show_tray_icon);
    void setHideOnStartup(bool hide_on_startup);

    void setVisible(bool visible);

signals:
    void settingsChanged();

    void globalError(const QString &msg, const QString &ctx);
    void globalDebug(const QString &msg, const QString &ctx);

private:
    void initDatabase(const QString &name, SettingsDatabase &db);
    void initCache(const QString &name, SettingsDatabase &cache);

    QString helpText();
    void showClientMessage(const QString &msg);
    void showClientError(const QString &msg);

private slots:
    void trayActivated(QSystemTrayIcon::ActivationReason reason);

    void acceptClient();
    void processServerAnswer(const QStringList &arguments);
};

#endif
