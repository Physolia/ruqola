/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorconfigureutil.h"

#include <KLocalizedString>
#include <QComboBox>

bool TranslatorConfigureUtil::hasConfigureDialog(TranslatorEngineBase::TranslatorEngine engineType)
{
    switch (engineType) {
    case TranslatorEngineBase::TranslatorEngine::Google:
    case TranslatorEngineBase::TranslatorEngine::Yandex:
    case TranslatorEngineBase::TranslatorEngine::Bing:
    case TranslatorEngineBase::TranslatorEngine::Lingva:
        return false;
    case TranslatorEngineBase::TranslatorEngine::LibreTranslate:
    case TranslatorEngineBase::TranslatorEngine::DeepL:
        return true;
    }
    return false;
}

void TranslatorConfigureUtil::fillComboboxSettings(QComboBox *combo)
{
    for (int i = 0; i <= TranslatorEngineBase::TranslatorEngine::LastEngine; ++i) {
        switch (i) {
        case TranslatorEngineBase::TranslatorEngine::Google:
            combo->addItem(i18n("Google"), QStringLiteral("google"));
            break;
        case TranslatorEngineBase::TranslatorEngine::Yandex:
            combo->addItem(i18n("Yandex"), QStringLiteral("yandex"));
            break;
        case TranslatorEngineBase::TranslatorEngine::Bing:
            combo->addItem(i18n("Bing"), QStringLiteral("bing"));
            break;
        case TranslatorEngineBase::TranslatorEngine::Lingva:
            combo->addItem(i18n("Lingva"), QStringLiteral("lingva"));
            break;
        case TranslatorEngineBase::TranslatorEngine::LibreTranslate:
            combo->addItem(i18n("Libre Translate"), QStringLiteral("libretranslate"));
            break;
        case TranslatorEngineBase::TranslatorEngine::DeepL:
            combo->addItem(i18n("DeepL"), QStringLiteral("deepl"));
            break;
        default:
            // qCWarning(RUQOLA_TRANSLATION_LOG) << " Missing engine. It's a bug " << i;
            break;
        }
    }
}
