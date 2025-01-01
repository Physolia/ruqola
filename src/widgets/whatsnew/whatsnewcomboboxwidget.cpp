/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewcomboboxwidget.h"
#include <KLocalizedString>
#include <KSeparator>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>

WhatsNewComboBoxWidget::WhatsNewComboBoxWidget(QWidget *parent)
    : QWidget{parent}
    , mVersionComboBox(new QComboBox(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
    hboxLayout->setContentsMargins({});
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(i18nc("@label:textbox", "Version:"), this);
    label->setObjectName(QStringLiteral("label"));
    hboxLayout->addWidget(label);

    mVersionComboBox->setObjectName(QStringLiteral("mVersionComboBox"));
    hboxLayout->addWidget(mVersionComboBox);
    hboxLayout->addStretch(1);

    auto separator = new KSeparator(this);
    separator->setObjectName(QStringLiteral("separator"));
    mainLayout->addWidget(separator);

    connect(mVersionComboBox, &QComboBox::currentIndexChanged, this, &WhatsNewComboBoxWidget::slotCurrentIndexChanged);
}

void WhatsNewComboBoxWidget::addVersion(const QString &name, int identifier)
{
    mVersionComboBox->addItem(name, identifier);
}

WhatsNewComboBoxWidget::~WhatsNewComboBoxWidget() = default;

void WhatsNewComboBoxWidget::initializeVersion(int identifier)
{
    const int index = mVersionComboBox->findData(identifier);
    if (index != -1) {
        mVersionComboBox->setCurrentIndex(index);
    }
}

void WhatsNewComboBoxWidget::slotCurrentIndexChanged(int index)
{
    const int type = mVersionComboBox->itemData(index).toInt();
    Q_EMIT versionChanged(type);
}

#include "moc_whatsnewcomboboxwidget.cpp"
