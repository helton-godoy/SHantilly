#include "ParserMain.h"
#include <IShowboxBuilder.h>
#include <iostream>
#include <string>
#include <QStringList>

ParserMain::ParserMain(IShowboxBuilder *builder, QObject *parent)
    : QObject(parent), m_builder(builder)
{
    m_commandParser.registerCommand("add", [this](const QStringList &args) {
        handleAdd(args);
    });

    m_commandParser.registerCommand("show", [this](const QStringList &) {
        emit showRequested();
    });
}

void ParserMain::run()
{
    std::string line;
    while (std::getline(std::cin, line)) {
        QString qline = QString::fromStdString(line).trimmed();
        if (qline.isEmpty() || qline.startsWith("#")) continue;
        processLine(qline);
    }
}

void ParserMain::processLine(const QString &line)
{
    m_commandParser.parseLine(line);
}

void ParserMain::handleAdd(const QStringList &args)
{
    if (args.size() < 3) return;

    QString type = args[0].toLower();
    QString title = args[1];
    QString name = args[2];

    if (type == "pushbutton" || type == "button") {
        m_builder->buildPushButton(title, name);
    }
}