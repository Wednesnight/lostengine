#ifndef LOST_LUA_GRAMMAR_H
#define LOST_LUA_GRAMMAR_H

#include <boost/spirit/core.hpp>

using namespace boost;
using namespace boost::spirit;

namespace lost
{
  namespace lua
  {
    
    struct Grammar : public grammar<Grammar>
    {
      template<typename ScannerT>
      struct definition
      {
        rule<ScannerT> unop, binop, fieldsep, field, fieldlist, tableconstructor,
        name, namelist, number, exp, quote, string_, parlist, funcbody, block,
        function, args, functioncall, prefixexp, explist, var, varlist, funcname,
        laststat, chunk, emptychunk, while_statement, assignment, stat, do_, repeat_, if_, for_, for_in_, func_def_, local_func_def_, local_def_,
        binexp, unexp;
        
        rule<ScannerT> tcall, tsub, tvar, tdotted, tsimplevar, tchained;
        
        rule<ScannerT> texp, tbinop1, tbinop2, tbinop3, tbinop4, tbinop5, tbinop6, tbinop8;
        rule<ScannerT> tunop7;
        rule<ScannerT> texp0, texp1, texp2, texp3, texp4, texp5, texp6, texp7, texp8, texp_bracket, texp_terminal;
        
        definition(Grammar const& self)
        {
          
          BOOST_SPIRIT_DEBUG_RULE(unop);
          BOOST_SPIRIT_DEBUG_RULE(binop);
          BOOST_SPIRIT_DEBUG_RULE(fieldsep);
          BOOST_SPIRIT_DEBUG_RULE(field);
          BOOST_SPIRIT_DEBUG_RULE(fieldlist);
          BOOST_SPIRIT_DEBUG_RULE(tableconstructor);
          BOOST_SPIRIT_DEBUG_RULE(name);
          BOOST_SPIRIT_DEBUG_RULE(namelist);
          BOOST_SPIRIT_DEBUG_RULE(number);
          BOOST_SPIRIT_DEBUG_RULE(exp);
          BOOST_SPIRIT_DEBUG_RULE(quote);      
          BOOST_SPIRIT_DEBUG_RULE(string_);      
          BOOST_SPIRIT_DEBUG_RULE(parlist);
          BOOST_SPIRIT_DEBUG_RULE(funcbody);
          BOOST_SPIRIT_DEBUG_RULE(block);
          BOOST_SPIRIT_DEBUG_RULE(function);
          BOOST_SPIRIT_DEBUG_RULE(args);
          BOOST_SPIRIT_DEBUG_RULE(functioncall);
          BOOST_SPIRIT_DEBUG_RULE(prefixexp);
          BOOST_SPIRIT_DEBUG_RULE(explist);
          BOOST_SPIRIT_DEBUG_RULE(var);
          BOOST_SPIRIT_DEBUG_RULE(varlist);
          BOOST_SPIRIT_DEBUG_RULE(funcname);
          BOOST_SPIRIT_DEBUG_RULE(laststat);
          BOOST_SPIRIT_DEBUG_RULE(stat);
          BOOST_SPIRIT_DEBUG_RULE(chunk);
          BOOST_SPIRIT_DEBUG_RULE(emptychunk);
          BOOST_SPIRIT_DEBUG_RULE(assignment);      
          BOOST_SPIRIT_DEBUG_RULE(while_statement);
          BOOST_SPIRIT_DEBUG_RULE(do_);
          BOOST_SPIRIT_DEBUG_RULE(repeat_);
          BOOST_SPIRIT_DEBUG_RULE(if_);
          BOOST_SPIRIT_DEBUG_RULE(for_);
          BOOST_SPIRIT_DEBUG_RULE(for_in_);
          BOOST_SPIRIT_DEBUG_RULE(func_def_);
          BOOST_SPIRIT_DEBUG_RULE(local_func_def_);
          BOOST_SPIRIT_DEBUG_RULE(local_def_);
          BOOST_SPIRIT_DEBUG_RULE(binexp);
          BOOST_SPIRIT_DEBUG_RULE(unexp);
          
          BOOST_SPIRIT_DEBUG_RULE(tcall);
          BOOST_SPIRIT_DEBUG_RULE(tsub);
          BOOST_SPIRIT_DEBUG_RULE(tvar);
          BOOST_SPIRIT_DEBUG_RULE(tsimplevar);
          BOOST_SPIRIT_DEBUG_RULE(tchained);
          BOOST_SPIRIT_DEBUG_RULE(tdotted);

          BOOST_SPIRIT_DEBUG_RULE(texp);
          BOOST_SPIRIT_DEBUG_RULE(texp0);
          BOOST_SPIRIT_DEBUG_RULE(texp1);
          BOOST_SPIRIT_DEBUG_RULE(texp2);
          BOOST_SPIRIT_DEBUG_RULE(texp3);
          BOOST_SPIRIT_DEBUG_RULE(texp4);
          BOOST_SPIRIT_DEBUG_RULE(texp5);
          BOOST_SPIRIT_DEBUG_RULE(texp6);
          BOOST_SPIRIT_DEBUG_RULE(texp7);
          BOOST_SPIRIT_DEBUG_RULE(texp8);

          BOOST_SPIRIT_DEBUG_RULE(tbinop1);
          BOOST_SPIRIT_DEBUG_RULE(tbinop2);
          BOOST_SPIRIT_DEBUG_RULE(tbinop3);
          BOOST_SPIRIT_DEBUG_RULE(tbinop4);
          BOOST_SPIRIT_DEBUG_RULE(tbinop5);
          BOOST_SPIRIT_DEBUG_RULE(tbinop6);
          BOOST_SPIRIT_DEBUG_RULE(tbinop8);

          BOOST_SPIRIT_DEBUG_RULE(tunop7);

          BOOST_SPIRIT_DEBUG_RULE(texp_bracket);
          BOOST_SPIRIT_DEBUG_RULE(texp_terminal);

          
          tcall = args | (ch_p(':') >> name >> args);
          tsub = ch_p('[') >> exp >> ']';
          tsimplevar = (name | ('(' >> exp >> ')'));
          tdotted = ('.' >> name);
          tchained = tsimplevar >> +(tdotted | tcall | tsub);
          tvar =  tchained | tsimplevar ; // lexeme_d !!! reordering is a hack
          
          
          name = lexeme_d[alpha_p >> *(alnum_p|'_')];
          namelist = name >> *(',' >> name);
          number = lexeme_d[longest_d[(str_p("0x")>>hex_p) | real_p | int_p]];
          quote = ch_p('\"')|ch_p('\'');
          string_= quote >> *(anychar_p-quote) >> quote;
          
          chunk = end_p | (+(stat >> !ch_p(';')) >> !(laststat >> !ch_p(';')));
          block = chunk;
          
          while_statement = str_p("while") >> exp >> "do" >> block >> "end";
          do_ = str_p("do") >> block >> "end";
          repeat_ = str_p("repeat") >> block >> "until" >> exp;
          if_ = str_p("if") >> exp >> "then" >> block >> 
          *(str_p("elseif") >> exp >> "then" >> block)
          >> !(str_p("else") >> block) >> "end";
          for_ = str_p("for") >> name >> '=' >> exp >> ',' >> exp >> !(ch_p(',') >> exp) >> "do" >> block >> "end";
          for_in_ = str_p("for") >> namelist >> "in" >> explist >> "do" >> block >> "end";
          func_def_ = str_p("function") >> funcname >> funcbody;
          local_func_def_ = str_p("local") >> "function" >> name >> funcbody;
          local_def_ = str_p("local") >> namelist >> !(ch_p('=') >> explist);
          assignment = (varlist >> '=' >> explist);
          
          stat =  do_
                  |
                  while_statement
                  |
                  repeat_
                  |
                  if_
                  |
                  for_
                  |
                  for_in_
                  |
                  func_def_
                  |
                  local_func_def_
                  |
                  local_def_
                  | 
                  assignment
                  |
                  tvar;
          
          laststat = (str_p("return") >> !explist) | str_p("break");
          funcname = name >> *(ch_p('.') >> name) >> !(ch_p(':') >> name);
          varlist = var >> *(',' >> var);
          var = tvar;
          
          explist = *(exp >> ',') >> exp;
          binexp = exp >> binop >> exp;
          unexp = unop >> exp;
/*          exp = longest_d[str_p("nil") | "false" | "true" |
                          number | string_ | "..." |
                          function | tableconstructor | tvar | binexp | unexp];
*/          
          prefixexp = ('(' >> exp >> ')') | var | functioncall;
          functioncall = (tvar >> args) | tvar >> ':' >> name >> args;
          args = (ch_p('(') >> ')' ) // empty call
                  |
                  (ch_p('(') >> explist >> ')') // call with params
                  |
                  tableconstructor 
                  |
                  string_;
          function = str_p("function") >> funcbody;
          funcbody = ch_p('(') >> !parlist >> ')' >> block >> "end";
          parlist = (namelist >> !(ch_p(',') >> "...")) | "...";
          tableconstructor = ch_p('{') >> !fieldlist >> '}';
          fieldlist = field >> *(fieldsep >> field) >> !fieldsep;
          field = (ch_p('[') >> exp >> ']' >> '=' >> exp)
                  | 
                  (name >> '=' >> exp)
                  |
                  exp;
          fieldsep = ch_p(',') | ';';
          unop = ch_p('-') | "not" | '#';
          binop = ch_p('+') | "<=" | ">=" | "==" | "~=" | "and" | "or" | '-' | '*' | '/' | '^' | '%' | ".." |
          '<'  | '>' ; // order is important here so that >=       
          
/*        1 or
          2 and
          3 <     >     <=    >=    ~=    ==
          4 ..
          5 +     -
          6 *     /     %
          7 not   #     - (unary)
          8 ^
*/          
          texp_terminal = str_p("nil") | "false" | "true" | number | string_ | "..." ;
          
          tbinop8 = str_p("or");
          tbinop6 = str_p("and");
          tbinop5 = str_p("<=") | ">="  | "~=" | "==" | '<' | '>'  ;
          tbinop4 = str_p("..");
          tbinop3 = ch_p('+') | '-';
          tbinop2 = ch_p('*') | '/' | '%';
          tbinop1 = ch_p('^');
          
          tunop7 = str_p("not") | ch_p('#') | ch_p('-');
          
          texp_bracket = ch_p('(') >> texp >> ')';
          texp0 = texp_terminal | function | tableconstructor | texp_bracket | tvar;
          texp1 = texp0 >> *(tbinop1 >> texp0);
          texp2 = texp1 >> *(tbinop2 >> texp1);
          texp3 = texp2 >> *(tbinop3 >> texp2);
          texp4 = texp3 >> *(tbinop4 >> texp3);
          texp5 = texp4 >> *(tbinop5 >> texp4);
          texp6 = texp5 >> *(tbinop6 >> texp5);
          texp7 = (tunop7 >> texp6) | texp6;
          texp8 = texp7 >> *(tbinop8 >> texp7);
          texp = texp8;
          exp = texp;
        }
        
        rule<ScannerT> const& start() const {return block;};
      };
    };
  }
}


#endif