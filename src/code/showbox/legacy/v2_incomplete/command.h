#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <QList>
#include <QString>

class ExecutionContext;

class Command
{
public:
    virtual ~Command() = default;
    virtual void execute(ExecutionContext &context, const QList<QString> &args) = 0;
};

class CommandFactory
{
public:
    virtual ~CommandFactory() = default;
    virtual std::unique_ptr<Command> create() const = 0;
};

#endif // COMMAND_H
