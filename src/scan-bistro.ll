%option noyywrap noinput nounput

%{
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "base.hh"
#include "bignum.hh"
#include "parse-bistro.hh"
#include "parse-driver.hh"

#define YY_USER_ACTION                                    \
  loc.columns(yyleng);
#define TOKEN(Type)                                       \
  yy::parser::make_ ## Type(loc)
#define TOKEN_VAL(Type, Value)                            \
  yy::parser::make_ ## Type(Value, loc)
#define yyterminate()                                     \
  yy_delete_buffer(YY_CURRENT_BUFFER);                    \
  return TOKEN(EOF);
static yy::location loc;
static int base_length = 0;
static int base_count = 0;
static std::stringstream s;

yy::parser::symbol_type
complete_bignum(::bistro::parser::ParseDriver& p)
{
  try
  {
    auto num = std::make_shared<num_t>(s, p.get_base());
    if (s.peek() != EOF)
    {
      p.set_error();
      std::cerr << loc << ": Invalid trailing characters"
                << "\n";
      yyterminate();
    }
    s.str("");
    s.clear();
    return TOKEN_VAL(BIGNUM, num);
  }
  catch (std::length_error& e)
  {
    p.set_error();
    std::cerr << e.what() << "\n";
    yyterminate();
  }
}
%}

int           [0-9]+
base_char     [^\+\*\-\/\%\(\)]


%x BASE EXPRESSION BIGNUM


%%
%{
  loc.step();
%}

{int}         {
                base_length = std::stoul(yytext);
                return TOKEN(BASE_LEN);
              }

\n            loc.lines(); BEGIN BASE; return TOKEN(NEWLINE);

.             {
                p.set_error();
                std::cerr << loc << ": invalid character in base length.\n";
                yyterminate();
              }

<BASE>{
  {base_char} {
                /* The base definition format is as follows:
                 *  - the first line contains the numerical value of the base,
                 *    in base 10.
                 *  - the second line contains the representation of each of the
                 *   digits of the base, in order.
                 */
                if (*yytext == '\n')
                  loc.lines();
                if (base_count == base_length)
                {
                  if (*yytext != '\n')
                  {
                    p.set_error();
                    std::cerr << loc << ": base actual length differ from"
                    " announced length\n";
                    yyterminate();
                  }
                  BEGIN EXPRESSION;
                  return TOKEN(NEWLINE);
                }
                try
                {
                  p.get_base().add_digit(*yytext);
                }
                catch (std::invalid_argument& e)
                {
                  p.set_error();
                  std::cerr << e.what() << "\n";
                  yyterminate();
                }
                base_count++;
                if (base_count == base_length)
                  return TOKEN(BASE);
              }
  .           {
                p.set_error();
                std::cerr << loc
                          << ": invalid use of reserved character in base.\n";
                yyterminate();
              }
}

<EXPRESSION>{
  "+"         return TOKEN(PLUS);
  "-"         return TOKEN(MINUS);
  "/"         return TOKEN(DIV);
  "*"         return TOKEN(MUL);
  "**"        return TOKEN(POW);
  "%"         return TOKEN(MOD);
  "("         return TOKEN(LPAR);
  ")"         return TOKEN(RPAR);
  {base_char} {
                yyless(0);
                loc.columns(-1);
                BEGIN BIGNUM;
              }
}

<BIGNUM>{
  {base_char} {
                s.put(*yytext);
              }
  .           {
                yyless(0);
                loc.columns(-1);
                BEGIN EXPRESSION;
                return complete_bignum(p);
              }
  <<EOF>>     {
                BEGIN EXPRESSION;
                return complete_bignum(p);
              }
}

%%
