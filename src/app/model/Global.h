#ifndef GLOBAL_H
#define GLOBAL_H

#include <QVariant>
#include <QSettings>

#define APP_NAME            "TechService CRM"
#define ORGANIZATION_NAME   "TechService"
#define APP_VERSION         "1.0.0.0"

extern bool g_ordersDbOpened;

//extern const QString g_personalNumber;
//extern const QString g_personalNumberName;

namespace global
{
    void setSettingsValue(const QString& key, const QVariant& value, const QString& group = "");

    QVariant getSettingsValue(const QString& key, const QString& group = "", const QVariant& value = QVariant());

    void removeSettingsKey(const QString& key, const QString& group = "");

    bool containsSettingsKey(const QString& key, const QString& group = "");

    QStringList getSettingKeys(const QString& group = "");

    QString getExtensionNumber(const QString& group = "");
}

#endif // GLOBAL_H
