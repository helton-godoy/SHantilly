#include "icon_helper.h"
#include <QFile>
#include <QDebug>
#include "logger.h"

QIcon IconHelper::loadIcon(const QString &iconSource)
{
    if (iconSource.isEmpty()) {
        return QIcon();
    }

    // Try as local file first
    if (QFile::exists(iconSource)) {
        qCDebug(guiLog) << "Loading icon from file:" << iconSource;
        return QIcon(iconSource);
    }

    // Try from system theme
    qCDebug(guiLog) << "Loading icon from theme:" << iconSource;
    QIcon themeIcon = QIcon::fromTheme(iconSource);
    
    if (!themeIcon.isNull()) {
        return themeIcon;
    }

    qCWarning(guiLog) << "Could not find icon:" << iconSource;
    return QIcon();
}
