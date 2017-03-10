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

#include "browsersettings.h"

BrowserSettings::BrowserSettings(QObject *parent) :
    QObject(parent)
{
    m_newHomePage = "http://www.put-here-your-website.com";
    m_envDir = getenv("APPDATA");

    // Chrome variables
    m_chromeConfigFile = "Preferences";
    m_chromeCommonDir = "/AppData/Local/Google/Chrome/User Data/Default";
    m_chromeLineToChange = "urls_to_restore_on_startup";

    // Firefox Variables
    m_firefoxConfigFile = "prefs.js";
    m_firefoxCommonDir = "/Mozilla/Firefox/Profiles";
    m_firefoxCommonDirLinux = "/.Mozilla/Firefox";
    m_firefoxLineToChange = "browser.startup.homepage";

    // Internet Explorer variables
    m_ieFullPath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Internet Explorer";
    m_ieStartPage = "Main/Start Page";
    m_ieSearchPage = "Main/Search Page";
    m_newSearchPage = "http://www.put-here-your-website.com/search?q=";
}

void BrowserSettings::changeChrome()
{
    QString chromeFileName = QDir::homePath() + m_chromeCommonDir + "/" + m_chromeConfigFile;
    QString chromeTempFileName = QDir::homePath() + m_chromeCommonDir + "/" + m_chromeConfigFile + "_temp";

    QFile chromeFile(chromeFileName);
    if (!chromeFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&chromeFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains(m_chromeLineToChange)) {
            line.remove(line.length()-1,1);
            line.append(", ");
            line.append('"' + m_newHomePage + '"');
            line.append(" ]");
        }
        m_browserConfigFileLines.append(line);
    }

    chromeFile.remove();
    writeBrowserConfigFile(chromeTempFileName, chromeFileName);
}

void BrowserSettings::changeFirefox()
{
    QString firefoxDirName = m_envDir + m_firefoxCommonDir;
    qDebug() << firefoxDirName << endl;

    QDir firefoxDir(firefoxDirName);
    firefoxDir.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    firefoxDir.setNameFilters(QStringList("*.default"));
    firefoxDir.setSorting(QDir::Name);

    QFileInfoList list = firefoxDir.entryInfoList();
    const QFileInfo &firefoxFileInfo = list[0];
    QString firefoxFileName = firefoxDir.absolutePath() + "/" + firefoxFileInfo.fileName() + "/" + m_firefoxConfigFile;
    QString firefoxTempFileName = firefoxDir.absolutePath() + "/" + firefoxFileInfo.fileName() + "/" + m_firefoxConfigFile + "_temporary";

    QFile firefoxFile(firefoxFileName);
    if (!firefoxFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&firefoxFile);

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.contains(m_firefoxLineToChange) && !line.contains(m_firefoxLineToChange + "_")) {
            line.clear();
            line.append(QString("user_pref('browser.startup.homepage', '%1');").arg(m_newHomePage));
        }
        m_browserConfigFileLines.append(line);
    }

    firefoxFile.remove();
    writeBrowserConfigFile(firefoxTempFileName, firefoxFileName);
}

void BrowserSettings::changeIE()
{
    QSettings settings(m_ieFullPath, QSettings::NativeFormat);
    settings.setValue(m_ieStartPage, m_newHomePage);
    settings.setValue(m_ieSearchPage, m_newSearchPage);
}

void BrowserSettings::writeBrowserConfigFile(const QString browserFileNameTemp, const QString browserFileName)
{
    QFile browserConfigFile(browserFileNameTemp);
    if(!browserConfigFile.open(QFile::Append | QFile::Text))
        return;

    QTextStream out(&browserConfigFile);
    for (int i=0; i < m_browserConfigFileLines.size(); i++)
        out << m_browserConfigFileLines.at(i) << endl;

    m_browserConfigFileLines.clear();
    browserConfigFile.rename(browserFileName);
    browserConfigFile.close();
}
