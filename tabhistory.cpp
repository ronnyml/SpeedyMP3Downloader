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

#include "tabhistory.h"

#include <QtCore>

TabHistory::TabHistory(QWidget *parent) :
    QWidget(parent)
{
    m_titleLabel = new QLabel;
    m_titleLabel->setText(tr("Downloads History: "));

    m_clearButton = new QPushButton;
    m_clearButton->setText(tr("Clear History"));
    m_clearButton->setFixedWidth(200);

    m_historyWidget = new QTreeWidget(this);
    m_historyWidget->setColumnCount(4);
    m_hlabels << tr("FileName") << tr("Song") << tr("Artist") << tr("Download Date");
    m_historyWidget->setHeaderLabels(m_hlabels);
    m_historyWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

    m_labelsLayout = new QHBoxLayout;
    m_labelsLayout->addWidget(m_titleLabel);
    m_labelsLayout->addWidget(m_clearButton);

    m_historyLayout = new QGridLayout;
    m_historyLayout->addLayout(m_labelsLayout,0,0);
    m_historyLayout->addWidget(m_historyWidget,1,0);

    m_timer = new QTimer(this);
    m_timer->start(1000);

    m_downloadsFile.setFileName(QDir::currentPath() + "/downloads.txt");
    if(!m_downloadsFile.exists())
        return;

    connect(m_clearButton, SIGNAL(clicked()), this, SLOT(clearHistory()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(readMP3sFile()));

    setLayout(m_historyLayout);
}

void TabHistory::clearHistory()
{
    m_historyWidget->clear();

    if(!m_downloadsFile.open(QFile::Append | QFile::Text)) {
        return;
    }

    m_downloadsFile.resize(0);
    m_downloadsFile.close();
}

void TabHistory::readMP3sFile()
{
    m_historyWidget->clear();

    if (!m_downloadsFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&m_downloadsFile);

    while(!stream.atEnd()) {
        QString line = stream.readLine();
        QStringList dataSong = line.split(";");
        QTreeWidgetItem *item = new QTreeWidgetItem;

        for (int i=0; i < 4; i++) {
            QString fileString = dataSong.at(i).toLocal8Bit().constData();
            item->setText(i, fileString);
        }
        m_historyWidget->addTopLevelItem(item);
    }
    m_downloadsFile.close();
}
