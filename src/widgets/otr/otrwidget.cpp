/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "otrwidget.h"

#include <KLocalizedString>
#include <QAction>

OtrWidget::OtrWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Information);
    setText(i18n("Otr activated")); // FIXME i18n ?
    auto endOtrAction = new QAction(i18n("End OTR"), this);
    endOtrAction->setObjectName(QStringLiteral("endOtrAction"));
    connect(endOtrAction, &QAction::triggered, this, &OtrWidget::closeOtr);
    addAction(endOtrAction);

    auto refreshKeysAction = new QAction(i18n("Refresh Keys"), this);
    refreshKeysAction->setObjectName(QStringLiteral("refreshKeysAction"));
    connect(refreshKeysAction, &QAction::triggered, this, &OtrWidget::refreshKeys);
    addAction(refreshKeysAction);
}

OtrWidget::~OtrWidget() = default;
