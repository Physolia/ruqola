/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <PimCommonTextTranslator/TranslatorEnginePlugin>

class YandexEnginePlugin : public PimCommonTextTranslator::TranslatorEnginePlugin
{
    Q_OBJECT
public:
    explicit YandexEnginePlugin(QObject *parent = nullptr);
    ~YandexEnginePlugin() override;

    void translate() override;

private:
    void parseCredentials(QNetworkReply *reply);
    void translateText();
    void parseTranslation(QNetworkReply *reply);
    static QString sYandexKey;
    QString mResult;
};