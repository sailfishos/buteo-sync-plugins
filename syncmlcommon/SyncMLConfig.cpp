/*
 * This file is part of buteo-sync-plugins package
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Sateesh Kavuri <sateesh.kavuri@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include "SyncMLConfig.h"

#include <QDir>

#include "SyncMLPluginLogging.h"
#include "SyncCommonDefs.h"

const QString XMLDIR( "/etc/buteo/xml/");
const QString DBDIR( "/sync-app/" );
const QString DEVINFO_FILE_NAME("devInfo.xml");

SyncMLConfig::SyncMLConfig()
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);
}

SyncMLConfig::~SyncMLConfig()
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);
}


QString SyncMLConfig::getDatabasePath()
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    QString path = Sync::syncConfigDir() + DBDIR;

    QDir dir( path );
    dir.mkpath( path );

    return path;

}

QString SyncMLConfig::getXmlDataPath()
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    return XMLDIR;
}

QString SyncMLConfig::getDevInfoFile()
{
    return SyncMLConfig::getDatabasePath() + DEVINFO_FILE_NAME;
}

void
SyncMLConfig::syncmlConfigFilePaths (QString& aDefaultConfigFile, QString& aExtConfigFile)
{
    aDefaultConfigFile = "/etc/buteo/meego-syncml-conf.xml";
    aExtConfigFile = "/etc/buteo/ext-syncml-conf.xml";
}
