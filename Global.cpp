#include "Global.h"

#include <QSettings>

bool g_mainDbsOpened = false;
bool g_ordersDbOpened = false;

void global::setSettingsValue(const QString &key, const QVariant &value, const QString &group)
{
    QSettings settings(ORGANIZATION_NAME, APP_NAME);

    if (!group.isEmpty())
        settings.beginGroup(group);

    settings.setValue(key, value);
}

/**
 * Возвращает настройку из реестра.
 */
QVariant global::getSettingsValue(const QString &key, const QString &group, const QVariant &defaultValue)
{
    QSettings settings(ORGANIZATION_NAME, APP_NAME);

    if (!group.isEmpty())
        settings.beginGroup(group);

    return settings.value(key, defaultValue);
}

/**
 * Выполняет удаление ключа настройки в реестре.
 */
void global::removeSettingsKey(const QString &key, const QString &group)
{
    QSettings settings(ORGANIZATION_NAME, APP_NAME);

    if (!group.isEmpty())
        settings.beginGroup(group);

    settings.remove(key);
}

/**
 * Выполняет проверку на наличие ключа настройки в реестре.
 */
bool global::containsSettingsKey(const QString &key, const QString &group)
{
    if (key.isEmpty())
        return false;

    QSettings settings(ORGANIZATION_NAME, APP_NAME);

    if (!group.isEmpty())
        settings.beginGroup(group);

    return settings.contains(key);
}

/**
 * Возвращает ключи настройки из реестра.
 */
QStringList global::getSettingKeys(const QString &group)
{
    QSettings settings(ORGANIZATION_NAME, APP_NAME);

    if (!group.isEmpty())
        settings.beginGroup(group);

    return settings.childKeys();
}
