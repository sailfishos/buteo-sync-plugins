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

#include "CalendarBackend.h"
#include <KCalendarCore/Event>
#include <KCalendarCore/Journal>
#include "SyncMLPluginLogging.h"
#include <QDir>
#include <QDebug>

CalendarBackend::CalendarBackend() : iCalendar( 0 ), iStorage( 0 )
{
	FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);
}

CalendarBackend::~CalendarBackend()
{
	FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);
}

bool CalendarBackend::init(const QString &aNotebookName, const QString& aUid)
{
	FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    if( aNotebookName.isEmpty() )
    {
    	qCDebug(lcSyncMLPlugin) << "NoteBook Name to Sync is expected. It Cannot be Empty";
        return false;
    }

    iNotebookStr = aNotebookName;

    iCalendar = mKCal::ExtendedCalendar::Ptr( new mKCal::ExtendedCalendar( QTimeZone::systemTimeZone()) );

    qCDebug(lcSyncMLPlugin) << "Creating Default Maemo Storage";
    iStorage = iCalendar->defaultStorage( iCalendar );

    bool opened = iStorage->open();
    if(!opened)
    {
        qCDebug(lcSyncMLPluginTrace) << "Calendar storage open failed";
    }

    mKCal::Notebook::Ptr openedNb;

    // If we have an Uid, we try to get the corresponding Notebook
    if (!aUid.isEmpty()) {
        openedNb = iStorage->notebook(aUid);

        // If we didn't get one, we create one and set its Uid
        if (!openedNb) {
            openedNb = mKCal::Notebook::Ptr(new mKCal::Notebook(aNotebookName,
                                                                "Synchronization Created Notebook for " + aNotebookName));
            if (!openedNb.isNull()) {
                openedNb->setUid(aUid);
                if (!iStorage->addNotebook(openedNb)) {
                    qCWarning(lcSyncMLPlugin) << "Failed to add notebook to storage";
                    openedNb.clear();
                }
            }
        }
    }

    // If we didn't have an Uid or the creation above failed,
    // we use the default notebook
    if (openedNb.isNull()) {
        openedNb = iStorage->defaultNotebook();
        if(openedNb.isNull()) {
            qCDebug(lcSyncMLPlugin) << "No default notebook exists, creating one";
            openedNb = mKCal::Notebook::Ptr(new mKCal::Notebook("Default", QString()));
            if (!iStorage->setDefaultNotebook(openedNb)) {
                qCWarning(lcSyncMLPlugin) << "Failed to set default notebook of storage";
                openedNb.clear();
            }
        }
    }

    bool loaded = false;
    if(opened && openedNb)
    {
        qCDebug(lcSyncMLPlugin) << "Loading all incidences from::" << openedNb->uid();
        loaded = iStorage->loadNotebookIncidences(openedNb->uid());
        if(!loaded)
        {
            qCWarning(lcSyncMLPlugin) << "Failed to load calendar!";
        }
    }

    if (opened && loaded && !openedNb.isNull())
    {
        iNotebookStr = openedNb->uid();

        qCDebug(lcSyncMLPlugin) << "Calendar initialized";
        return true;
    }
    else
    {
        qCWarning(lcSyncMLPlugin) << "Not able to initialize calendar";

        iStorage.clear();

        qCDebug(lcSyncMLPluginTrace) << "Storage deleted";

        iCalendar.clear();

        qCDebug(lcSyncMLPluginTrace) << "Calendar deleted";

        return false;
    }
}

bool CalendarBackend::uninit()
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    if( iStorage ) {
        qCDebug(lcSyncMLPluginTrace) << "Closing calendar storage...";
        iStorage->close();
        qCDebug(lcSyncMLPluginTrace) << "Done";
        iStorage.clear();
    }

    if( iCalendar ) {
        qCDebug(lcSyncMLPluginTrace) << "Closing calendar...";
        iCalendar->close();
        qCDebug(lcSyncMLPluginTrace) << "Done";
        iCalendar.clear();
    }

    return true;
}

