/*
 * Recep Erciyas
*/


#include "parseInt.h"
#include "val.h"
map<string, bool> defVar;

map<string, bool> tempDefVar;

map<string, Token> SymTable;

map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 

queue<Value> ValQue; //declare a pointer variable to a queue of Value objects

map<string, Value> decisionCheckerTemp;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line)
{
	bool f1, f2;
	LexItem tok = Parser::GetNextToken(in, line);
		
	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			
			tok = Parser::GetNextToken(in, line);
			if (tok.GetToken() == SEMICOL) {
				f1 = DeclBlock(in, line); 
			
				if(f1) {
					f2 = ProgBody(in, line);
					if(!f2)
					{
						ParseError(line, "Incorrect Program Body.");
						return false;
					}
					
					return true;//Successful Parsing is completed
				}
				else
				{
					ParseError(line, "Incorrect Declaration Section.");
					return false;
				}
			}
			else
			{
				//Parser::PushBackToken(tok);
				ParseError(line-1, "Missing Semicolon.");
				return false;
			}
		}
		else
		{
			ParseError(line, "Missing Program Name.");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	ParseError(line, "Missing PROGRAM.");
	return false;
}

bool ProgBody(istream& in, int& line){
	bool status;
		
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == BEGIN) {
		
		// cout<<"We are beginning program. The declared variables thus far are: "<<endl;
		// std::map<string,Token>::iterator iter;
      	// 	for(iter = SymTable.begin(); iter != SymTable.end(); ++iter){
		// 		cout<<"Key: "<<iter->first<<" Value: "<<iter->second<<endl;
      	// 	}
		status = Stmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Statement.");
				return false;
			}
			
			status = Stmt(in, line);
		}
			
		tok = Parser::GetNextToken(in, line);
		if(tok == END )
		{
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Program Body.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Program Body.");
		return false;
	}	
}//End of ProgBody function


bool DeclBlock(istream& in, int& line) {
	bool status = false;
	LexItem tok;
	//cout << "in Decl" << endl;
	LexItem t = Parser::GetNextToken(in, line);
	if(t == VAR)
	{
		status = DeclStmt(in, line);
		
		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Declaration Statement.");
				return false;
			}
			status = DeclStmt(in, line);
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok == BEGIN )
		{
			Parser::PushBackToken(tok);
			return true;
		}
		else 
		{
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Declaration Block.");
		return false;
	}
	
}//end of DeclBlock function

bool DeclStmt(istream& in, int& line)
{
	LexItem t;
	bool status = IdentList(in, line);
	
	if (!status)
	{
		ParseError(line, "Incorrect variable in Declaration Statement.");
		return status;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t == COLON)
	{
		t = Parser::GetNextToken(in, line);
		if(t == INTEGER || t == REAL || t == STRING)
		{

			std::map<string,bool>::iterator iter;
      		for(iter = tempDefVar.begin(); iter != tempDefVar.end(); ++iter){
				if(iter->second==false){
					iter->second=true;
					string tmp = iter->first;
					// cout<<"inserting into symtable: "<<tmp<<t.GetToken()<<endl;
					SymTable[tmp]=t.GetToken();
					//SymTable.insert({iter->first, "PROGRAM_VAR"});
				}
      		}
			tempDefVar.clear();
			return true;
		}
		else
		{
			ParseError(line, "Incorrect Declaration Type.");
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(t);
		return false;
	}
	
}//End of DeclStmt

//IdList:= IDENT {,IDENT}
bool IdentList(istream& in, int& line) {
	bool status = false;
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			tempDefVar[identstr] = false;
			defVar[identstr] = true;

		}	
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = IdentList(in, line);
	}
	else if(tok == COLON)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return status;
	
}//End of IdentList
	


//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
//Stmt = AssigStmt | IfStmt | WriteStmt 
bool Stmt(istream& in, int& line) {
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case WRITELN:
		//cout<<"WRITE LINE RECEIVED!!!"<<endl;
		status = WriteLnStmt(in, line);
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		
		break;

		
	default:
		Parser::PushBackToken(t);
		return false;
	}
	// cout<<"-----------"<<endl;
	// std::map<string,Value>::iterator iter;
    // for(iter = TempsResults.begin(); iter != TempsResults.end(); ++iter){
	// 	cout<<"key: "<<iter->first<<" value: "<<iter->second<<endl;
    // }
	// 	cout<<"-----------"<<endl;
	return status;
}//End of Stmt



