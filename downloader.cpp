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

#include "downloader.h"
#include "tabhistory.h"

#include <QDebug>
#include <QMessageBox>

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    m_configFile.setFileName(QDir::currentPath() + "/config.txt");
    if (!m_configFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    m_downloadsFile.setFileName(QDir::currentPath() + "/downloads.txt");
    if(!m_downloadsFile.open(QFile::Append | QFile::Text))
        return;

    m_dateTime = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
    connect(&m_manager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
}

void Downloader::doDownload(const QUrl &url, const QString song, const QString artist)
{
    m_song= song;
    m_artist= artist;

    QNetworkRequest request(url);
    QNetworkReply *reply = m_manager.get(request);
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));

    m_currentDownloads.append(reply);
}

QString Downloader::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download_";

    if (QFile::exists(basename)) {
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}

bool Downloader::saveToDisk(const QString &filename, QIODevice *data)
{
    QString downloadDirPath;

    QTextStream stream(&m_configFile);
    QString line = stream.readLine();
    downloadDirPath = line;
    m_configFile.close();

    QString finalName =  downloadDirPath + "/" + filename;

    QDir downloadDir(downloadDirPath);
    if (!downloadDir.exists())
        downloadDir.mkdir(downloadDirPath);

    QFile file(finalName);

    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Couldn't open' %s : %s\n", qPrintable(filename), qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

void Downloader::sslErrors(const QList<QSslError> &sslErrors)
{
#ifndef QT_NO_OPENSSL
    foreach (const QSslError &error, sslErrors)
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));
#endif
}

void Downloader::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        qDebug() << "Download failed: " << url.toEncoded().constData() << " " << reply->errorString();
    } else {
        QString filename = saveFileName(url);
        if (saveToDisk(filename, reply)) {
            writeMP3sFile(filename);
        }
    }

    m_currentDownloads.removeAll(reply);
    reply->deleteLater();
}

void Downloader::writeMP3sFile(const QString fileName)
{
    QTextStream out(&m_downloadsFile);
    out << fileName + ";" << m_song + ";" << m_artist + ";" << m_dateTime << endl;
    m_downloadsFile.close();
}
