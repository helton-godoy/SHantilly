#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include "command_registry.h"
#include "execution_context.h"

class Parser
{
public:
    Parser(const CommandRegistry &registry, ExecutionContext &context);
    bool parse(const QString &input);

private:
    const CommandRegistry &m_registry;
    ExecutionContext &m_context;
};

#endif // PARSER_H