bool CalendarBackend::getAllIncidences( KCalendarCore::Incidence::List& aIncidences )
{
	FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

	if( !iStorage ) {
	    return false;
	}

	if( !iStorage->allIncidences( &aIncidences, iNotebookStr ) ) {
        qCWarning(lcSyncMLPlugin) << "Error Retrieving ALL Incidences from the  Storage ";
        return false;
	}

    filterIncidences( aIncidences );
    return true;
}

void CalendarBackend::filterIncidences(KCalendarCore::Incidence::List& aList)
{
	FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);
	QString event(INCIDENCE_TYPE_EVENT);
	QString todo(INCIDENCE_TYPE_TODO);

	for (int i = 0; i < aList.size(); ++i) {
        KCalendarCore::Incidence::Ptr incidence = aList.at(i);
        if ((incidence->type() != KCalendarCore::Incidence::TypeEvent) && (incidence->type() != KCalendarCore::Incidence::TypeTodo)) {
	        qCDebug(lcSyncMLPlugin) << "Removing incidence type" << incidence->typeStr();
                aList.remove( i, 1);
		incidence.clear();
	    }
        }
}

bool CalendarBackend::getAllNew( KCalendarCore::Incidence::List& aIncidences, const QDateTime& aTime )
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    if( !iStorage ) {
        return false;
    }

    if( !iStorage->insertedIncidences( &aIncidences, aTime, iNotebookStr) ) {
        qCWarning(lcSyncMLPlugin) << "Error Retrieving New Incidences from the Storage";
        return false;
    }

    filterIncidences( aIncidences );
    return true;
}

bool CalendarBackend::getAllModified( KCalendarCore::Incidence::List& aIncidences, const QDateTime& aTime )
{
	FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    if( !iStorage ) {
        return false;
    }

    if( !iStorage->modifiedIncidences( &aIncidences, aTime, iNotebookStr ) ) {
        qCWarning(lcSyncMLPlugin) << " Error retrieving modified Incidences ";
        return false;
    }

    filterIncidences( aIncidences );
    return true;
}

bool CalendarBackend::getAllDeleted( KCalendarCore::Incidence::List& aIncidences, const QDateTime& aTime )
{
	FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    if( !iStorage ) {
        return false;
    }

    if( !iStorage->deletedIncidences( &aIncidences, aTime, iNotebookStr ) ) {
        qCWarning(lcSyncMLPlugin) << " Error retrieving deleted Incidences ";
        return false;
    }

    filterIncidences( aIncidences );
    return true;
}

KCalendarCore::Incidence::Ptr CalendarBackend::getIncidence( const QString& aUID )
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    QStringList iDs = aUID.split(ID_SEPARATOR);
    KCalendarCore::Incidence::Ptr incidence;
    if (iDs.size() == 2) {
       iStorage->load ( iDs.at(0), QDateTime::fromString(iDs.at(1), Qt::ISODate) );
       incidence = iCalendar->incidence(iDs.at(0), QDateTime::fromString(iDs.at(1), Qt::ISODate));
    } else {
	   iStorage->load ( aUID );
	   incidence = iCalendar->incidence( aUID );
    }
    return incidence;
}

QString CalendarBackend::getVCalString(KCalendarCore::Incidence::Ptr aInci)
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    Q_ASSERT( aInci );

    QString vcal;

    KCalendarCore::Incidence::Ptr temp = KCalendarCore::Incidence::Ptr ( aInci->clone() );
    if(temp) {
    KCalendarCore::Calendar::Ptr tempCalendar( new KCalendarCore::MemoryCalendar( QTimeZone::utc() ) );
	tempCalendar->addIncidence(temp);
        KCalendarCore::VCalFormat vcf;
        vcal = vcf.toString(tempCalendar);
    }
    else {
    	qCWarning(lcSyncMLPlugin) << "Error Cloning the Incidence for VCal String";
    }

    return vcal;
}

