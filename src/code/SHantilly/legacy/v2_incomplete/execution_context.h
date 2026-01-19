#ifndef EXECUTION_CONTEXT_H
#define EXECUTION_CONTEXT_H

#include <functional>

class SHantilly;

class ExecutionContext {
public:
  explicit ExecutionContext(SHantilly *dialogBox);
  SHantilly *dialogBox() const;

  // Execute function on the GUI thread (blocking)
  void executeOnGui(std::function<void()> func);

private:
  SHantilly *m_dialogBox;
};

#endif // EXECUTION_CONTEXT_H