//WriteStmt:= wi, ExpreList 
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
		
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}

	//Evaluate: print out the list of expressions' values
	while (!ValQue.empty())
	{
		Value nextVal = ValQue.front();
		cout << nextVal;
		ValQue.pop();
		//cout<<' , ';
	}

	cout << endl;
	
	return ex;
}






//IfStmt:= if (Expr) then Stm} [Else Stmt]
bool IfStmt(istream& in, int& line) {
	bool ex=false, status ; 
	LexItem t;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	Value logicVal;


	ex = LogicExpr(in, line, logicVal);

	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != THEN)
	{
		ParseError(line, "If-Stmt Syntax Error");
		return false;
	}


	if(logicVal.GetBool()){
		status = Stmt(in, line);
	}

	else if(!logicVal.GetBool()){
		decisionCheckerTemp = TempsResults;
		status = Stmt(in, line);
		TempsResults = decisionCheckerTemp;
	}

	if(!status)
	{
		ParseError(line, "Missing Statement for If-Stmt Then-Part");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if( t == ELSE ) {

		if(!logicVal.GetBool()){
			status = Stmt(in, line);
		}
		else{
			decisionCheckerTemp = TempsResults;
			status = Stmt(in, line);
			TempsResults = decisionCheckerTemp;
		}

		if(!status)
		{
			ParseError(line, "Missing Statement for If-Stmt Else-Part");
			return false;
		}
		//cout << "in IFStmt status of Stmt true" << endl;
		return true;
	}
	Parser::PushBackToken(t);
	return true;
}//End of IfStmt function


//Var:= ident
bool Var(istream& in, int& line, LexItem & idtok)
{
	//called only from the AssignStmt function
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == IDENT){
		identstr = tok.GetLexeme();
		
		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}
		idtok=LexItem(tok.GetToken(),tok.GetLexeme(),tok.GetLinenum());
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}//End of Var


