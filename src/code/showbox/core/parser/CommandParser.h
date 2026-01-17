#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <functional>

class CommandParser
{
public:
    using CommandHandler = std::function<void(const QStringList&)>;

    CommandParser();
    
    void registerCommand(const QString &name, CommandHandler handler);
    void parseLine(const QString &line);

private:
    QMap<QString, CommandHandler> m_commands;
};

#endif // COMMANDPARSER_H
