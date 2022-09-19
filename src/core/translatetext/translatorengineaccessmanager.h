/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QObject>

class QNetworkAccessManager;
class TranslatorEngineAccessManager : public QObject
{
    Q_OBJECT
public:
    explicit TranslatorEngineAccessManager(QObject *parent = nullptr);
    ~TranslatorEngineAccessManager();

    static TranslatorEngineAccessManager *self();

    QNetworkAccessManager *networkManager();

private:
    QNetworkAccessManager *const mNetworkAccessManager;
};