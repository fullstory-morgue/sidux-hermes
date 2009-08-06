#include <QtGui>
#include <QFile>
#include <QProcess>
#include <QString>
#include <QTimer>
#include <QMenu>
#include <QMessageBox>
#include <QListView>
#include <QDomDocument>


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
	upgradablePackagesAction = new QAction(tr("Upgradable packages"), this);
	connect(upgradablePackagesAction, SIGNAL(triggered()), this, SLOT(showUpgradablePackages()));



	trayIconMenu = new QMenu();
	trayIconMenu->addAction(forumAction);
	trayIconMenu->addAction(manualAction);
	trayIconMenu->addAction(kernelAction);
	trayIconMenu->addAction(upgradablePackagesAction);
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
	upWidget = new upgradablePackages();
	
	

	
	connect(&http, SIGNAL(readyRead(const QHttpResponseHeader &)), this, SLOT(readFeed(const QHttpResponseHeader &)));
	warningsWidget = new QWidget();
	warningsGui.setupUi(warningsWidget);
	fetchFeed(); // update icon
	
	
	connect(&http2, SIGNAL(readyRead(const QHttpResponseHeader &)), this, SLOT(readKernel(const QHttpResponseHeader &)));
}

void SysTray::fetchFeed()
{
	xml.clear();
	warningsGui.label->clear();
	//QUrl url("http://www.sidux.com/module-News-view-prop-Topic-cat-10006-theme-rss.html");
	QUrl url("http://localhost/feed.rss");
	http.setHost(url.host());
	http.get(url.path());

}



void SysTray::readFeed(const QHttpResponseHeader &resp)
{
	if (resp.statusCode() != 200) {
		http.abort();
		trayIcon->setIcon( QIcon("/usr/share/sidux-hermes/icons/disconnected.png") );
		status = tr("No connection to sidux.com!");
	}
	else {
		xml.addData(http.readAll());
		parseXml();
		
		
	}
	
	trayIcon->setToolTip( status );
	QTimer::singleShot(1800000, this, SLOT(fetchFeed()));
}
 
 
 
void SysTray::parseXml()
{
	bool redAlert = false;
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isStartElement()) {
			currentTag = xml.name().toString();
		} else if (xml.isEndElement()) {
			if (xml.name() == "item") {
				if( warningsGui.label->text().isEmpty() ) 
					warningsGui.label->setText(title+description);
				else 
					warningsGui.label->setText(warningsGui.label->text()+"<br><br>"+title+description);
				title.clear(); description.clear();
			}
		} else if (xml.isCharacters() && !xml.isWhitespace()) {
			if (currentTag == "title") {
				title = "<b>"+xml.text().toString() + "</b><br><br>";
				if( title.contains("Alert") or title.contains("ALERT"))
					redAlert = true;
			}
			else if (currentTag == "description")
				description = xml.text().toString();
		}
	}
	if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
	    qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
	    http.abort();
	}
	
	if( warningsGui.label->text().isEmpty() ) 
	{
		trayIcon->setIcon( QIcon("/usr/share/sidux-hermes/icons/ok-hermes.png") );
		status = tr("No dist-upgrade warnings.");
	}
	else
	{
		if( redAlert) 
			trayIcon->setIcon( QIcon("/usr/share/sidux-hermes/icons/alert.png") );
		else
			trayIcon->setIcon( QIcon("/usr/share/sidux-hermes/icons/prealert.png") );
		status = tr("There are dist-upgrade alerts. Please visit sidux.com for more information!");
	}
	
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
 
	if( !warningsGui.label->text().isEmpty() ) 
		warningsWidget->show();
	else
		trayIcon->showMessage ( tr("Status"), status, QSystemTrayIcon::Information, 10000 );
}


void SysTray::showKernel()
{

  
  
	if( currentKernel.isEmpty() ) {
		QProcess exec;
		exec.start("uname", (QStringList() << "-r") );
		exec.waitForFinished();
		currentKernel = QString( exec.readAll() ).replace("\n", "");
	}
	
	
	if( newestKernel.isEmpty() ) {	
		QUrl url("http://sidux.com/debian/pool/main/l/linux-sidux-2.6/");
		http2.setHost(url.host());
		http2.get(url.path());
	}
	else{
	      QString kernel;
	      kernel += "\n"+tr("Current kernel")+": "+currentKernel;
	      kernel += "\n"+tr("Newest  kernel")+": "+newestKernel;
	      trayIcon->showMessage ( tr("Kernel informations"), kernel, QSystemTrayIcon::Information, 10000 );
	}
}






void SysTray::readKernel(const QHttpResponseHeader &resp)
{
	if (resp.statusCode() != 200) {
		http2.abort();
	}
	else {
	  	QString kernelTyp = "686";
		if( currentKernel.contains("amd64") )
			kernelTyp = "amd64";
		QStringList fetchArray = QString( http2.readAll() ).split('"');
		foreach(QString fetch, fetchArray) {
			if(fetch.contains("linux-image") and fetch.contains(kernelTyp) and fetch.contains("slh") ) {
				newestKernel = fetch.replace("linux-image-","");
				newestKernel = newestKernel.split("_")[0];
				break;
			}
		
		}
	}
	
	QString kernel;
	kernel += "\n"+tr("Current kernel")+": "+currentKernel;
	kernel += "\n"+tr("Newest  kernel")+": "+newestKernel;
	trayIcon->showMessage ( tr("Kernel informations"), kernel, QSystemTrayIcon::Information, 10000 );
}




void SysTray::showAbout()
{

	QString about; 
	about += "\n"+tr("This program warns you, if there are problems in the Debian sid\nrepository. It gets the warnings from the sidux homepage.")+"\n\n" ; 

	about += tr("Programmer")+":\n";
	about += QString::fromUtf8("Fabian Würtz")+" <xadras@sidux.com>\n";

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

	about += tr("Licence") +": GPL" +"\n";

	QString version = readProcess("sidux-hermes", (QStringList() << "-v") );
	about += tr("Version") +": "+version ;


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
//-- upgradable packages -------------------------------------------------------
//------------------------------------------------------------------------------

void SysTray::showUpgradablePackages()
{
	upWidget->init();
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