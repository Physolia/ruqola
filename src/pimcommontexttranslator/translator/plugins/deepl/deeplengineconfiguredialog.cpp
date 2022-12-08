/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "deeplengineconfiguredialog.h"
#include "deeplengineconfigurewidget.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>

DeeplEngineConfigureDialog::DeeplEngineConfigureDialog(QWidget *parent)
    : QDialog(parent)
    , mConfigureWidget(new DeeplEngineConfigureWidget(this))
{
    mConfigureWidget->setObjectName(QStringLiteral("mConfigureWidget"));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->addWidget(mConfigureWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    mainLayout->addWidget(buttonBox);
}

DeeplEngineConfigureDialog::~DeeplEngineConfigureDialog() = default;