//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	
	bool varstatus = false, status = false;
	LexItem t;
	
	LexItem id;
	varstatus = Var(in, line, id);
	
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		
		if (t == ASSOP){
			Value retVal;

			// Token variableToken=SymTable.find(id.GetLexeme())->second;

			// if(variableToken==INTEGER){
			// 	retVal.SetType(VINT);
			// }
			// else if(variableToken==REAL){
			// 	retVal.SetType(VREAL);
			// }
			// else if(variableToken==STRING){
			// 	retVal.SetType(VSTRING);
			// }
			// else{
			// 	ParseError(line, "Missing Expression in Assignment Statmentasf,masdnas");
			// 	return false;
			// }

			status = Expr(in, line, retVal); 


			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
			
			Token variableToken=SymTable.find(id.GetLexeme())->second;

			std::map<string,Value>::iterator itr;

			if(variableToken==INTEGER && (retVal.GetType()==VINT || retVal.GetType()==VREAL)){

				
				if(retVal.GetType()==VINT){
					if ( TempsResults.find(id.GetLexeme()) == TempsResults.end() ) {
  					// not found
					TempsResults[id.GetLexeme()]=retVal;
				} else {
 				 // found
					itr = TempsResults.find(id.GetLexeme());
      				if (itr != TempsResults.end()){
						itr->second = retVal;
					}
				}

				}

				if(retVal.GetType()==VREAL){
					
					Value realToInteger;
					realToInteger.SetType(VINT);
					int num = trunc(floor(retVal.GetReal()));
					realToInteger.SetInt(num);

					if ( TempsResults.find(id.GetLexeme()) == TempsResults.end() ) {
  					// not found
					TempsResults[id.GetLexeme()]=realToInteger;
					} else {
 				 	// found
						itr = TempsResults.find(id.GetLexeme());
      						if (itr != TempsResults.end()){
								itr->second = realToInteger;
							}
					}
				}

				
				
			}

			else if(variableToken==REAL && (retVal.GetType()==VINT || retVal.GetType()==VREAL)){

				if(retVal.GetType()==VINT){
					Value intToReal;
					intToReal.SetType(VREAL);
					float num = retVal.GetInt();
					intToReal.SetReal(num);
					if ( TempsResults.find(id.GetLexeme()) == TempsResults.end() ) {
  							// not found
							TempsResults[id.GetLexeme()]=intToReal;
						} else {
 							 // found
							itr = TempsResults.find(id.GetLexeme());
      						if (itr != TempsResults.end()){
								itr->second = intToReal;
							}		
						}

				}
				if(retVal.GetType()==VREAL){
						if ( TempsResults.find(id.GetLexeme()) == TempsResults.end() ) {
  							// not found
							TempsResults[id.GetLexeme()]=retVal;
						} else {
 							 // found
							itr = TempsResults.find(id.GetLexeme());
      						if (itr != TempsResults.end()){
								itr->second = retVal;
							}		
						}
				}


				// if ( TempsResults.find(id.GetLexeme()) == TempsResults.end() ) {
  				// // not found
				// 	TempsResults[id.GetLexeme()]=retVal;
				// } else {
 				//  // found
				// 	itr = TempsResults.find(id.GetLexeme());
      			// 	if (itr != TempsResults.end()){
				// 		itr->second = retVal;
				// 	}
				// }
			}

			else if(retVal.GetType()==VSTRING && variableToken==STRING ){

		
				if ( TempsResults.find(id.GetLexeme()) == TempsResults.end() ) {
  				// not found
					TempsResults[id.GetLexeme()]=retVal;
				} else {
 				 // found
					itr = TempsResults.find(id.GetLexeme());
      				if (itr != TempsResults.end()){
						itr->second = retVal;
					}
				}

			}
			
			else{
				//THROW ERROR SAYING IT DOESNT MATCH TYPE
				ParseError(line,"Illegal Assignment Operation");
			}

			
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;	
}



//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value exprVal;

	status = Expr(in, line, exprVal);
	// cout<<"RECEIVED EXPR TYPE: "<<exprVal.GetType()<<endl;
	// if(exprVal.IsString()){
	// 	cout<<"EXPR(str): "<<exprVal.GetString()<<endl;
	// }
	// if(exprVal.IsInt()){
	// 	cout<<"EXPR(int): "<<exprVal.GetInt()<<endl;
	// }
	// if(exprVal.IsReal()){
	// 	cout<<"EXPR(real): "<<exprVal.GetReal()<<endl;
	// }
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	ValQue.push(exprVal);
	// cout<<"PUSHED"<<endl;
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
	}

	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}


//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line,Value & retVal) {
	

	Value termValue;
	bool t1 = Term(in, line, termValue);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}


	if(termValue.IsInt()){
		retVal.SetInt(termValue.GetInt());
		retVal.SetType(VINT);
	}

	else if(termValue.IsReal()){
		retVal.SetReal(termValue.GetReal());
		retVal.SetType(VREAL);
	}

	else if(termValue.IsString()){
		retVal.SetString(termValue.GetString());
		retVal.SetType(VSTRING);
	}
	

	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	while ( tok == PLUS || tok == MINUS ) 
	{
		Value secondTerm;
		t1 = Term(in, line, secondTerm);

		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if(tok==PLUS){
			retVal = termValue.operator+(secondTerm);
			if(retVal.IsErr()){
				ParseError(line,"Illegal addition operation.");
				return false;
			}
		}

		if(tok==MINUS){
			retVal = termValue.operator-(secondTerm);
			if(retVal.IsErr()){
				ParseError(line,"Illegal subtraction operation.");
				return false;
			}
		}

		
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		termValue = retVal;
	}

	Parser::PushBackToken(tok);
	return true;
}






