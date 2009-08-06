#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <QSystemTrayIcon>
#include <QAction>
#include <QWidget>
 #include <QHttp>
  #include <QXmlStreamReader>
 
#include "ui_warnings.h"
#include "upgradablePackages.h"


class SysTray : public QWidget
{
	Q_OBJECT

	public:
		SysTray();
		
	private slots:
		void iconActivated(QSystemTrayIcon::ActivationReason reason);
		void showAbout();
		void showForum();
		void showManual();
		void showKernel();
		void showMessage();
		void showUpgradablePackages();
		void fetchFeed();
		void readFeed(const QHttpResponseHeader &);
		void readKernel(const QHttpResponseHeader &);
		void parseXml();   
		     
		     


	private:
		// menu
		QMenu *trayIconMenu;
		QAction *quitAction;
		QAction *forumAction;
		QAction *manualAction;
		QAction *aboutAction;
		QAction *kernelAction;
		QAction *upgradablePackagesAction;
		
		// systray
		QSystemTrayIcon *trayIcon;
		QString status;
		QTimer *timer;
		
		QString readProcess(QString, QStringList);
		
		// upgradable packages
		upgradablePackages* upWidget;
		
		
		// kernel info
		QString currentKernel, newestKernel;
		QHttp http2;

		// warnings
		Ui::warningsGui warningsGui;
		QWidget *warningsWidget;
		QString currentTag, title, description;
		QXmlStreamReader xml;
		QHttp http;



};

#endif 
