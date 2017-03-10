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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    PreferencesDialog(QWidget *parent = 0);

private:
    QDir m_userDir;
    QFile m_configFile;
    QGridLayout *m_dialogLayout;
    QHBoxLayout *m_buttonsLayout;
    QHBoxLayout *m_directoryLayout;
    QLabel *m_directoryLabel;
    QLineEdit *m_directoryDownload;
    QPushButton *m_cancelButton;
    QPushButton *m_changeButton;
    QPushButton *m_saveButton;
    QString m_downloadDirPath;

private slots:
    void saveDownloadDirectory();
    void setDownloadDirectory();

};

#endif // PREFERENCESDIALOG_H
