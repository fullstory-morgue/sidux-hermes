/*
 * legend.cpp
 *
 * Copyright (c) 2007 Fabian Wuertz <xadras@sidux.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include "upgradablePackages.h"
#include <QWhatsThis>
up::up (QWidget* parent, Qt::WFlags flags): QWidget (parent, flags)
{
	setupUi(this);

	connect(reloadPushButton, SIGNAL(clicked()), this, SLOT(getPackages()));
	connect(upgradePushButton, SIGNAL(clicked()), this, SLOT(upgrade()));
	connect(updatePushButton, SIGNAL(clicked()), this, SLOT(update()));
	connect(downloadPushButton, SIGNAL(clicked()), this, SLOT(download()));


	process = new QProcess(this);
}


void up::init()
{

	if( isHidden() ) {
		show();
		if ( treeWidget->topLevelItemCount() == 0 ) {
			QTreeWidgetItem * item = new QTreeWidgetItem( treeWidget, 0 );
			item->setText(0,tr("Please wait")); 
			getPackages(); 
		}

	}
	else
		hide();
}

void up::getPackages()
{

	reloadPushButton->setEnabled(FALSE);
	upgradePushButton->setEnabled(FALSE);
	updatePushButton->setEnabled(FALSE);
	downloadPushButton->setEnabled(FALSE);
	closePushButton->setEnabled(FALSE);
	treeWidget->clear();
	name.clear();
	currentVersion.clear();
	newVersion.clear();

	QTreeWidgetItem *item = new QTreeWidgetItem( treeWidget, 0 );
	item->setText( 0,tr("Plase wait") );

	connect( process, SIGNAL(readyReadStandardOutput()),this, SLOT(readOutput()));
	connect( process, SIGNAL(finished(int)),this, SLOT(displayPackages()));
	process->start("sidux-hermes", (QStringList() << "getUpgradablePackages") );
}


void up::readOutput()
{
	QByteArray result;
	result = process->readAllStandardOutput();

	QStringList packages = QString(result).split("\n");
	QStringList tmp;
	foreach( QString package, packages ) {
		package.replace("*manually* ", "" );
		if(package != "") {
			tmp = package.split( "/" );
				name.append(tmp[0]);
			tmp = package.split( " " );
			if(tmp.count() > 3)
				currentVersion.append(tmp[3]);
			else
				currentVersion.append("");
			if(tmp.count() > 5)
				newVersion.append(tmp[5]); 
			else
				newVersion.append("");
		}
	}
}

void up::displayPackages()
{
	treeWidget->clear();
	for( int i = 0; i < name.count(); i++) {
		QTreeWidgetItem * item = new QTreeWidgetItem( treeWidget, 0 );
		item->setText( 0, name[i] );
		item->setText( 1, currentVersion[i] );
		item->setText( 2, newVersion[i] );
	}
	reloadPushButton->setEnabled(TRUE);
	upgradePushButton->setEnabled(TRUE);
	closePushButton->setEnabled(TRUE);
	updatePushButton->setEnabled(TRUE);
	downloadPushButton->setEnabled(TRUE);

}



void up::upgrade()
{
	QWhatsThis::showText( QCursor::pos(), tr("To update your system you have to leave the graphic modus. Press for this CTR+ALT+F1, login as root and type init 3. After that run the command smxi or siduxcc (Software->Dist-upgrade)."), this );

}

void up::update()
{
	runSAQ("update");
}


void up::download()
{
	runSAQ("download");;

}

void up::runSAQ(QString argument)
{

	QString program = "su-to-root";
	QStringList arguments;
	arguments << "-X" << "-c" << "sidux-apt-qt4 "+argument;

	QProcess *saqProcess = new QProcess(this);
	saqProcess->start(program, arguments);
}

void up::closeEvent(QCloseEvent *event)
{
	hide();
	event->ignore();
}