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

#ifndef MP3DOWNLOAD_H
#define MP3DOWNLOAD_H

#include "tabhistory.h"
#include "tabsearch.h"

#include <QtGui/QWidget>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QWebFrame>
#include <QWebView>


class MP3Download : public QWidget
{
    Q_OBJECT
    
public:
    MP3Download(QWidget *parent = 0);
    TabSearch *tabSearch;
    TabHistory *tabHistory;

private:
    QString m_htmlPubli;
    QTabWidget *m_tabWidget;
    QVBoxLayout *m_mainLayout;
    QWebView *m_view;

public slots:
    void openPubliUrl(const QUrl url);

};

#endif // MP3DOWNLOAD_H
