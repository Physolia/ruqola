/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "roleeditwidgettest.h"
#include "administratordialog/roles/roleeditwidget.h"
#include "administratordialog/roles/rolescopecombobox.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(RoleEditWidgetTest)
RoleEditWidgetTest::RoleEditWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void RoleEditWidgetTest::shouldHaveDefaultValues()
{
    RoleEditWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());

    auto mDescription = w.findChild<QLineEdit *>(QStringLiteral("mDescription"));
    QVERIFY(mDescription);
    QVERIFY(mDescription->text().isEmpty());

    auto mTwoFactor = w.findChild<QCheckBox *>(QStringLiteral("mTwoFactor"));
    QVERIFY(mTwoFactor);
    QVERIFY(!mTwoFactor->isChecked());
    QVERIFY(!mTwoFactor->text().isEmpty());

    auto mRoleScopeComboBox = w.findChild<RoleScopeComboBox *>(QStringLiteral("mRoleScopeComboBox"));
    QVERIFY(mRoleScopeComboBox);

    QVERIFY(!w.roleEditDialogInfo().isValid());
}
