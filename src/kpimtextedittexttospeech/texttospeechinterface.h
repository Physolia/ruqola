/*
   SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedittexttospeech_export.h"
#include <KPIMTextEditTextToSpeech/AbstractTextToSpeechInterface>
#include <KPIMTextEditTextToSpeech/TextToSpeechWidget>
class QObject;
namespace KPIMTextEditTextToSpeech
{
class TextToSpeechWidget;
class TextToSpeechInterfacePrivate;
/**
 * @brief The TextToSpeechInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDITTEXTTOSPEECH_EXPORT TextToSpeechInterface : public AbstractTextToSpeechInterface
{
    Q_OBJECT
public:
    explicit TextToSpeechInterface(TextToSpeechWidget *textToSpeechWidget, QObject *parent = nullptr);
    ~TextToSpeechInterface() override;

    Q_REQUIRED_RESULT bool isReady() const override;
    void say(const QString &text) override;
    Q_REQUIRED_RESULT double volume() const override;
    void setVolume(double value) override;
    void reloadSettings() override;

private:
    void stateChanged(KPIMTextEditTextToSpeech::TextToSpeechWidget::State state);
    std::unique_ptr<TextToSpeechInterfacePrivate> const d;
};
}
