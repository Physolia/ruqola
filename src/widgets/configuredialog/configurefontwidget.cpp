/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "configurefontwidget.h"
#include "ruqolaglobalconfig.h"
#include <KFontChooser>
#include <KLocalizedString>
#include <kwidgetsaddons_version.h>

#include <QCheckBox>
#include <QVBoxLayout>

ConfigureFontWidget::ConfigureFontWidget(QWidget *parent)
    : QWidget(parent)
    , mCustomFontCheck(new QCheckBox(i18n("&Use custom fonts"), this))
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 86, 0)
    , mFontChooser(new KFontChooser(KFontChooser::DisplayFrame, this))
#else
    , mFontChooser(new KFontChooser(this, KFontChooser::DisplayFrame, QStringList(), 4))
#endif
{
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 86, 0)
    mFontChooser->setMinVisibleItems(4);
#endif
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mCustomFontCheck->setObjectName(QStringLiteral("mCustomFontCheck"));
    mainLayout->addWidget(mCustomFontCheck);

    mFontChooser->setObjectName(QStringLiteral("mFontChooser"));
    mFontChooser->setEnabled(false); // since !mCustomFontCheck->isChecked()
    mainLayout->addWidget(mFontChooser);
    connect(mCustomFontCheck, &QCheckBox::toggled, mFontChooser, &KFontChooser::setEnabled);
}

ConfigureFontWidget::~ConfigureFontWidget() = default;

void ConfigureFontWidget::save()
{
    RuqolaGlobalConfig::self()->setUseCustomFont(mCustomFontCheck->isChecked());
    RuqolaGlobalConfig::self()->setGeneralFont(mFontChooser->font());
    RuqolaGlobalConfig::self()->save();
}

void ConfigureFontWidget::load()
{
    mCustomFontCheck->setChecked(RuqolaGlobalConfig::self()->useCustomFont());
    mFontChooser->setFont(RuqolaGlobalConfig::self()->generalFont());
}
