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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mp3download.h"

#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTextStream>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setDefaultConfigFile();

private:
    QAction *m_exitAction;
    QAction *m_helpAction;
    QAction *m_preferencesAction;
    QHBoxLayout *m_directoryLayout;
    QLineEdit *m_downloadDirectory;
    QMenu *m_fileMenu;
    QMenu *m_optionsMenu;
    QMenu *m_helpMenu;
    QMenuBar *m_menuBar;
    QPushButton *m_directoryButton;
    MP3Download *mp3Download;

public slots:
    void aboutMessage();
    void preferences();
    void exit(); 

};

#endif // MAINWINDOW_H
