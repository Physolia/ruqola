/*
   SPDX-FileCopyrightText: 2021 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "localdatabasebase.h"
#include <QString>
#include <memory>

class QFile;
class QSqlTableModel;
class Message;

class LIBRUQOLACORE_EXPORT LocalMessageLogger : public LocalDatabaseBase
{
public:
    LocalMessageLogger();

    void addMessage(const QString &accountName, const QString &roomName, const Message &message);
    void deleteMessage(const QString &accountName, const QString &roomName, const QString &messageId);
    Q_REQUIRED_RESULT std::unique_ptr<QSqlTableModel> createMessageModel(const QString &accountName, const QString &roomName) const;
    Q_REQUIRED_RESULT bool saveToFile(QFile &file, const QString &accountName, const QString &roomName) const;

protected:
    Q_REQUIRED_RESULT QString schemaDataBase() const override;
};
