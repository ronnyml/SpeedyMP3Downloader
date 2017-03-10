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

#ifndef TABHISTORY_H
#define TABHISTORY_H

#include <QDir>
#include <QFile>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QTreeWidget>
#include <QWidget>


class TabHistory : public QWidget
{
    Q_OBJECT

public:
    explicit TabHistory(QWidget *parent = 0);

private:
    QFile m_downloadsFile;
    QGridLayout *m_historyLayout;
    QHBoxLayout *m_labelsLayout;
    QLabel *m_titleLabel;
    QPushButton *m_clearButton;
    QStringList m_hlabels;
    QTimer *m_timer;
    QTreeWidget *m_historyWidget;

public slots:
    void readMP3sFile();
    void clearHistory();

};

#endif // TABHISTORY_H
