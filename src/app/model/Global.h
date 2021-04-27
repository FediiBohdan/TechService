#ifndef GLOBAL_H
#define GLOBAL_H

#include <QVariant>
#include <QSettings>

#define APP_NAME            "TechService CRM"
#define ORGANIZATION_NAME   "TechService"
#define APP_VERSION         "1.0.0.0"

namespace global
{
    QStringList getSettingKeys(const QString &group = "");

    QString getExtensionNumber(const QString &group = "");

    void removeSettingsKey(const QString &key, const QString &group = "");

    bool containsSettingsKey(const QString &key, const QString &group = "");

    void setSettingsValue(const QString &key, const QVariant &value, const QString &group = "");

    QVariant getSettingsValue(const QString &key, const QString &group = "", const QVariant &value = QVariant());
}

#endif // GLOBAL_H
