/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commonmessagesmodel.h"
#include "rocketchataccount.h"

#include "listmessages.h"

CommonMessagesModel::CommonMessagesModel(RocketChatAccount *account, QObject *parent)
    : MessagesModel(QByteArrayLiteral("no_room"), account, nullptr, parent)
{
}

CommonMessagesModel::~CommonMessagesModel() = default;

void CommonMessagesModel::clearModel()
{
    mStringNotFound = true;
    mLoadingInProgress = false;
    clear();
}

QList<Message> CommonMessagesModel::extractMessages(const QJsonObject &obj)
{
    ListMessages messages;
    messages.parseMessages(obj, QStringLiteral("messages"));
    return messages.listMessages();
}

void CommonMessagesModel::parse(const QJsonObject &obj)
{
    clear();
    const QList<Message> messages = extractMessages(obj);
    addMessages(messages);
    setStringNotFound(rowCount() == 0);
}

void CommonMessagesModel::setStringNotFound(bool stringNotFound)
{
    if (mStringNotFound != stringNotFound) {
        mStringNotFound = stringNotFound;
        Q_EMIT stringNotFoundChanged();
    }
}

bool CommonMessagesModel::loadCommonMessagesInProgress() const
{
    return mLoadingInProgress;
}

void CommonMessagesModel::setLoadCommonMessagesInProgress(bool loadSearchMessageInProgress)
{
    if (mLoadingInProgress != loadSearchMessageInProgress) {
        mLoadingInProgress = loadSearchMessageInProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

#include "moc_commonmessagesmodel.cpp"
