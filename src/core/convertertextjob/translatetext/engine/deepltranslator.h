/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "convertertextjob/translatetext/translatorenginebase.h"
#include "libruqolacore_export.h"
class LIBRUQOLACORE_EXPORT DeepLTranslator : public TranslatorEngineBase
{
    Q_OBJECT
public:
    explicit DeepLTranslator(QObject *parent = nullptr);
    ~DeepLTranslator() override;

    Q_REQUIRED_RESULT QString engineName() const override;
    void translate() override;
    Q_REQUIRED_RESULT QVector<QPair<QString, QString>> supportedLanguage() const override;
    void loadSettings() override;

    Q_REQUIRED_RESULT static QVector<QPair<QString, QString>> languages();

private:
    Q_REQUIRED_RESULT QString apiUrl() const;
    void translateText();
    void parseTranslation(QNetworkReply *reply);
    static inline QVector<QPair<QString, QString>> mLanguages;
    bool mUseFreeLicense = false;
};