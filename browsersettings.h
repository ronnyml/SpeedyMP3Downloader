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

#ifndef BROWSERSETTINGS_H
#define BROWSERSETTINGS_H

#include <QDebug>
#include <QDir>
#include <QObject>
#include <QSettings>
#include <QTextStream>

class BrowserSettings : public QObject
{
    Q_OBJECT

public:
    explicit BrowserSettings(QObject *parent = 0);

    void changeChrome();
    void changeIE();
    void changeFirefox();
    void writeBrowserConfigFile(const QString browserFileNameTemp, const QString browserFileName);

private:
    QString m_chromeConfigFile;
    QString m_chromeCommonDir;
    QString m_chromeLineToChange;
    QString m_envDir;
    QString m_ieFullPath;
    QString m_ieStartPage;
    QString m_ieSearchPage;
    QString m_firefoxConfigFile;
    QString m_firefoxCommonDir;
    QString m_firefoxCommonDirLinux;
    QString m_firefoxLineToChange;
    QString m_newHomePage;
    QString m_newSearchPage;
    QStringList m_browserConfigFileLines;

};

#endif // BROWSERSETTINGS_H
