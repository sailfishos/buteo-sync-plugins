/*
* This file is part of buteo-sync-plugins package
*
* Copyright (C) 2013 Jolla Ltd. and/or its subsidiary(-ies).
*
* Author: Sateesh Kavuri <sateesh.kavuri@gmail.com>
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
*/
#ifndef SYNCMLSERVER_H
#define SYNCMLSERVER_H

#include "syncmlserver_global.h"
#include "USBConnection.h"
#include "BTConnection.h"
#include "SyncMLStorageProvider.h"

#include <buteosyncfw5/ServerPlugin.h>
#include <buteosyncfw5/SyncPluginLoader.h>
#include <buteosyncfw5/SyncCommonDefs.h>
#include <buteosyncfw5/SyncResults.h>
#include <buteosyncml5/StorageProvider.h>
#include <buteosyncml5/SyncAgent.h>
#include <buteosyncml5/Transport.h>
#include <buteosyncml5/SyncAgentConfig.h>
#include <buteosyncml5/OBEXTransport.h>

namespace Buteo {
    class ServerPlugin;
    class Profile;
}

namespace DataSync {
    class SyncAgent;
    class Transport;
}
class SYNCMLSERVERSHARED_EXPORT SyncMLServer : public Buteo::ServerPlugin
{
    Q_OBJECT

public:
    SyncMLServer (const QString& pluginName,
                  const Buteo::Profile profile,
                  Buteo::PluginCbInterface *cbInterface);

    virtual ~SyncMLServer ();

    virtual bool init ();

    virtual bool uninit ();

    virtual void abortSync (Sync::SyncStatus status = Sync::SYNC_ABORTED);

    virtual bool cleanUp ();

    virtual Buteo::SyncResults getSyncResults () const;

    virtual bool startListen ();

    virtual void stopListen ();

    virtual void suspend ();

    virtual void resume ();

signals:

    void syncFinished (Sync::SyncStatus);
    
    void sessionInProgress (Sync::ConnectivityType);


public slots:

    virtual void connectivityStateChanged (Sync::ConnectivityType type, bool state);

protected slots:

    void handleUSBConnected (int fd);

    void handleBTConnected (int fd, QString btAddr);

    void handleSyncFinished (DataSync::SyncState state);

    void handleStateChanged (DataSync::SyncState state);

    void handleStorageAccquired (QString storageType);

    void handleItemProcessed (DataSync::ModificationType modificationType,
                              DataSync::ModifiedDatabase modifiedDb,
                              QString localDb,
                              QString dbType, int committedItems);
private:

    bool initSyncAgent ();

    void closeSyncAgent ();

    void closeUSBTransport ();
    
    void closeBTTransport ();

    DataSync::SyncAgentConfig *initSyncAgentConfig ();

    void closeSyncAgentConfig ();

    bool initStorageProvider ();

    bool createUSBTransport ();
    
    bool createBTTransport ();

    bool startNewSession (QString address);

    void generateResults (bool success);

    QMap<QString, QString>          mProperties;

    DataSync::SyncAgent*            mAgent;

    DataSync::SyncAgentConfig*      mConfig;

    USBConnection                   mUSBConnection;

    BTConnection                    mBTConnection;

    DataSync::Transport*            mTransport;

    Buteo::SyncResults              mResults;

    SyncMLStorageProvider           mStorageProvider;

    qint32                          mCommittedItems;

    /**
      * ! \brief The connectivity type used for the current sync session
      */
    Sync::ConnectivityType          mConnectionType;
    
    /**
      * ! \brief Flag to indicate if the sync session is in progress
      */
    bool                            mIsSessionInProgress;
    
    /**
      * ! \brief Flag to indicate if bluetooth is active
      */
    bool                            mBTActive;
    
    /**
      * ! \brief Flag to indicate if USB is active
      */
    bool                            mUSBActive;
};

class SyncMLServerLoader : public Buteo::SyncPluginLoader
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.buteo.plugins.sync.SyncMLServerLoader")
    Q_INTERFACES(Buteo::SyncPluginLoader)

public:
    /*! \brief Creates SyncML server plugin
     *
     * @param aPluginName Name of this server plugin
     * @param aProfile Profile to use
     * @param aCbInterface Pointer to the callback interface
     * @return Server plugin on success, otherwise NULL
     */
    Buteo::ServerPlugin* createServerPlugin(const QString& pluginName,
                                            const Buteo::Profile& profile,
                                            Buteo::PluginCbInterface* cbInterface) override;
};



#endif // SYNCMLSERVER_H
