/*
 * upgradablePackages.h
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


#ifndef UP_H
#define UP_H

#include <QCloseEvent>
#include <QProcess>


#include "ui_upgradablePackages.h"

class up : public QWidget, Ui::upgradablePackages
{
	Q_OBJECT

	public:
		up(QWidget* parent = 0, Qt::WFlags flags = 0);
		void init();


	private slots:
		void getPackages();
		void upgrade();
		void download();
		void update();
		void readOutput();
		void displayPackages();
		void runSAQ(QString);


	private:
		QProcess *process;
		QString packages;

	protected:
		void closeEvent(QCloseEvent *event);

};

#endif
