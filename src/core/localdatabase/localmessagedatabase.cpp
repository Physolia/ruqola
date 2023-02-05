/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "localmessagedatabase.h"
#include "config-ruqola.h"
#include "localdatabaseutils.h"
#include "messages/message.h"
#include "ruqola_database_debug.h"

#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>

static const char s_schemaMessageDataBase[] = "CREATE TABLE MESSAGES (messageId TEXT PRIMARY KEY NOT NULL, timestamp INTEGER, json TEXT)";
enum class Fields {
    MessageId,
    TimeStamp,
    Json,
}; // in the same order as the table

LocalMessageDatabase::LocalMessageDatabase()
    : LocalDatabaseBase(LocalDatabaseUtils::localMessagesDatabasePath(), LocalDatabaseBase::DatabaseType::Message)
{
}

LocalMessageDatabase::~LocalMessageDatabase() = default;

QString LocalMessageDatabase::schemaDataBase() const
{
    return QString::fromLatin1(s_schemaMessageDataBase);
}

void LocalMessageDatabase::addMessage(const QString &accountName, const QString &roomName, const Message &m)
{
#if HAVE_DATABASE_SUPPORT
    QSqlDatabase db;
    if (initializeDataBase(accountName, roomName, db)) {
        QSqlQuery query(QStringLiteral("INSERT OR REPLACE INTO MESSAGES VALUES (?, ?, ?)"), db);
        query.addBindValue(m.messageId());
        query.addBindValue(m.timeStamp());
        query.addBindValue(m.text()); // TODO replace by json
        if (!query.exec()) {
            qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in MESSAGES table" << db.databaseName() << query.lastError();
        }
    }
#endif
}

void LocalMessageDatabase::deleteMessage(const QString &accountName, const QString &_roomName, const QString &messageId)
{
#ifdef HAVE_DATABASE_SUPPORT
    // addMessage is always called before deleteMessage, if only for the history replay on connect
    // So the db must exist
    const QString roomName = LocalDatabaseUtils::fixRoomName(_roomName);
    const QString dbName = databaseName(accountName + QLatin1Char('-') + roomName);
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if (!db.isValid()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "The assumption was wrong, deleteMessage was called before addMessage, in account" << accountName << "room"
                                       << roomName;
        return;
    }
    Q_ASSERT(db.isValid());
    Q_ASSERT(db.isOpen());
    QSqlQuery query(QStringLiteral("DELETE FROM MESSAGES WHERE messageId = ?"), db);
    query.addBindValue(messageId);
    if (!query.exec()) {
        qCWarning(RUQOLA_DATABASE_LOG) << "Couldn't insert-or-replace in MESSAGES table" << db.databaseName() << query.lastError();
    }

#endif
}
