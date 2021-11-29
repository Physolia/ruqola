/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDebug>
#include <QUrl>
#include <QWidget>
class QLineEdit;
class KUrlRequester;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomEmojiCreateWidget : public QWidget
{
    Q_OBJECT
public:
    struct CustomEmojiCreateInfo {
        QString alias;
        QString name;
        QUrl fileNameUrl;
    };
    explicit AdministratorCustomEmojiCreateWidget(QWidget *parent = nullptr);
    ~AdministratorCustomEmojiCreateWidget() override;

    void setCustomEmojiInfo(const CustomEmojiCreateInfo &info);

    Q_REQUIRED_RESULT AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo info() const;

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    void slotUpdateOkButton();
    QLineEdit *const mName;
    QLineEdit *const mAlias;
    KUrlRequester *const mSelectFile;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo &t);
