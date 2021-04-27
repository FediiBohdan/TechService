#include "Global.h"

void global::setSettingsValue(const QString &key, const QVariant &value, const QString &group)
{
    QSettings settings(ORGANIZATION_NAME, APP_NAME);

    if (!group.isEmpty())
        settings.beginGroup(group);

    settings.setValue(key, value);
}

/**
 * Returns settings from register.
 */
QVariant global::getSettingsValue(const QString &key, const QString &group, const QVariant &defaultValue)
{
    QSettings settings(ORGANIZATION_NAME, APP_NAME);

    if (!group.isEmpty())
        settings.beginGroup(group);

    return settings.value(key, defaultValue);
}

/**
 * Removes key settings from register.
 */
void global::removeSettingsKey(const QString &key, const QString &group)
{
    QSettings settings(ORGANIZATION_NAME, APP_NAME);

    if (!group.isEmpty())
        settings.beginGroup(group);

    settings.remove(key);
}

/**
 * Checks key settings existance in register.
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
 * Returns key settings from the register.
 */
QStringList global::getSettingKeys(const QString &group)
{
    QSettings settings(ORGANIZATION_NAME, APP_NAME);

    if (!group.isEmpty())
        settings.beginGroup(group);

    return settings.childKeys();
}