QString CalendarBackend::getICalString(KCalendarCore::Incidence::Ptr aInci)
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    Q_ASSERT( aInci );

    KCalendarCore::Incidence::Ptr temp = KCalendarCore::Incidence::Ptr ( aInci->clone() );

    QString ical;
    if( temp ) {
    KCalendarCore::Calendar::Ptr tempCalendar( new KCalendarCore::MemoryCalendar( QTimeZone::utc() ) );
	tempCalendar->addIncidence(temp);
    KCalendarCore::ICalFormat icf;
	ical = icf.toString(tempCalendar);
    }
    else {
    	qCWarning(lcSyncMLPlugin) << "Error Cloning the Incidence for Ical String";
    }

    return ical;
}

KCalendarCore::Incidence::Ptr CalendarBackend::getIncidenceFromVcal( const QString& aVString )
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    KCalendarCore::Incidence::Ptr pInci;

    KCalendarCore::Calendar::Ptr tempCalendar( new KCalendarCore::MemoryCalendar( QTimeZone::systemTimeZone()) );
    KCalendarCore::VCalFormat vcf;
    vcf.fromString(tempCalendar, aVString);
    KCalendarCore::Incidence::List lst = tempCalendar->rawIncidences();

    if(!lst.isEmpty()) {
        pInci = KCalendarCore::Incidence::Ptr ( lst[0]->clone() );
    }
    else {
        qCWarning(lcSyncMLPlugin) << "VCal to Incidence Conversion Failed ";
    }
    return pInci;
}

KCalendarCore::Incidence::Ptr CalendarBackend::getIncidenceFromIcal( const QString& aIString )
{
	FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    KCalendarCore::Incidence::Ptr pInci;

    KCalendarCore::Calendar::Ptr tempCalendar( new KCalendarCore::MemoryCalendar( QTimeZone::systemTimeZone()) );
    KCalendarCore::ICalFormat icf;
    icf.fromString(tempCalendar, aIString);
    KCalendarCore::Incidence::List lst = tempCalendar->rawIncidences();

    if(!lst.isEmpty()) {
        pInci = KCalendarCore::Incidence::Ptr ( lst[0]->clone() );
    } else {
    	qCWarning(lcSyncMLPlugin) << "ICal to Incidence Conversion Failed ";
    }

    return pInci;
}

bool CalendarBackend::addIncidence( KCalendarCore::Incidence::Ptr aInci, bool commitNow )
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    if( !iCalendar || !iStorage ) {
        return false;
    }

    switch(aInci->type())
    {
        case KCalendarCore::Incidence::TypeEvent:
            {
                KCalendarCore::Event::Ptr event = aInci.staticCast<KCalendarCore::Event>();
                if(!iCalendar->addEvent(event, iNotebookStr))
                {
                    qCWarning(lcSyncMLPlugin) << "Could not add event";
                    return false;
                }
            }
            break;
        case KCalendarCore::Incidence::TypeTodo:
            {
                KCalendarCore::Todo::Ptr todo = aInci.staticCast<KCalendarCore::Todo>();
                if(!iCalendar->addTodo(todo, iNotebookStr))
                {
                    qCWarning(lcSyncMLPlugin) << "Could not add todo";
                    return false;
                }
            }
            break;
        default:
            qCWarning(lcSyncMLPlugin) << "Could not add incidence, wrong type" << aInci->type();
            return false;
    }
    
    // if you add an incidence, that incidence will be owned by calendar
    // so no need to delete it. Committing for each modification, can cause performance
    // problems.

    if( commitNow )  {
        if( !iStorage->save() )
        {
            qCWarning(lcSyncMLPlugin) << "Could not commit changes to calendar";
            return false;
        }
        qCDebug(lcSyncMLPlugin) << "Single incidence committed";
    }

    qCDebug(lcSyncMLPlugin) << "Added an item with UID : " << aInci->uid() << "Recurrence Id :" << aInci->recurrenceId().toString();

    return true;
}

