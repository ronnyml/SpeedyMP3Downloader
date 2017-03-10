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
#include "tabsearch.h"

#include <QtGui>
#include <QtCore>
#include <QDebug>
#include <QNetworkRequest>
#include <QtNetwork>
#include <QMessageBox>

TabSearch::TabSearch(QWidget *parent) :
    QWidget(parent)
{
    m_counting = 0;
    m_searchURL = "http://your-music-server.com/search.xml?q="; // The URL where to find songs, could be something like this.
    m_titleLabel = new QLabel;
    m_titleLabel->setText(tr("Speedy MP3 Downloader: "));

    m_searchButton = new QPushButton;
    m_searchButton->setText(tr("Search"));
    m_searchBox = new QLineEdit;

    m_treeWidget = new QTreeWidget(this);
    m_headerLabels << tr("Song") << tr("Artist") << tr("Duration") << tr("FileName") << tr("URL");
    m_treeWidget->setHeaderLabels(m_headerLabels);
    m_treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);
    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeWidget->setColumnHidden(4,true);

    m_downloadAction = new QAction(this);
    m_downloadAction->setText(tr("Download"));

    m_openDownloadsAction = new QAction(this);
    m_openDownloadsAction->setText(tr("Downloads Folder"));

    m_downloadsLabel = new QLabel;
    m_downloadsLabel->setText(tr("Current downloads: "));

    m_openDownloadsButton = new QPushButton;
    m_openDownloadsButton->setText(tr("Open downloads folder"));
    m_openDownloadsButton->setFixedWidth(200);

    m_downloadsWidget = new QTableWidget(this);
    m_downloadsWidget->setColumnCount(5);
    m_downloadsHeaderLabels << tr("FileName") << tr("Size") << tr("Status") << tr("Progress") << tr("Downloaded");
    m_downloadsWidget->setHorizontalHeaderLabels(m_downloadsHeaderLabels);
    m_downloadsWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    m_downloadsWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    m_labelsLayout = new QHBoxLayout;
    m_labelsLayout->addWidget(m_titleLabel);

    m_itemsLayout = new QHBoxLayout;
    m_itemsLayout->addWidget(m_searchBox);
    m_itemsLayout->addWidget(m_searchButton);

    m_downloadsWidgetsLayout = new QHBoxLayout;
    m_downloadsWidgetsLayout->addWidget(m_downloadsLabel);
    m_downloadsWidgetsLayout->addWidget(m_openDownloadsButton);

    m_downloadsLayout = new QVBoxLayout;
    m_downloadsLayout->addLayout(m_downloadsWidgetsLayout);
    m_downloadsLayout->addWidget(m_downloadsWidget);

    m_searchLayout = new QGridLayout;
    m_searchLayout->addLayout(m_labelsLayout,0,0);
    m_searchLayout->addLayout(m_itemsLayout,1,0);
    m_searchLayout->addWidget(m_treeWidget,2,0);
    m_searchLayout->addLayout(m_downloadsLayout,3,0);

    m_progressDialog = new QProgressDialog;
    m_progressDialog->setCancelButton(0);
    m_progressDialog->setFixedSize(350,50);
    m_progressDialog->setLabelText(tr("Searching..."));
    m_progressDialog->setRange(0,0);
    m_progressDialog->setWindowModality(Qt::WindowModal);

    m_fileSizeItem = new QTableWidgetItem;

    connect(m_downloadsWidget, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showDownloadsContextMenu(const QPoint &)) );
    connect(m_downloadAction, SIGNAL(triggered()), SLOT(downloadMP3RightClicked()) );
    connect(&m_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(finished(QNetworkReply*)) );
    connect(m_openDownloadsAction, SIGNAL(triggered()), SLOT(openDownloadsFolder()) );
    connect(m_openDownloadsButton, SIGNAL(clicked()), this, SLOT(openDownloadsFolder()) );
    connect(m_searchBox, SIGNAL(returnPressed()), this, SLOT(search()) );
    connect(m_searchButton, SIGNAL(clicked()), this, SLOT(search()) );
    connect(m_treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)) );
    connect(m_treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(downloadMP3DoubleClicked(QTreeWidgetItem*)) );
    connect(m_treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(setIndicators(QTreeWidgetItem*)) );

    setLayout(m_searchLayout);
}

QString TabSearch::convertToKB(qint64 bytes)
{
    qreal total = bytes/1024;
    QString totalString = QString::number(total) + " KB";
    return totalString;
}

QString TabSearch::getDuration(const QString songDuration)
{
    QString finalDuration = "";
    int duration = songDuration.toInt();
    int minutes = duration / 60;
    int seconds = duration % 60;

    QString minutesString = QString::number(minutes);
    QString secondsString = "";

    if (seconds < 10) {
        secondsString = "0" + QString::number(seconds);
    } else {
        secondsString = QString::number(seconds);
    }

    finalDuration = minutesString + ":" + secondsString;
    return finalDuration;
}

