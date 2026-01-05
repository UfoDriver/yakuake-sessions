#include "session.h"

bool compare_sessions_by_pointer(const Session *lhs, const Session *rhs) {
  return *lhs < *rhs;
}
