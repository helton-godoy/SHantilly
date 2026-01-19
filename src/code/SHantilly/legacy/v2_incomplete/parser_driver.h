#ifndef PARSER_DRIVER_H
#define PARSER_DRIVER_H

#include <QThread>

#include "execution_context.h"
#include "parser.h"

class ParserDriver : public QThread {
    Q_OBJECT
public:
    ParserDriver(const CommandRegistry& registry, ExecutionContext& context,
                 QObject* parent = nullptr);
    ~ParserDriver() override;
    void run() override;

private:
    Parser m_parser;
};

#endif // PARSER_DRIVER_H