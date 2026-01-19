#include "commands/query_command.h"
#include "execution_context.h"
#include "logger.h"
#include "SHantilly.h"

void QueryCommand::execute(ExecutionContext &context,
                           const QList<QString> &args) {
  Q_UNUSED(args);
  SHantilly *dialog = context.dialogBox();
  if (!dialog)
    return;

  qCDebug(parserLog) << "Executing QueryCommand";
  context.executeOnGui([dialog]() { dialog->report(); });
}

std::unique_ptr<Command> QueryCommandFactory::create() const {
  return std::make_unique<QueryCommand>();
}
