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

#include "permissionmanagertest.h"
#include "permissions/permissionmanager.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(PermissionManagerTest)
PermissionManagerTest::PermissionManagerTest(QObject *parent)
    : QObject(parent)
{
}

void PermissionManagerTest::shouldHaveDefaultValues()
{
    PermissionManager w;
    QCOMPARE(w.permissionCount(), 0);
}

void PermissionManagerTest::shouldHasRoles()
{
    PermissionManager w;
    QVERIFY(w.roles(QStringLiteral("bla")).isEmpty());
}

void PermissionManagerTest::shouldLoadPermissions_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("permissionsCount");
    QTest::addColumn<int>("permissionsAdded");

    QTest::addRow("permissions1") << QStringLiteral("permissions1") << 5 << 4;
}

void PermissionManagerTest::shouldLoadPermissions()
{
    QFETCH(QString, name);
    QFETCH(int, permissionsCount);
    QFETCH(int, permissionsAdded);
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/permissions/") + name + QLatin1String(".json");
    const QJsonArray obj = AutoTestHelper::loadJsonArrayObject(originalJsonFile);
    QCOMPARE(obj.count(), permissionsCount);
    qDebug() << " OBJ " << obj;

    PermissionManager r;
    r.parseUpdatePermission(obj);
    QCOMPARE(r.permissionCount(), permissionsAdded);
}
