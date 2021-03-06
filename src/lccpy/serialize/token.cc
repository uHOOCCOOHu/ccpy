#include "./token.h"
#include <utility>
#include "./structual.h"
#include "./number.h"
#include "../util/adt.h"
#include "../util/types.h"
using namespace std;
using namespace ccpy::ast;

namespace ccpy::serialize {

namespace {

Structual parened(Str name, Str value) {
  return Structual { StructParen {
    optional<Str> { move(name) },
    vector<Structual> { StructStr { move(value) } }
  } };
}

Structual trans(const Token &tok) {
  return match<Structual>(tok
  , [&](const TokKeyword &tok) {
    auto kw = KeywordMap[static_cast<size_t>(tok.keyword)];
    return parened("Keyword", kw);
  }
  , [&](const TokSymbol &tok) {
    auto sym = SymbolMap[static_cast<size_t>(tok.symbol)];
    return parened("Symbol", sym);
  }
  , [&](const TokName &tok) {
    return parened("Name", tok.name);
  }
  , [&](const TokString &tok) {
    return parened("String", tok.str);
  }
  , [&](const TokInteger &tok) {
    return parened("Integer", IntegerSerializer {}(tok.integer));
  }
  , [&](const TokIndent &) {
    return Structual { StructValue { "Indent" } };
  }
  , [&](const TokDedent &) {
    return Structual { StructValue { "Dedent" } };
  }
  , [&](const TokNewline &) {
    return Structual { StructValue { "Newline" } };
  }
  );
}

} // namespace anonymous


Structual TokenTreeSerializer::operator()(const Token &tok) const {
  return trans(tok);
}


} // namespace ccpy::serialize