void TabSearch::downloadMP3(const QString song, const QString artist, const QString mp3)
{
    Downloader *downloader = new Downloader();
    QUrl mp3URL = QUrl::fromEncoded(mp3.toLocal8Bit());
    downloader->doDownload(mp3URL, song, artist);
}

void TabSearch::downloadMP3RightClicked()
{
    QString song = m_treeWidget->currentItem()->text(0);
    QString artist = m_treeWidget->currentItem()->text(1);
    QString mp3 = m_treeWidget->currentItem()->text(4);

    downloadMP3(song, artist, mp3);
    setIndicators(m_treeWidget->currentItem());
}

void TabSearch::downloadMP3DoubleClicked(QTreeWidgetItem *item)
{
    QString song = item->text(0);
    QString artist = item->text(1);
    QString mp3 = item->text(4);

    downloadMP3(song, artist, mp3);
}

void TabSearch::getSongs(const QUrl &url)
{
    QNetworkRequest request(url);
    /*if (currentReply) {
        //currentReply->disconnect(this);
        currentReply->deleteLater();
    }*/

    m_currentReply = m_manager.get(request);
    connect(m_currentReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_currentReply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
    connect(m_currentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

void TabSearch::error(QNetworkReply::NetworkError)
{
    qWarning("Error downloading mp3 file.");
    m_currentReply->disconnect(this);
    m_currentReply->deleteLater();
    m_currentReply = 0;
}

void TabSearch::finished(QNetworkReply *reply)
{
    Q_UNUSED(reply);
    m_searchBox->setReadOnly(false);
    m_searchButton->setEnabled(true);
    m_progressDialog->cancel();
}



void TabSearch::metaDataChanged()
{
    QUrl redirectionTarget = m_currentReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirectionTarget.isValid()) {
        getSongs(redirectionTarget);
    }
}

void TabSearch::openDownloadsFolder()
{
    QString downloadDirPath;
    m_configFile.setFileName(QDir::currentPath() + "/config.txt");
    if ( !m_configFile.open(QIODevice::ReadOnly | QIODevice::Text) )
        return;

    QTextStream stream(&m_configFile);
    QString line = stream.readLine();
    downloadDirPath = line;
    m_configFile.close();

    QDir downloadDir(downloadDirPath);
    if (!downloadDir.exists())
        downloadDir.mkdir(downloadDirPath);

    QDesktopServices services;
    services.openUrl(QUrl::fromLocalFile(downloadDirPath));
}

void TabSearch::parseXml()
{
    while (!m_xml.atEnd()) {
        m_xml.readNext();
        if (m_xml.isStartElement()) {
            if (m_xml.name() == "item")
                m_songArtist = m_xml.attributes().value("rss:about").toString();
            m_currentTag = m_xml.name().toString();
        } else if (m_xml.isEndElement()) {
            if (m_xml.name() == "item") {
                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, m_songName);
                item->setText(1, m_songArtist);
                item->setText(2, m_songDuration);
                item->setText(3, m_songMP3);
                item->setText(4, m_songURL);
                m_treeWidget->addTopLevelItem(item);
                m_songName.clear();
                m_songArtist.clear();
                m_songDuration.clear();
                m_songMP3.clear();
                m_songURL.clear();
            }
        } else if (m_xml.isCharacters() && !m_xml.isWhitespace()) {
            if (m_currentTag == "name") {
                m_songName += m_xml.text().toString();
            } else if (m_currentTag == "artist") {
                m_songArtist += m_xml.text().toString();
            } else if (m_currentTag == "duration") {
                m_songDuration += getDuration(m_xml.text().toString());
            } else if (m_currentTag == "mp3") {
                m_songURL += m_xml.text().toString();
                m_songMP3 += m_downloader.saveFileName(m_xml.text().toString());
            }
        }
    }

    if (m_xml.error() && m_xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        qWarning() << "XML ERROR:" << m_xml.lineNumber() << ": " << m_xml.errorString();
    }
}

void TabSearch::readyRead()
{
    int statusCode = m_currentReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
        QByteArray data = m_currentReply->readAll();
        m_xml.addData(data);
        parseXml();
    }
}

void TabSearch::search()
{
    if (m_searchBox->text() == "" || m_searchBox->text().length() == 0) {
        QMessageBox searchBoxMessage;
        searchBoxMessage.setText(tr("You need to enter an artist name or song."));
        searchBoxMessage.exec();
    } else {
        m_progressDialog->show();
        m_searchBox->setReadOnly(true);
        m_searchButton->setEnabled(false);
        m_treeWidget->clear();
        m_xml.clear();

        QString finalURL = m_searchURL + m_searchBox->text();
        QUrl url(finalURL);
        getSongs(url);
    }
}

