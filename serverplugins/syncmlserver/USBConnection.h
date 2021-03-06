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
#ifndef USBCONNECTION_H
#define USBCONNECTION_H

#include <QObject>
#include <QMutex>

#ifdef GLIB_FD_WATCH
#include <glib.h>
#else
#include <QSocketNotifier>
#endif

#include <buteosyncml5/OBEXConnection.h>

/*! \brief Class for creating connection to a PC that acts as a USB
 *         host for synchronization of data using buteosyncml
 *
 */
class USBConnection : public QObject, public DataSync::OBEXConnection
{
    Q_OBJECT

public:

    USBConnection ();

    virtual ~USBConnection ();

    /*! \sa DataSync::OBEXConnection::connect ()
     *
     */
    virtual int connect ();

    /*! \sa DataSync::OBEXConnection::isConnected ()
     *
     */
    virtual bool isConnected () const;

    /*! \sa DataSync::OBEXConnection::disconnect ()
     *
     */
    virtual void disconnect ();

    void handleSyncFinished (bool isSyncInError);

signals:

    void usbConnected (int fd);

#ifndef GLIB_FD_WATCH
protected slots:

    void handleUSBActivated (int fd);

    void handleUSBError (int fd);
#endif

private:
    // Functions

    int openUSBDevice ();

    void closeUSBDevice ();

    void addFdListener ();

    void removeFdListener ();

    void signalNewSession ();

#ifdef GLIB_FD_WATCH
    void setFdWatchEventSource (guint = 0);

    void setIdleEventSource (guint = 0);

    guint fdWatchEventSource ();

    guint idleEventSource ();

    static gboolean handleIncomingUSBEvent (GIOChannel* ioChannel,
                                            GIOCondition condition,
                                            gpointer user_data);

    void removeEventSource ();

    static gboolean reopenUSB (gpointer data);

#endif
private:

    int                     mFd;

    QMutex                  mMutex;

    bool                    mDisconnected;

    bool                    mFdWatching;

#ifdef GLIB_FD_WATCH
    GIOChannel              *mIOChannel;

    guint                   mIdleEventSource;

    guint                   mFdWatchEventSource;
#else
    QSocketNotifier         *mReadNotifier;

    QSocketNotifier         *mWriteNotifier;

    QSocketNotifier         *mExceptionNotifier;
#endif
};

#endif // USBCONNECTION_H
