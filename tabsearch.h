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

#ifndef TABSEARCH_H
#define TABSEARCH_H

#include "downloader.h"

#include <QAction>
#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProgressBar>
#include <QProgressDialog>
#include <QPushButton>
#include <QTableWidget>
#include <QTreeWidget>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include <QXmlStreamReader>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class TabSearch : public QWidget
{
    Q_OBJECT

public:
    explicit TabSearch(QWidget *parent = 0);

    QString convertToKB(qint64 bytes);
    QString getDuration(const QString duration);
    void getSongs(const QUrl &url);
    void parseXml();

private:
    int m_counting;
    Downloader m_downloader;

    QAction *m_downloadAction;
    QAction *m_openDownloadsAction;
    QFile m_configFile;
    QGridLayout *m_searchLayout;
    QHBoxLayout *m_downloadsWidgetsLayout;
    QHBoxLayout *m_itemsLayout;
    QHBoxLayout *m_labelsLayout;
    QLabel *m_downloadsLabel;
    QLabel *m_titleLabel;
    QLineEdit *m_searchBox;
    QNetworkReply *m_currentReply;
    QNetworkAccessManager m_manager;
    QNetworkAccessManager m_urlManager;
    QProgressBar *m_downloadBar;
    QProgressDialog *m_progressDialog;
    QPushButton *m_searchButton;
    QPushButton *m_openDownloadsButton;
    QString m_currentTag;
    QString m_fileDownloading;
    QString m_searchURL;
    QString m_songArtist;
    QString m_songDuration;
    QString m_songMP3;
    QString m_songName;
    QString m_songSize;
    QString m_songURL;
    QStringList m_downloadsHeaderLabels;
    QStringList m_headerLabels;
    QTableWidget *m_downloadsWidget;
    QTableWidgetItem *m_fileSizeItem;
    QTreeWidget *m_treeWidget;
    QVBoxLayout *m_downloadsLayout;
    QXmlStreamReader m_xml;

public slots:
    void downloadMP3DoubleClicked(QTreeWidgetItem * item);
    void downloadMP3RightClicked();
    void downloadMP3(const QString song, const QString artist, const QString mp3);
    void error(QNetworkReply::NetworkError);
    void finished(QNetworkReply *reply);
    void metaDataChanged();
    void openDownloadsFolder();
    void readyRead();
    void search();
    void setDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void setIndicators(QTreeWidgetItem * item);
    void setIndicatorsFinished();
    void showContextMenu(const QPoint &pos);
    void showDownloadsContextMenu(const QPoint &pos);
    
};

#endif // TABSEARCH_H
