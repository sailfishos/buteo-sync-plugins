/*
 * This file is part of buteo-sync-plugins package
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * Copyright (C) 2013 - 2021 Jolla Ltd.
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
#ifndef CONTACTSCHANGENOTIFIERPLUGIN_H
#define CONTACTSCHANGENOTIFIERPLUGIN_H

#include "StorageChangeNotifierPlugin.h"
#include "StorageChangeNotifierPluginLoader.h"

class ContactsChangeNotifier;

class ContactsChangeNotifierPlugin : public Buteo::StorageChangeNotifierPlugin
{
    Q_OBJECT

public:
    /*! \brief constructor
     * see StorageChangeNotifierPlugin
     */
    ContactsChangeNotifierPlugin(const QString& aStorageName);

    /*! \brief destructor
     */
    ~ContactsChangeNotifierPlugin();

    /*! \brief see StorageChangeNotifierPlugin::name
     */
    QString name() const;

    /*! \brief see StorageChangeNotifierPlugin::hasChanges
     */
    bool hasChanges() const;

    /*! \brief see StorageChangeNotifierPlugin::changesReceived
     */
    void changesReceived();

    /*! \brief see StorageChangeNotifierPlugin::enable
     */
    void enable();

    /*! \brief see StorageChangeNotifierPlugin::disable
     */
    void disable(bool disableAfterNextChange = false);

private Q_SLOTS:
    /*! \brief handles a change notification from contacts notifier
     */
    void onChange();

private:
    ContactsChangeNotifier* icontactsChangeNotifier;
    bool ihasChanges;
    bool iDisableLater;
};


class ContactsChangeNotifierPluginLoader : public Buteo::StorageChangeNotifierPluginLoader
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.buteo.plugins.storage.ContactsChangeNotifierPluginLoader")
    Q_INTERFACES(Buteo::StorageChangeNotifierPluginLoader)

public:
    Buteo::StorageChangeNotifierPlugin* createPlugin(const QString& aStorageName) override;
};

#endif
