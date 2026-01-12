#include "commands/simple_commands.h"
#include "showbox.h"
#include "execution_context.h"
#include "commands/command_utils.h"

void ShowCommand::execute(ExecutionContext &context, const QList<QString> &args) {
    QString name = args.isEmpty() ? "" : args[0];
    context.executeOnGui([&context, name]() {
        QWidget *w = name.isEmpty() ? context.dialogBox() : context.dialogBox()->findWidget(const_cast<char*>(name.toUtf8().data()));
        if (w) context.dialogBox()->setOptions(w, DialogCommandTokens::OptionVisible, DialogCommandTokens::OptionVisible, nullptr);
    });
}

void HideCommand::execute(ExecutionContext &context, const QList<QString> &args) {
    QString name = args.isEmpty() ? "" : args[0];
    context.executeOnGui([&context, name]() {
        QWidget *w = name.isEmpty() ? context.dialogBox() : context.dialogBox()->findWidget(const_cast<char*>(name.toUtf8().data()));
        if (w) context.dialogBox()->setOptions(w, 0, DialogCommandTokens::OptionVisible, nullptr);
    });
}

void EnableCommand::execute(ExecutionContext &context, const QList<QString> &args) {
    QString name = args.isEmpty() ? "" : args[0];
    context.executeOnGui([&context, name]() {
        QWidget *w = name.isEmpty() ? context.dialogBox() : context.dialogBox()->findWidget(const_cast<char*>(name.toUtf8().data()));
        if (w) context.dialogBox()->setEnabled(w, true);
    });
}

void DisableCommand::execute(ExecutionContext &context, const QList<QString> &args) {
    QString name = args.isEmpty() ? "" : args[0];
    context.executeOnGui([&context, name]() {
        QWidget *w = name.isEmpty() ? context.dialogBox() : context.dialogBox()->findWidget(const_cast<char*>(name.toUtf8().data()));
        if (w) context.dialogBox()->setEnabled(w, false);
    });
}

void EndCommand::execute(ExecutionContext &context, const QList<QString> &args) {
    Q_UNUSED(args); // Could handle type
    context.executeOnGui([&context]() {
        // Simple end logic, prioritize based on current state?
        // Old parser priority: list, group, page, tabs
        context.dialogBox()->endGroup();
        context.dialogBox()->endList();
    });
}

void StepCommand::execute(ExecutionContext &context, const QList<QString> &args) {
    bool vertical = !args.isEmpty() && args[0] == "vertical";
    context.executeOnGui([&context, vertical]() {
        if (vertical) context.dialogBox()->stepVertical();
        else context.dialogBox()->stepHorizontal();
    });
}

void ClearCommand::execute(ExecutionContext &context, const QList<QString> &args) {
    QString name = args.isEmpty() ? "" : args[0];
    context.executeOnGui([&context, name]() {
        if (name.isEmpty()) context.dialogBox()->clearDialog();
        else context.dialogBox()->clear(const_cast<char*>(name.toUtf8().data()));
    });
}

void RemoveCommand::execute(ExecutionContext &context, const QList<QString> &args) {
    if (args.isEmpty()) return;
    QString name = args[0];
    context.executeOnGui([&context, name]() {
        context.dialogBox()->removeWidget(const_cast<char*>(name.toUtf8().data()));
    });
}
