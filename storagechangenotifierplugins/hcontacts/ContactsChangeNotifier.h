#ifndef CONTACTSCHANGENOTIFIER_H
#define CONTACTSCHANGENOTIFIER_H

#include <QObject>
#include <QContactManager>
#include <QList>

#include <QContactId>
using namespace QtContacts;

class ContactsChangeNotifier : public QObject
{
    Q_OBJECT

public:
    /*! \brief constructor
     */
    ContactsChangeNotifier();

    /*! \brief constructor
     */
    ~ContactsChangeNotifier();

    /*! \brief start listening to changes from QContactManager
     */
    void enable();

    /*! \brief stop listening to changes from QContactManager
     */
    void disable();

Q_SIGNALS:
    /*! emit this signal to notify a change in contacts backend
     */
    void change();

private Q_SLOTS:
    void onContactsAdded(const QList<QContactId>& ids);
    void onContactsRemoved(const QList<QContactId>& ids);
    void onContactsChanged(const QList<QContactId>& ids);

private:
    QContactManager* iManager;
    bool iDisabled;
};

#endif
