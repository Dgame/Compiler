#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <iostream>
#include <vector>
#include <list>
#include <map>

#include "Expression.hpp"
#include "VarManager.hpp"
#include "Loc.hpp"
#include "LabelSection.hpp"
#include "Env.hpp"

enum class Tok {
	Print,
	Var,
	Exit,
	If,
	Else,
	While,

	True,
	False,

	Equal,
	NotEqual,
	LessOrEqual,
	Less,
	GreaterOrEqual,
	Greater,

	And,
	Or,
	Xor,
	Not,
};

const std::map<Tok, const std::string> TokStr = {
	{Tok::Print, "print"},
	{Tok::Var, "var"},
	{Tok::Exit, "exit"},
	{Tok::If, "if"},
	{Tok::Else, "else"},
	{Tok::While, "while"},

	{Tok::True, "true"},
	{Tok::False, "false"},

	{Tok::Equal, "=="},
	{Tok::NotEqual, "!="},
	{Tok::LessOrEqual, "<="},
	{Tok::Less, "<"},
	{Tok::GreaterOrEqual, ">="},
	{Tok::Greater, ">"},

	{Tok::And, "and"},
	{Tok::Or, "or"},
	{Tok::Xor, "xor"},
	{Tok::Not, "not"},
};

enum class Label {
	PrintI,
	PrintlnI,
	PrintS,
	PrintlnS
};

const std::map<Label, const std::string> LabelStr = {
	{Label::PrintI, "_print_int"},
	{Label::PrintlnI, "_println_int"},
	{Label::PrintS, "_print_string"},
	{Label::PrintlnS, "_println_string"}
};

bool skipComment(Loc& loc);
void skipSpaces(Loc& loc);

bool read(Loc& loc, const std::string& what);
bool read(Loc& loc, Tok tok);
bool read(Loc& loc, char ch);

bool peek(Loc& loc, char what);

bool readNumber(Loc& loc, int* n);
bool readIdentifier(Loc& loc, std::string* identifier);

bool parsePrint(Env& env);

bool parseVar(Env& env);
bool parseVarAssign(Env& env, const std::string& name);

bool parseExit(Env& env);

bool parseLiteral(Env &env);
bool parseFactor(Env& env);
bool parseTerm(Env& env);
bool parseNumericExpression(Env& env);
bool parseCompareExpression(Env& env);
bool parseBooleanExpression(Env& env);

bool parseBlock(Env& env);
bool parseIf(Env& env);

bool parseCommand(Env& env);

#endif