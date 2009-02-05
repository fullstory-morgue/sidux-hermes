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
		QTimer *timer;
		
	private slots:
		void updateIcon();
		void iconActivated(QSystemTrayIcon::ActivationReason reason);
		void showMessage();
		void showAbout();
		void showForum();
		void showManual();

	private:
		QAction *quitAction;
		QAction *forumAction;
		QAction *manualAction;
		QAction *aboutAction;
		QSystemTrayIcon *trayIcon;
		QMenu *trayIconMenu;
		QString status;


 };

 #endif 
