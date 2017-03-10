/*
 * Copyright (C) 2013 Ronny Yabar <ronnycontacto@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "mainwindow.h"
#include "preferencesdialog.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_exitAction = new QAction(tr("Quit"), this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Quit"));

    m_preferencesAction = new QAction(tr("Preferences"), this);
    m_preferencesAction->setStatusTip(tr("Preferences"));

    m_helpAction = new QAction(tr("About Speedy MP3 Downloader"), this);
    m_helpAction->setStatusTip(tr("About Speedy MP3 Downloader"));

    m_fileMenu = new QMenu;
    m_fileMenu->addAction(m_exitAction);
    m_fileMenu->setTitle(tr("File"));

    m_optionsMenu = new QMenu;
    m_optionsMenu->addAction(m_preferencesAction);
    m_optionsMenu->setTitle(tr("Options"));

    m_helpMenu = new QMenu;
    m_helpMenu->addAction(m_helpAction);
    m_helpMenu->setTitle(tr("Help"));

    m_menuBar = new QMenuBar;
    m_menuBar->addMenu(m_fileMenu);
    m_menuBar->addMenu(m_optionsMenu);
    m_menuBar->addMenu(m_helpMenu);
    m_menuBar->addSeparator();

    mp3Download = new MP3Download;

    connect(m_exitAction, SIGNAL(triggered(bool)), this, SLOT(exit()));
    connect(m_preferencesAction, SIGNAL(triggered(bool)), this, SLOT(preferences()));
    connect(m_helpAction, SIGNAL(triggered(bool)), this, SLOT(aboutMessage()));

    setCentralWidget(mp3Download);
    setDefaultConfigFile();
    setMenuBar(m_menuBar);
    setWindowTitle(tr("Speedy MP3 Downloader"));
}

void MainWindow::aboutMessage()
{
    QMessageBox::StandardButton aboutMessage;
    aboutMessage = QMessageBox::information(this, "Speedy MP3 Downloader", "<p>Speedy MP3 Downloader v1.0 is a program that allows you, " \
                                                  "to download MP3 files in a really fast way.</p>" \
                                                  "<p><strong>http://github.com/ronnyml/SpeedyMP3Downloader</strong></p>");
    qDebug() << aboutMessage << endl;
}

void MainWindow::preferences()
{
    PreferencesDialog preferencesDialog;
    preferencesDialog.exec();
}

void MainWindow::setDefaultConfigFile()
{
    QString configFileName = QDir::currentPath() + "/config.txt";

    QFile configFile(configFileName);
    if (!configFile.open(QFile::Append | QFile::Text)) {
        return;
    }

    if (configFile.size() == 0) {
        QString downloadDirPath = QDir::homePath() + "/SpeedyMP3Downloader/";
        QTextStream out(&configFile);
        out << downloadDirPath << endl;
    }
    configFile.close();
}

void MainWindow::exit()
{
    QCoreApplication::instance()->quit();
}
