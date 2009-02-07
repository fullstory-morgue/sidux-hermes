#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <QSystemTrayIcon>
#include <QAction>
#include <QWidget>

#include "upgradablePackages.h"

class SysTray : public QObject
{
	Q_OBJECT

	public:
		SysTray();
		
	private slots:
		void updateIcon();
		void iconActivated(QSystemTrayIcon::ActivationReason reason);
		void showAbout();
		void showForum();
		void showManual();
		void showKernel();
		void showMessage();
		void showUpgradablePackages();

	private:
		QAction *quitAction;
		QAction *forumAction;
		QAction *manualAction;
		QAction *aboutAction;
		QAction *kernelAction;
		QAction *upgradablePackagesAction;
		QSystemTrayIcon *trayIcon;
		QMenu *trayIconMenu;
		QString status;
		QTimer *timer;
		QString readProcess(QString, QStringList);
		upgradablePackages* upWidget;

};

#endif 
