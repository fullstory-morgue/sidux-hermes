 #include <QtGui>
 #include <QTranslator>
 #include "systray.h"





 int main(int argc, char *argv[])
 {
	QApplication app(argc, argv);
        QString locale = QLocale::system().name();
        QTranslator translator;
        translator.load("sidux-hermes-qt4_"+locale, "/usr/share/sidux-hermes/translations" );
        app.installTranslator(&translator);


	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		QMessageBox::critical(0, "sidux-hermes", QObject::tr("I couldn't detect any system tray on this system."));
		return 1;
	}

	SysTray tray;
	return app.exec();

 }