//Term:= SFactor {(*|/) SFactor}
bool Term(istream& in, int& line,Value & retVal) {
	
	Value sfacValue;
	bool t1 = SFactor(in, line,sfacValue);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	if(sfacValue.IsInt()){
		retVal.SetInt(sfacValue.GetInt());
		retVal.SetType(VINT);
	}
	else if(sfacValue.IsReal()){
		retVal.SetReal(sfacValue.GetReal());
		retVal.SetType(VREAL);
	}
	else if(sfacValue.IsString()){
		retVal.SetString(sfacValue.GetString());
		retVal.SetType(VSTRING);
	}


	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}

	while ( tok == MULT || tok == DIV  )
	{

		Value secondSfacVal;
		t1 = SFactor(in, line, secondSfacVal);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		
		if(tok==MULT){
			retVal = sfacValue.operator*(secondSfacVal);
			if(retVal.IsErr()){
				ParseError(line, "Run-Time Error-Illegal Multiplication");
				return false;
			}
		}

		if(tok==DIV){
			retVal = sfacValue.operator/(secondSfacVal);
			if(retVal.IsErr()){
				ParseError(line, "Run-Time Error-Illegal Division by Zero");
				return false;
			}
		}

		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}





//SFactor = Sign Factor | Factor
bool SFactor(istream& in, int& line,Value & retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else
		Parser::PushBackToken(t);

	
	Value factorValue;
	status = Factor(in, line, sign,factorValue);
	if(sign==1){
		// positive
		if(factorValue.GetType()==VINT){
			factorValue.SetInt(+factorValue.GetInt());
		}
		if(factorValue.GetType()==VREAL){
			factorValue.SetReal(+factorValue.GetReal());
		}
		if(factorValue.GetType()==VSTRING){
			factorValue.SetType(VERR);
			status= false;
		}
		

	}
	else if(sign==-1){
		//negative
		if(factorValue.GetType()==VINT){
			factorValue.SetInt(-factorValue.GetInt());
		}
		if(factorValue.GetType()==VREAL){
			factorValue.SetReal(-factorValue.GetReal());
		}
		if(factorValue.GetType()==VSTRING){
			factorValue.SetType(VERR);
			status= false;
		}
		
	}

	retVal = factorValue;

	return status;
}






//LogicExpr = Expr (== | <) Expr
bool LogicExpr(istream& in, int& line,Value & retVal)
{
	Value lefthandExpr;
	bool t1 = Expr(in, line, lefthandExpr);

	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == GTHAN  || tok == EQUAL  || tok == LTHAN)
	{
		Value rightExprVal;
		t1 = Expr(in, line, rightExprVal);
		if( !t1 ) 
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}	


		if(tok==GTHAN){
			
			retVal = lefthandExpr.operator>(rightExprVal);
		}

		if(tok==EQUAL){

			retVal = lefthandExpr.operator==(rightExprVal);

		}

		if(tok==LTHAN){
			
			retVal = lefthandExpr.operator<(rightExprVal);

		}

		if(retVal.IsErr()){
			return false;
		}
		return true;
	}
	Parser::PushBackToken(tok);
	return true;
}





//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, int sign,Value & retVal) {
	LexItem tok = Parser::GetNextToken(in, line);
	Value factorVal;
	if(tok == END){
		cout<<"FINISHED"<<endl;
		return true;
	}
	else if( tok == IDENT ) {
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undefined Variable");
			return false;	
		}
		if((TempsResults.find(lexeme)==TempsResults.end())){
			ParseError(line, "Undefined Variable");
			return false;
		}
		factorVal = TempsResults[lexeme];
		// cout<<"factorVal in Factorfunc is: "<<factorVal.GetType()<<endl;
		retVal=factorVal;
		return true;
	}

	else if( tok == ICONST ) {
		retVal.SetInt(stoi(tok.GetLexeme()));
		retVal.SetType(VINT);
		return true;
	}
	else if( tok == SCONST ) {
		retVal.SetString(tok.GetLexeme());
		retVal.SetType(VSTRING);
		return true;
	}
	else if( tok == RCONST ) {
		retVal.SetReal(stod(tok.GetLexeme()));
		retVal.SetType(VREAL);
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, factorVal);
		if( !ex ) {
			ParseError(line, "Missing expression after (");
			return false;
		}

		retVal=factorVal;

		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else 
		{

			Parser::PushBackToken(tok);
			ParseError(line, "Missing ) after expression");
			return false;

		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}