void TabSearch::setDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    QString totalFileSize = convertToKB(bytesTotal);
    m_downloadBar->setMaximum(bytesTotal);
    m_downloadBar->setValue(bytesReceived);

    QString downloading = convertToKB(bytesReceived) + " of " + convertToKB(bytesTotal);
    QTableWidgetItem *fileDownloadedItem = new QTableWidgetItem;
    fileDownloadedItem->setFlags(fileDownloadedItem->flags() & (~Qt::ItemIsEditable));
    fileDownloadedItem->setText(downloading);
    m_downloadsWidget->setItem(m_downloadsWidget->rowCount()-1,4,fileDownloadedItem);

    if (m_counting == 0) {
        QTableWidgetItem *fileSizeItem = new QTableWidgetItem;
        fileSizeItem->setFlags(fileSizeItem->flags() & (~Qt::ItemIsEditable));
        fileSizeItem->setText(totalFileSize);
        m_downloadsWidget->setItem(m_downloadsWidget->rowCount()-1,1,fileSizeItem);
        m_downloadsWidget->setCellWidget(m_downloadsWidget->rowCount()-1,3,m_downloadBar);

        m_counting++;
    }
}

void TabSearch::setIndicators(QTreeWidgetItem *item)
{
    m_treeWidget->setEnabled(false);
    m_counting = 0;
    m_downloadsWidget->insertRow(m_downloadsWidget->rowCount());
    m_downloadBar = new QProgressBar;
    m_downloadBar->setOrientation(Qt::Horizontal);
    m_downloadBar->setFixedWidth(300);
    m_downloadBar->setMinimum(0);

    QString progressBarStylesheet = QString (
                   "QProgressBar::chunk {"
                       "background-color: #3399ff;"
                   "}");

    progressBarStylesheet.append("QProgressBar {"
                                   "border: 2px solid grey;"
                                   "border-radius: 2px;"
                                   "text-align: right;"
                                   "background: #FFFFFF;"
                                 "}");
    m_downloadBar->setStyleSheet(progressBarStylesheet);

    int nro_columns = item->columnCount();
    for (int i=0; i < nro_columns; i++) {
        item->setBackgroundColor(i,QColor(Qt::darkBlue));
        item->setTextColor(i, QColor(Qt::white));
    }

    QString mp3 = item->text(4);
    QUrl mp3URL = QUrl::fromEncoded(mp3.toLocal8Bit());
    m_fileDownloading = item->text(0);

    QTableWidgetItem *fileNameItem = new QTableWidgetItem;
    fileNameItem->setFlags(fileNameItem->flags() & (~Qt::ItemIsEditable));
    fileNameItem->setText(m_fileDownloading);
    m_downloadsWidget->setItem(m_downloadsWidget->rowCount()-1,0,fileNameItem);

    QTableWidgetItem *fileStatusItem = new QTableWidgetItem;
    fileStatusItem->setFlags(fileStatusItem->flags() & (~Qt::ItemIsEditable));
    fileStatusItem->setText(tr("Downloading..."));
    m_downloadsWidget->setItem(m_downloadsWidget->rowCount()-1,2,fileStatusItem);

    QNetworkRequest request(mp3URL);
    QNetworkReply *downloadReply = m_urlManager.get(request);
    connect(downloadReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(setDownloadProgress(qint64,qint64)));
    connect(downloadReply, SIGNAL(finished()), this, SLOT(setIndicatorsFinished()));
}

void TabSearch::setIndicatorsFinished()
{
    m_treeWidget->setEnabled(true);
    QTableWidgetItem *fileStatusItem = new QTableWidgetItem;
    fileStatusItem->setFlags(fileStatusItem->flags() & (~Qt::ItemIsEditable));
    fileStatusItem->setText("Completed");

    m_downloadsWidget->setItem(m_downloadsWidget->rowCount()-1, 2, fileStatusItem);
    QString finishedStylesheet = QString (
                   "QProgressBar::chunk {"
                       "background-color: #008000;"
                   "}");

    finishedStylesheet.append("QProgressBar {"
                                   "border: 2px solid grey;"
                                   "border-radius: 2px;"
                                   "text-align: right;"
                                   "background: #FFFFFF;"
                                 "}");
    m_downloadBar->setStyleSheet(finishedStylesheet);
}

void TabSearch::showContextMenu(const QPoint &pos)
{
      QTreeWidgetItem *item = m_treeWidget->itemAt(pos);
      if (!item)
         return;

      QMenu songMenu(tr("Files Context Menu"), this);
      songMenu.addAction(m_downloadAction);
      songMenu.exec(m_treeWidget->viewport()->mapToGlobal(pos));
}

void TabSearch::showDownloadsContextMenu(const QPoint &pos)
{
      QTableWidgetItem *item = m_downloadsWidget->itemAt(pos);
      if (!item)
         return;

      QMenu songMenu(tr("Downloads Context Menu"), this);
      songMenu.addAction(m_openDownloadsAction);
      songMenu.exec(m_downloadsWidget->viewport()->mapToGlobal(pos));
}
