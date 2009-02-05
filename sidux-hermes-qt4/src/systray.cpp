#include <QtGui>
#include <QFile>
#include <QProcess>
#include <QString>
#include <QTimer>
#include <QMenu>
#include <QMessageBox>

 #include "systray.h"

 SysTray::SysTray()
 {

	quitAction = new QAction(tr("&Close"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	forumAction = new QAction( tr("Forum - Upgrade Warnings"), this);
	connect(forumAction, SIGNAL(triggered()), this, SLOT(showForum()));
	manualAction = new QAction(tr("Manual - Upgrade of an Installed System"), this);
	connect(manualAction, SIGNAL(triggered()), this, SLOT(showManual()));

	kernelAction = new QAction(tr("Kernel informations"), this);
	connect(kernelAction, SIGNAL(triggered()), this, SLOT(showKernel()));

	aboutAction = new QAction(tr("About sidux-hermes"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));



	trayIconMenu = new QMenu();
	trayIconMenu->addAction(forumAction);
	trayIconMenu->addAction(manualAction);
	trayIconMenu->addAction(kernelAction);
	trayIconMenu->addAction(aboutAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);


	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
	trayIcon->setIcon( QIcon("/usr/share/sidux-hermes/icons/disconnected.png") );
	trayIcon->setVisible(TRUE);
	trayIcon->setToolTip("sidux-hermes");
	trayIcon->show();


	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	timer = new QTimer(this);
	
	updateIcon();

}

void SysTray::updateIcon()
{

	QString result  = readProcess("sidux-hermes", (QStringList() << "duWarnings")  );

	if ( result == "disconnected\n" )
	{
		trayIcon->setIcon( QIcon("/usr/share/sidux-hermes/icons/disconnected.png") );
		status = tr("No connection to sidux.com.");
	}
	else
	{
		if ( result == "" )
		{
			trayIcon->setIcon( QIcon("/usr/share/sidux-hermes/icons/ok-hermes.png") );
			status = tr("No dist-upgrade warnings.");
		}
		else
		{
			trayIcon->setIcon( QIcon("/usr/share/sidux-hermes/icons/notok-hermes.png") );
			status = tr("There are dist-upgrade warnings. Please visit sidux.com for more information!");
		}
	}

	trayIcon->setToolTip( result );
	
	QTimer::singleShot(1800000, this, SLOT(updateIcon()));
	
}


//------------------------------------------------------------------------------
//-- tray events ---------------------------------------------------------------
//------------------------------------------------------------------------------

void SysTray::iconActivated(QSystemTrayIcon::ActivationReason reason)
{

	switch (reason) {
		case QSystemTrayIcon::Trigger:
		case QSystemTrayIcon::DoubleClick:
			showMessage();
			break;
		case QSystemTrayIcon::MiddleClick:
			showMessage();
			break;
		default:
			;
	}
}



void SysTray::showMessage()
{
	trayIcon->showMessage ( tr("Status"), status, QSystemTrayIcon::Information, 10000 );
}


void SysTray::showKernel()
{


	QString currentKernel = readProcess("sidux-hermes", (QStringList() << "currentKernel") ).replace("\n", "");
	QString newestKernel  = readProcess("sidux-hermes", (QStringList() << "newestKernel")  ).replace("\n", "");

	QString kernel;

	kernel += "\n"+tr("Current kernel")+": "+currentKernel;
	kernel += "\n"+tr("Newest  kernel")+": "+newestKernel;

	trayIcon->showMessage ( tr("Kernel informations"), kernel, QSystemTrayIcon::Information, 10000 );
}

void SysTray::showAbout()
{

	QString about; 
	about += "\n"+tr("This programm warns you if there a problems in the Debian sid\nrepository. It gets the warnings from the sidux homepage.")+"\n\n" ; 

	about += tr("Programmer")+":\n";
	about += QString::fromUtf8("Fabian Würtz")+" <xadras@sidux.com>\n\n";

	QString language = tr("LANGUAGE OF TRANSLATION");
	QStringList translators = tr("NAME OF TRANSLATORS").split("\n");
	QStringList emails      = tr("EMAIL OF TRANSLATORS").split("\n");
	if( language != "LANGUAGE OF TRANSLATION") {
		about += tr("Translator")+" ("+language+"):\n";
		for( int i = 0; i < translators.count(); i++) {
			QString email = "";
			QString translator = translators[i].replace("\n", "");
			if( i < emails.count() )
				if( emails[i] != "")
					email = " <"+emails[i]+">";
			about += translator+email+"\n";
		}
	}
	
	about += "\n"+tr("Icons")+":\n";
	about += QString::fromUtf8("Bernard Gray")+" <bernard.gray@gmail.com>\n\n";

	about += tr("Licence") +": GPL" ;


	trayIcon->showMessage ( tr("About sidux-hermes"), about, QSystemTrayIcon::Information, 20000 );
}

void SysTray::showForum()
{
	QDesktopServices::openUrl(QUrl("http://www.sidux.com/PNphpBB2-viewforum-f-29.html"));
}

void SysTray::showManual()
{
	QString locale = QLocale::system().name().left(2).toLower();
	QStringList manualLanguages;
	manualLanguages << "da" << "de" << "el" << "es" << "fr";
	manualLanguages << "hr" << "it" << "nl" << "pl" << "ja";
	manualLanguages << "pt" << "ro" << "ru";
 	if( manualLanguages.indexOf(locale) == -1)
		locale = "en";

	QDesktopServices::openUrl(QUrl("http://manual.sidux.com/" + locale + "/sys-admin-apt-" + locale + ".htm#apt-upgrade"));
}

//------------------------------------------------------------------------------
//-- exec ----------------------------------------------------------------------
//------------------------------------------------------------------------------

QString SysTray::readProcess(QString run, QStringList arguments)
{
	QProcess exec;
	exec.start(run, arguments);
	exec.waitForFinished();
	return QString( exec.readAll() );
}