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

#include "mp3download.h"

MP3Download::MP3Download(QWidget *parent)
    : QWidget(parent)
{
    tabSearch = new TabSearch;
    tabHistory = new TabHistory;

    m_tabWidget = new QTabWidget;
    m_tabWidget->addTab(tabSearch, tr("Search "));
    m_tabWidget->addTab(tabHistory, tr("History "));
    m_tabWidget->setCurrentIndex(0);

    m_htmlPubli = "<html><body bgcolor='#F0F0F0'><div align='center'><iframe src='http://your-ads.server.com/ad.html' width='730' height='90' frameborder='0' scrolling='no'></iframe></div></body></html>";

    m_view = new QWebView;
    m_view->setFixedHeight(100);
    m_view->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    m_view->setHtml(m_htmlPubli);
    m_view->show();

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->addWidget(m_tabWidget,0,0);
    m_mainLayout->addWidget(m_view,1,0);

    connect(m_view, SIGNAL(linkClicked(QUrl)), this, SLOT(openPubliUrl(QUrl)));

    setWindowTitle(tr("Speedy MP3 Downloader"));
    setLayout(m_mainLayout);
}

void MP3Download::openPubliUrl(const QUrl publiUrl)
{
    QDesktopServices services;
    services.openUrl(publiUrl);
}
