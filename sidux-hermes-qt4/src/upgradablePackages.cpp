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
	packages.clear();

	QTreeWidgetItem *item = new QTreeWidgetItem( treeWidget, 0 );
	item->setText( 0,tr("Plase wait") );

	connect( process, SIGNAL(readyReadStandardOutput()),this, SLOT(readOutput()));
	connect( process, SIGNAL(finished(int)),this, SLOT(displayPackages()));
	process->start("sidux-hermes", (QStringList() << "getUpgradablePackages") );
}


void up::readOutput()
{
	packages += QString(process->readAllStandardOutput());

}

void up::displayPackages()
{
	treeWidget->clear();
	packages.replace("\n", " ");
	packages.replace("*manually* ", "");
	packages.replace("/", "@");
	packages.replace(" upgradeable from ", "@");
	packages.replace(" to ", "@");
  

	foreach( QString package, packages.split(" ") ) {
		QStringList tmpArray = package.split("@");
		if( tmpArray.count() == 4) {
			QTreeWidgetItem * item = new QTreeWidgetItem( treeWidget, 0 );
			item->setText( 0, tmpArray[0] );
			item->setText( 1, tmpArray[2] );
			item->setText( 2, tmpArray[3] );
		}
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