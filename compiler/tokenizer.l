%option noyywrap yylineno

%{
%}

%%

program { printf("Key[program]\n"); }
const { printf("Key[const]\n"); }
begin { printf("Key[begin]\n"); }
end { printf("Key[end]\n"); }
var { printf("Key[var]\n"); }
integer { printf("Key[integer]\n"); }
while { printf("Key[while]\n"); }
if { printf("Key[if]\n"); }
then { printf("Key[then]\n"); }
else { printf("Key[else]\n"); }
do { printf("Key[do]\n"); }
procedure { printf("Key[procedure]\n"); }
"=" { printf("Symbol[=]\n"); }
"<" { printf("Symbol[<]\n"); }
"<=" { printf("Symbol[<=]\n"); }
">" { printf("Symbol[>]\n"); }
">=" { printf("Symbol[>=]\n"); }
"<>" { printf("Symbol[<>]\n"); }
"+" { printf("Symbol[+]\n"); }
"*" { printf("Symbol[*]\n"); }
"-" { printf("Symbol[-]\n"); }
"," { printf("Symbol[,]\n"); }
":" { printf("Symbol[:]\n"); }
";" { printf("Symbol[;]\n"); }
":=" { printf("Symbol[:=]\n"); }
[a-zA-Z]+ { printf("Id[%s]\n", yytext); }
[1-9][0-9]* {printf("Number[%s]\n", yytext); }
. 

%%
int main(int argc, char **argv)
{
 yylex();
}
