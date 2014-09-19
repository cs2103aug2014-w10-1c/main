#include "stdafx.h"
#include "add_query.h"

namespace You {
namespace NLP {
namespace Internal {

std::wostream& operator<<(std::wostream& s, const ADD_QUERY& q) {
	return s << q.description << L"(due " << q.due << L")";
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You
