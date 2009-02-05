 #ifndef SYSTRAY_H
 #define SYSTRAY_H

 #include <QSystemTrayIcon>

 class QAction;
 class QCheckBox;
 class QComboBox;
 class QGroupBox;
 class QLabel;
 class QLineEdit;
 class QMenu;
 class QPushButton;
 class QSpinBox;
 class QTextEdit;

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

	private:
		QAction *quitAction;
		QAction *forumAction;
		QAction *manualAction;
		QAction *aboutAction;
		QAction *kernelAction;
		QSystemTrayIcon *trayIcon;
		QMenu *trayIconMenu;
		QString status;
		QTimer *timer;
		QString readProcess(QString, QStringList);


 };

 #endif 
