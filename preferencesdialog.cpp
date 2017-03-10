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

#include "preferencesdialog.h"

#include <QtCore>

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
{
    m_directoryLabel =new QLabel(this);
    m_directoryLabel->setText(tr("Downloads Folder: "));
    m_directoryDownload = new QLineEdit(this);

    m_configFile.setFileName(QDir::currentPath() + "/config.txt");
    if(!m_configFile.exists())
        return;

    if (!m_configFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&m_configFile);
    QString line = stream.readLine();

    m_downloadDirPath = line;
    m_configFile.close();

    m_directoryDownload->setText(m_downloadDirPath);
    m_directoryDownload->setFixedWidth(300);

    m_changeButton = new QPushButton(tr("Change.."), this);
    m_saveButton = new QPushButton;
    m_saveButton->setText(tr("Save"));
    m_saveButton->setFixedWidth(100);

    m_cancelButton = new QPushButton;
    m_cancelButton->setText(tr("Cancel"));
    m_cancelButton->setFixedWidth(100);

    m_directoryLayout = new QHBoxLayout;
    m_directoryLayout->addWidget(m_directoryLabel);
    m_directoryLayout->addWidget(m_directoryDownload);
    m_directoryLayout->addWidget(m_changeButton);

    m_buttonsLayout = new QHBoxLayout;
    m_buttonsLayout->addWidget(m_saveButton);
    m_buttonsLayout->addWidget(m_cancelButton);

    m_dialogLayout = new QGridLayout(this);
    m_dialogLayout->addLayout(m_directoryLayout,0,0);
    m_dialogLayout->addLayout(m_buttonsLayout,1,0);

    connect(m_changeButton, SIGNAL(clicked()), this, SLOT(setDownloadDirectory()));
    connect(m_saveButton, SIGNAL(clicked()), this, SLOT(saveDownloadDirectory()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    setLayout(m_dialogLayout);
}

void PreferencesDialog::setDownloadDirectory()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QFileDialog fileDialog;
    QString directory = fileDialog.getExistingDirectory(this, tr("Downloads Folder"), m_downloadDirPath, options);

    if (!directory.isEmpty())
        m_directoryDownload->setText(directory);
}

void PreferencesDialog::saveDownloadDirectory()
{
    if(!m_configFile.open(QFile::Append | QFile::Text)) {
        return;
    }
    m_configFile.resize(0);

    QTextStream out(&m_configFile);
    out << m_directoryDownload->text() << endl;
    m_configFile.close();

    this->close();
}