bool CalendarBackend::commitChanges()
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);
    bool changesCommitted = false;

    if( !iStorage )
    {
        qCWarning(lcSyncMLPlugin) << "No calendar storage!";
    }
    else if( iStorage->save() )  {
        qCDebug(lcSyncMLPlugin) << "Committed changes to calendar";
        changesCommitted = true;
    }
    else
    {
        qCDebug(lcSyncMLPlugin) << "Could not commit changes to calendar";
    }
    return changesCommitted;
}

bool CalendarBackend::modifyIncidence( KCalendarCore::Incidence::Ptr aInci, const QString& aUID, bool commitNow )
{
	FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    if( !iCalendar || !iStorage ) {
        return false;
    }

    KCalendarCore::Incidence::Ptr origInci = getIncidence ( aUID );

    if( !origInci ) {
        qCWarning(lcSyncMLPlugin) << "Item with UID" << aUID << "does not exist. Cannot modify";
        return false;
    }

    if( !modifyIncidence( origInci, aInci ) ) {
        qCWarning(lcSyncMLPlugin) << "Could not make modifications to incidence";
        return false;
    }

    if( commitNow )  {
        if( !iStorage->save() )
        {
            qCWarning(lcSyncMLPlugin) << "Could not commit changes to calendar";
            return false;
        }
        qCDebug(lcSyncMLPlugin) << "Single incidence committed";
    }

    return true;
}

CalendarBackend::ErrorStatus CalendarBackend::deleteIncidence( const QString& aUID )
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);
    CalendarBackend::ErrorStatus errorCode = CalendarBackend::STATUS_OK;

    if( !iCalendar || !iStorage ) {
        errorCode = CalendarBackend::STATUS_GENERIC_ERROR;
    }

    KCalendarCore::Incidence::Ptr incidence = getIncidence( aUID );
    
    if( !incidence ) {
        qCWarning(lcSyncMLPlugin) << "Could not find incidence to delete with UID" << aUID;
        errorCode = CalendarBackend::STATUS_ITEM_NOT_FOUND;
    }

    if( !iCalendar->deleteIncidence( incidence) )
    {
        qCWarning(lcSyncMLPlugin) << "Could not delete incidence with UID" << aUID;
        errorCode = CalendarBackend::STATUS_GENERIC_ERROR;
    }

    if( !iStorage->save() ) {
        qCWarning(lcSyncMLPlugin) << "Could not commit changes to calendar";
        errorCode =  CalendarBackend::STATUS_GENERIC_ERROR;
    }

    return errorCode;
}

bool CalendarBackend::modifyIncidence( KCalendarCore::Incidence::Ptr aIncidence, KCalendarCore::Incidence::Ptr aIncidenceData )
{
    FUNCTION_CALL_TRACE(lcSyncMLPluginTrace);

    Q_ASSERT( aIncidence );
    Q_ASSERT( aIncidenceData );

    // Save critical data from original item
    aIncidenceData->setUid( aIncidence->uid() );
    aIncidenceData->setCreated( aIncidence->created() );

    if( aIncidence->type() != aIncidenceData->type() ) {
        qCWarning(lcSyncMLPlugin) << "Expected incidence type" << aIncidence->typeStr() <<", got" << aIncidenceData->typeStr();
        return false;
    }

    if( aIncidence->type() == KCalendarCore::Incidence::TypeEvent || aIncidence->type() == KCalendarCore::Incidence::TypeTodo )
    {
    KCalendarCore::IncidenceBase::Ptr inc = aIncidence;
    KCalendarCore::IncidenceBase::Ptr data = aIncidenceData;
        *inc = *data;    
    }
    else {
        qCWarning(lcSyncMLPlugin) << "Unsupported incidence type:" << aIncidence->typeStr();
        return false;
    }

    iCalendar->setNotebook( aIncidence, iNotebookStr );

    return true;

}
