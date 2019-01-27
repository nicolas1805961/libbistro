%language "C++"
%define api.value.type variant
%define api.token.prefix {TOK_}
%define api.token.constructor
%define locations
%define filename_type {const std::string}
%skeleton "lalr1.cc"
%expect 0
%defines
%param {bistro::parser::ParseDriver& p}

%token            BASE_LEN  "base-length"
                  PLUS      "+"
                  MINUS     "-"
                  MUL       "*"
                  POW       "**"
                  DIV       "/"
                  MOD       "%"
                  LPAR      "("
                  RPAR      ")"
                  NEWLINE   "\n"
                  BASE      "base"
                  EOF 0     "end-of-mine"
%token <std::shared_ptr<num_t>> BIGNUM    "bignum"
%type <node_t> exp t f p

%code requires
{
#include <memory>
#include "base.hh"
#include "bignum.hh"
#include "parse-driver.hh"
#include "ast-node.hh"
using num_t = bistro::parser::ParseDriver::num_t;
using base_t = bistro::parser::ParseDriver::base_t;
using node_t = std::shared_ptr<bistro::ASTNode<num_t, base_t>>;
}

%code provides
{
#define YY_DECL                                           \
  yy::parser::symbol_type yylex(bistro::parser::ParseDriver& p)

YY_DECL;
extern FILE* yyin;
}

%start program

/*
 * The operators handled are:
 *   - '+', '-' (both unary and binary)
 *   - '*', '/', '%' with a higher priority
 *   - '(', ')' for grouping
 *
 * The LL grammar used is the following:
 *
 *   Exp := T
 *        | Exp '+' T
 *        | Exp '-' T
 *
 *   T := P
 *      | T '*' P
 *      | T '/' P
 *      | T '%' P
 *
 *   P := F
        | P '**' F
 *
 *   F := number
 *      | '(' Exp ')'
 *      | '-' F
 *      | '+' F
 *
 * This grammar is left-associative. AST Nodes are constructred using the
 * ASTFactory.
 */

%%

program:
  BASE_LEN NEWLINE BASE NEWLINE exp
  { p.set_ast($5); }
;

exp:
  t
  { $$ = $1; }
| exp PLUS t
  { $$ = p.get_factory()($1, $3, bistro::OpType::PLUS); }
| exp MINUS t
  { $$ = p.get_factory()($1, $3, bistro::OpType::MINUS); }
;

t:
  p
  { $$ = $1; }
| t MUL p
  { $$ = p.get_factory()($1, $3, bistro::OpType::TIMES); }
| t DIV p
  { $$ = p.get_factory()($1, $3, bistro::OpType::DIVIDE); }
| t MOD p
  { $$ = p.get_factory()($1, $3, bistro::OpType::MODULO); }
;

p:
  f
  { $$ = $1; }
| p POW f
  { $$ = p.get_factory()($1, $3, bistro::OpType::POWER); }
;

f:
  BIGNUM
  { $$ = p.get_factory()($1); }
| LPAR exp RPAR
  { $$ = $2; }
| MINUS f
  {
    $$ = p.get_factory()($2, bistro::OpType::MINUS);
  }
| PLUS f
  {
    $$ = p.get_factory()($2, bistro::OpType::PLUS);
  }
;

%%
void yy::parser::error(const location_type& l, const std::string& m)
{
  p.set_error();
  std::cerr << l << ": " << m << std::endl;
}
