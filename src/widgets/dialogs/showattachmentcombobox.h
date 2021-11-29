/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QComboBox>
class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowAttachmentComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ShowAttachmentComboBox(QWidget *parent = nullptr);
    ~ShowAttachmentComboBox() override;

private:
    void fillCombobox();
};

