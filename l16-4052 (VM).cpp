#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<stack>
using namespace std;
int tab = 0;
int index = 0;
ofstream fp;
ofstream ps;
ofstream tac;
ofstream st;
int addr = 0;
int n = 0;
stack <string> while_;
stack<string> if_;
string buffer1;
bool isFunc = false;
int cnt = 1;
int param = 0;
string func_buff;
int** quadaraple;

/*
P -> V | F  ( p for program  , v for variable , f for function)
V -> DT .:. id; (DT for datatypes)
DT -> int | char
F -> DT . id . (PL) . {SL} (pl for parameter list , sl for statement list)
PL -> PA | PL | ^ (pa for parameter , pl for parameter list)
PA -> DT . : . id
SL -> S | SL | ^ (sl for statement list)
S ->DS| AS | RS | BS | IS  | W | R | RET (As for assignment statement, rs for relational statement , Bs for branch statement , IS for iterative statement  , W for write , R for read , RET for return statment)
RS -> id | E  . > . id | E | id | E  .>= . id | E  |  id | E  . < .  id | E |  id | E  . < =.  id | E  |   id | E  .  /= .  id | E
DS -> DT() | ,
AS -> id. := . E ; | id.:=digit;
BS -> IF(E) {SL} | else {SL}
IS -> WHILE ( E ) {SL}
W-> write();  ; |  write (id);
R->read >> id ; | read >> digit;
RET-> return id ;| return digit;

id -> Alphabet | id. Alphabet
E -> E + T | E-T  | E * T | E / T |(E) | Digit | Alphabet
Digit -> 0| 1 | 2 | … | 9
Alphabet -> a | b | c | …… | z | A| B | C |…. | Z

*/

//this is a struct for token-lexeme pair
struct TokenLexemePair {

	string token = new char[100];
	string lexeme = new char[100];

	friend ostream& operator << (ostream& out, const TokenLexemePair& c);
};

//this is struct for opcode
struct Opcodes {

	string symbol;
	int opcode;


};

struct address {

	string variable_name;
	int relative_add;

};


address* add_arr;

//opcode Array
Opcodes* opcodesArray;

//overloading cout operator
ostream& operator << (ostream& out, const TokenLexemePair& c) {

	out << c.token;
	out << c.lexeme << endl;

	return out;

}

//look variable
TokenLexemePair look;

bool stop = false;

//making token-lexeme pair  global
TokenLexemePair* tokensArray;

//this function will character array to string
string convertToString(char* a, int size);

//this function will get a word and identify tokens for example keywords , valid/invalid identifier
int checkbuffer(char buffer[]);

//this function is lexical analyzer---making words out of characters
void lex(string filePath);

//counting no of line in file to run loop for parser
int countLines(string FilePath);

//this function will read file words.txt and make token-lexmem pair and populate array
TokenLexemePair* populateArray(int noOfLines, string filePath);

//this function will make opcodes array
Opcodes* populateArray2();

//this functions makes array of addresses and name opening translator symbol table
address* populateArray3(int noOfLines, string filePath);

//this is simple parser function
void parser();

//this  is simple virtual machine function
void VirtualMachine(int noOfLines , string filePath);

//this function will return tokens
TokenLexemePair nextToken();

//this function will write in parsetree file
void printTab(int tab);

//this function will write in symbol table file
void writeInFile(string buffer, string identifier, int val);

//this function will generate temporary variable for three adddress code
string produce_variable();

//this function will parse opcodes array and returns int opcode against operation
int getOpcode(string operation);

//this function will parse address array to return address of variable
int getVariableAdd(string var_name);

int op = 1;

string op_arr[13] = {"+" ,"-" , "/" , "*" ,"=" , ">" , ">=" , "<" , "<=" , "==" , "!="  , "<<", ">>"};

//functions used in parser
void P();
void V();
void F();
void DT();
void PL();
void SL();
string id();
void PL();
void PA();
void S();
void RS();
void BS();
void IS();
void AS();
void W();
void R();
void RET();
string digit();
void E();
void E1();
void E2();
void T1();
void T2();
void R1();
void R2();
void FL();
void DS();

string convertToString(char* a, int size)
{
	string s = a;
	return s;
}

TokenLexemePair nextToken() {

	string filePath = "words.txt";
	int lines = countLines(filePath);

	index++;
	for (; index < lines; index++)
	{
		look.lexeme = tokensArray[index].lexeme;
		look.token = tokensArray[index].token;

		return tokensArray[index];

	}

}

void writeInFile(string buffer1, string identifier, int val) {

	//datatype
	if (isFunc == true)

		st << "FUNC |";
	else
	{
		st << buffer1 << "|";
	}

	st.flush();
	//variable name
	st << identifier << "|";

	//relative address+ init val
	if (isFunc == true)

		st << "POINTER | 0";

	else
	{
		if (buffer1 == "int ")
		{
			st << addr << "| ";
			addr += 4;
		}

		else
		{
			st << addr << "|";
			addr += 1;
		}
	}

	st << val << "\n";

	st.flush();

}


string produce_variable() {

	string temp = "t";
	temp = "t" + to_string(cnt);
	cnt++;
	return temp;
}


void parser() {

	fp.open("parseTree.txt", ios::app);
	ps.open("parser-symboltable.txt", ios::app);
	tac.open("tac.txt", ios::app);
	st.open("translator-symboltable.txt", ios::app);

	if (ps.is_open() == true)

		ps << "data type \t identifier name\n";

	if (st.is_open() == true)

		st << "data type | variable name | relative address | initial value \n";

	st.flush();
	ps.flush();

	look.lexeme = tokensArray[index].lexeme;

	P();
}

void printTab(int tab) {

	for (size_t i = 0; i < tab; i++)
	{
		fp << "\t";
		fp.flush();
	}


	fp.flush();


}

void match(string str) {

	if (look.lexeme == str)
	{
		/*cout << look.lexeme << endl;*/
		printTab(tab);
		fp << look.lexeme << "\n";
		fp.flush();

		look.lexeme = nextToken().lexeme;

	}
	else
	{
		cout << "Bad token ...\n";
	}


}

//program
void P() {

	++tab;
	fp << "P";
	printTab(tab);

	V();

	F();
	--tab;
	fp.flush();

}

//variable
void V() {

	++tab;

	fp << "V";
	printTab(tab);

	DT();
	if (look.lexeme == ": ")
		match(": ");
	if (look.token == "identifier ")
	{
		writeInFile(buffer1, id(), 0);
		tac << buffer1 << "    ";
		tac << look.lexeme << "\n";
		//tac << buffer1;
		match(id());

		tac.flush();

	}
	if (look.lexeme == "; ")
		match("; ");
	--tab;


	fp.flush();
}

//function
void F() {

	++tab;

	fp << "F";
	printTab(tab);

	DT();
	if (look.lexeme == ": ")
		match(": ");
	if (look.token == "identifier ")
	{
		isFunc = true;
		writeInFile(buffer1, id(), 0);

		match(id());
		buffer1 = look.lexeme;
		isFunc = false;
		tac << look.lexeme << "    " << param << "\n";
		tac.flush();
	}
	if (look.lexeme == "( ")
	{

		match("( ");
	}
	PA();
	PL();
	if (look.lexeme == ") ")
		match(") ");
	if (look.lexeme == "{ ")
		match("{ ");
	SL();
	if (look.lexeme == "} ")
		match("} ");
	--tab;

	fp.flush();
}

//parameter
void PA() {

	++tab;

	fp << "PA";
	printTab(tab);

	V();
	PL();


	//tac <<"    " << param;
	//tac.flush();
	--tab;
	fp.flush();
}

//parameter list
void PL() {

	++tab;

	fp << "PL";
	printTab(tab);
	while (look.lexeme == ", ")
	{
		match(", ");
		PA();
		param++;

	}

	--tab;

	fp.flush();

}


//data type
void DT() {


	++tab;

	fp << "DT";
	printTab(tab);

	if (look.lexeme == "int ")

	{
		ps << "int\t";
		ps.flush();
		buffer1 = look.lexeme;
		match("int ");


	}
	if (look.lexeme == "char ")
	{
		ps << "char\t";
		ps.flush();
		buffer1 = look.lexeme;

		tac.flush();
	}
	--tab;


	fp.flush();

}

//statement list
void SL() {

	++tab;
	fp << "SL";

	if (stop != true) {
		printTab(tab);
		S();
		return;
		SL();
		--tab;

		fp.flush();
	}
}

//statement
void S() {

	++tab;

	fp << "S";
	printTab(tab);
	DS();
	AS();
	RS();
	BS();
	IS();
	W();
	R();
	RET();
	--tab;

	fp.flush();
}


//decleartion statement
void DS() {

	++tab;
	n++;
	fp << "DS";
	printTab(tab);
	V();

	while (look.lexeme == ", ")
	{
		match(", ");
		V();
	}

	if (look.lexeme == "; ")

		match("; ");

	V();
	--tab;
}

//assignment statements
void AS() {

	++tab;
	n++;
	fp << "AS";
	printTab(tab);
	if (look.lexeme == "int ")
	{
		match(look.lexeme);
	}

	if (look.lexeme == "char ")
	{
		match(look.lexeme);

	}

	if (look.token == "identifier ")
	{


		string var = produce_variable();
		tac << var << "=" << look.lexeme << "\n";
		tac.flush();
		match(id());

	}
	if (look.lexeme == ":= ")

		match(":= ");

	if (look.token == "identifier ")
	{
		string var = produce_variable();
		tac << var << " " << look.lexeme << "\n";
		tac.flush();
		match(id());

	}

	if (look.token == "numeric constant")
	{
		string var = produce_variable();
		tac << var << "=" << look.lexeme << "\n";
		tac.flush();
		match(digit());
	}
	if (look.lexeme == "; ")

		match("; ");
	--tab;

	fp.flush();
}

string digit() {

	++tab;


	fp << "digit";
	printTab(tab);
	if (look.token == "numeric constant ")
	{
		//match("numeric constant");
		return look.lexeme;
	}

	else {

		return "bad token...";
	}
	--tab;

	fp.flush();
}

//relational statements
void RS() {

	++tab;
	n++;
	fp << "RS";
	printTab(tab);
	E();
	id();

	if (look.lexeme == ">")
	{
		match(">");

		id();
		E();
	}

	E();
	id();

	if (look.lexeme == ">=")
	{
		match(">=");

		id();
		E();

	}


	E();
	id();

	if (look.lexeme == "<")
	{
		match("<");

		id();
		E();

	}
	E();
	id();

	if (look.lexeme == "<=")
	{
		match("<=");

		id();
		E();

	}
	E();
	id();

	if (look.lexeme == "!=")
	{
		match("!=");

		id();
		E();

	}
	E();
	id();

	if (look.lexeme == "==")
	{
		match("==");

		id();
		E();

	}
	--tab;

	fp.flush();
}

//expression
void E() {


	fp << "E";
	++tab;
	printTab(tab);
	E1();
	E2();
	--tab;
	fp.flush();
}

void E1() {

	++tab;

	fp << "E1";
	printTab(tab);
	T1();
	R1();
	--tab;

	fp.flush();
}
void E2() {


	++tab;

	fp << "E2";
	printTab(tab);
	T2();
	R2();

	--tab;

	fp.flush();
}
void T1() {

	++tab;

	fp << "T1";
	printTab(tab);
	--tab;
	fp.flush();
}
void T2() {

	++tab;

	fp << "T2";
	printTab(tab);
	--tab;

	fp.flush();


}

void R1() {


	++tab;

	fp << "R1";
	printTab(tab);
	if (look.lexeme == "+ ")

		match("+ ");

	FL();

	if (look.lexeme == " -")

		match("- ");

	FL();
	--tab;

	fp.flush();
}
void R2() {


	++tab;

	fp << "R2";
	printTab(tab);
	if (look.lexeme == "* ")

		match("* ");

	FL();

	if (look.lexeme == " /")

		match("/ ");

	FL();


	--tab;

	fp.flush();
}
void FL() {

	++tab;

	fp << "F";
	printTab(tab);
	if (look.token == "identifier ")

		match(id());

	if (look.token == "numeric constant ")

		match(digit());

	if (look.lexeme == "( ")
	{
		tac << "(";
		tac.flush();
		match("( ");
	}
	//E();

	if (look.lexeme == ") ")
	{
		tac << ")" << "\n";
		tac.flush();
		match(") ");
	}
	--tab;

	fp.flush();
}



//branch statements
void BS() {

	++tab;
	n++;
	fp << "BS";
	printTab(tab);
	if (look.lexeme == "if")
	{
		match("if");
		SL();
	}


	if (look.lexeme == "else")
	{

		match("else");
		SL();
	}

	--tab;

	fp.flush();
}

//iterative statements
void IS() {

	++tab;
	n++;
	fp << "IS";
	printTab(tab);
	if (look.lexeme == "while")
	{
		match("while");
		E();
		SL();
		tac << "if ";
		tac.flush();


	}

	--tab;

	fp.flush();
}

//write statements
void W() {

	++tab;
	n++;
	fp << "W";
	printTab(tab);
	if (look.lexeme == "write")

		match("write");
	if (look.lexeme == "( ")

		match("( ");

	if (look.token == "identifier ")

		match(id());

	if (look.lexeme == ") ")
		match(") ");

	if (look.lexeme == "; ")

		match("; ");

	--tab;

	fp.flush();
}

//read statements
void R() {

	++tab;
	n++;
	fp << "R";
	printTab(tab);
	if (look.lexeme == "read")
		match("read");

	if (look.lexeme == ">>")

		match(">>");

	if (look.token == "identifier ")

		match(id());


	if (look.token == "numeric constant ")

		match(digit());

	if (look.lexeme == "; ")

		match("; ");
	--tab;

	fp.flush();

}

//return statements
void RET() {

	++tab;
	n++;
	fp << "RET";
	printTab(tab);
	if (look.lexeme == "return")
		match("return");

	if (look.token == "identifier ")

		match(id());

	if (look.token == "numeric constant ")

		match(digit());

	if (look.lexeme == "; ")
	{
		match("; ");

		stop = false;
	}
	--tab;

	fp.flush();

}

//identifier
string id() {

	if (look.token == "identifier ")
	{
		//match("identifier");
		ps << look.lexeme << "\n";
		ps.flush();
		return look.lexeme;

	}
	else
	{
		//match("bad token");
		return "bad token";
	}

}

int getOpcode(string operation) {

	for (size_t i = 0; i < 13; i++)
	{
		if (opcodesArray[i].symbol == operation)
		{
			return opcodesArray[i].opcode;
		}
	}

	return -1;

}


int getVariableAdd(string variable) {

	int l = countLines("translation-symboltable.txt");

	for (size_t i = 0; i <l ; i++)
	{
		if (add_arr[i].variable_name==variable)
		{
			return add_arr[i].relative_add;
		}
	}

	return -1;
}

void VirtualMachine(int noOfLines , string filePath) {

	ifstream vm;
	ofstream mc;
	string line , temp_str;
	char* temp = new char[50];
	int opcode = 0, op1 = 0, op2 = 0, res = 0, jump_addr;
	string operation, operand_1, operand_2, result_1 ;
	int  i=0, j=0, k = 0 , l = 0 , size;
	//opening tac file
	vm.open(filePath);
	
	if (vm.is_open() == true)
	{
		mc.open("machine-code.txt");
		quadaraple = new int* [noOfLines];
		for (size_t i = 0; i < noOfLines; i++)
		{
			quadaraple[i] = new int[4];
		}

		while (vm.eof()==false)
		{

			getline(vm, line);


			for (; i < line.length(); )
			{
				//after getting line if we decleation statement we will ignore
				if (line.find("INT") != 0 || line.find("int") != 0 || line.find("Int") != 0 || line.find("CHAR") != 0 || line.find("char") != 0 || line.find("Char") != 0)

					break;
				else
				{
					//dividing string into parts
					//first we have result container
					while (line[j] != ' ')
					{
						temp[k++] = line[j++];

					}

					temp[k] = '\0';
					size = sizeof(temp) / sizeof(char);
					temp_str = convertToString(temp, size);
					result_1 = temp_str;
					k = 0;

					//second we have assignment operator
					while (line[j] != ' ')
					{
						temp[k++] = line[j++];

					}

					temp[k] = '\0';
					k = 0;

					//third we have operand 1
					while (line[j] != ' ')
					{
						temp[k++] = line[j++];

					}

					temp[k] = '\0';
					size = sizeof(temp) / sizeof(char);
					temp_str = convertToString(temp, size);
					operand_1 = temp_str;
					k = 0;

					//then we have operation
					while (line[j] != ' ')
					{
						temp[k++] = line[j++];

					}

					temp[k] = '\0';
					size = sizeof(temp) / sizeof(char);
					temp_str = convertToString(temp, size);
					operation = temp_str;
					k = 0;

					//then we have second operand
					while (line[j] != ' ')
					{
						temp[k++] = line[j++];

					}

					temp[k] = '\0';
					size = sizeof(temp) / sizeof(char);
					temp_str = convertToString(temp, size);
					operand_2 = temp_str;
					k = 0;

					//then we may have go to statement
					while (line[j] != ' ')
					{
						temp[k++] = line[j++];

					}

					temp[k] = '\0';
					k = 0;

					//then we may have address for goto
					while (line[j] != ' ')
					{
						temp[k++] = line[j++];

					}

					temp[k] = '\0';
					size = sizeof(temp) / sizeof(char);
					temp_str = convertToString(temp, size);
					//jump_addr = temp_str;
					if (temp_str.find("goto") != 0 || temp_str.find("GOTO") != 0 || temp_str.find("GoTo") != 0)
					{
						jump_addr = atoi(temp);
						goto writeGoto;
					}
					k = 0;

					//parsing respective arrays and getting opcodes or variable addresses
					opcode = getOpcode(operation);
					quadaraple[i][0] = opcode;

					op1 = getVariableAdd(operand_1);
					quadaraple[i][1] = op1;

					op2 = getVariableAdd(operand_2);
					quadaraple[i][2] = op2;

					res = getVariableAdd(result_1);
					quadaraple[i][3] = res;

					//writing in file
					mc << opcode << " " << op1 << " " << op2 << " " << res << "\n";
					mc.flush();

					break;

					//writing in file in-case of goto statement
				writeGoto:
					mc << opcode << " " << op1 << " " << op2 << " " << jump_addr << "\n";
					mc.flush();



				}

			}

		}

		

	}

	else
	{
		cout << "unable to open file";

	}

}

TokenLexemePair* populateArray(int noOfLines, string filePath) {

	tokensArray = new TokenLexemePair[noOfLines];

	ifstream fileRead(filePath);
	string line, temp_str;

	int i = 0, j = 0, k = 0, l = 0;
	int len = 0, size;
	char* temp = new char[100];



	if (fileRead.is_open())
	{

		while (fileRead.eof() == false && i < noOfLines)
		{

			//read full line
			getline(fileRead, line);
			j = 0;

			for (; j < line.length(); )
			{

				//first divde lexeme then token
				while (line[j] != '|')
				{
					temp[k++] = line[j++];
					l++;
				}


				j++;
				l++;
				temp[k] = '\0';



				size = sizeof(temp) / sizeof(char);

				temp_str = convertToString(temp, size);
				tokensArray[i].lexeme = temp_str;




				k = 0;
				for (j = l; j < line.length(); j++)
				{
					temp[k++] = line[j];
				}

				temp[k] = '\0';

				size = sizeof(temp) / sizeof(char);

				temp_str = convertToString(temp, size);
				tokensArray[i].token = temp_str;

				k = 0;
				j = 0;
				l = 0;
				k = 0;
				i++;

				break;


			}

		}

		return tokensArray;
	}

	else
	{
		cout << "unable to open file.\n";
		return nullptr;
	}


}

Opcodes* populateArray2() {

	opcodesArray = new Opcodes[13];

	for (size_t i = 0; i < 13; i++)
	{

		opcodesArray[i].symbol= op_arr[i];
		opcodesArray[i].opcode = op;
		op++;


	}

	return opcodesArray;

}

address* populateArray3(int noOfLines , string filePath ) {

	add_arr = new address[noOfLines-1];
	ifstream fileRead(filePath);
	string line, temp_str;

	int i = 0, j = 0, k = 0, l = 0;
	int len = 0, size;
	char* temp = new char[100];
	getline(fileRead, line);


	if (fileRead.is_open())
	{

		while (fileRead.eof() == false && i < noOfLines)
		{

			//read full line
			getline(fileRead, line);
			j = 0;

			for (; j < line.length(); )
			{

				//reading till  first |
				while (line[j] != '|')
				{
					temp[k++] = line[j++];
					l++;
				}

			
				j++;
				l++;
				temp[k] = '\0';


				k = 0;
				while (line[j] != '|')
				{
					temp[k++] = line[j++];
					l++;
				}

			
				j++;
				l++;
				temp[k] = '\0';

				size = sizeof(temp) / sizeof(char);
				temp_str = convertToString(temp, size);
				k = 0;
				int t = atoi(temp);
				add_arr[i].variable_name=t;



				//k = 0;
				//j = 0;
				//l = 0;
				//k = 0;
				
				while (line[j] != '|')
				{
					temp[k++] = line[j++];
					l++;
				}


				j++;
				l++;
				temp[k] = '\0';

				size = sizeof(temp) / sizeof(char);
				temp_str = convertToString(temp, size);
				k = 0;
				t = atoi(temp);
				add_arr[i].relative_add = t;


				k = 0;
				j = 0;
				l = 0;
				k = 0;
				i++;

				break;


			}

		}

		return add_arr;
	}

	else
	{
		cout << "unable to open file.\n";
		return nullptr;
	}


}

int countLines(string FilePath) {

	ifstream fileOpen(FilePath);
	string line;
	int count = 0;


	if (fileOpen.is_open() == true) {

		while (getline(fileOpen, line))
		{
			count++;
		}

		fileOpen.close();
		return count;
	}

	else
	{
		return -1;
	}

}


int checkbuffer(char buffer[]) {


	char dataTypes[6][15] = { "int","char", "float" , "double" ,"long",  "string" };
	char keywords[17][15] = { "print" , "while" , "for" ,  "if" , "else" , "read" ,"write", "return", "void" , "do", "switch","case" , "signed", "unsigned" , "break" , "goto", "continue" };
	bool valid = false;

	//datatypes
	for (int j = 0; j < 6; j++)
	{

		if (strcmp(dataTypes[j], buffer) == 0)
		{
			return 1;
		}
	}

	for (int j = 0; j < 17; j++)
	{

		if (strcmp(keywords[j], buffer) == 0)
		{
			return 6;
		}
	}

	//digit
	if (buffer[0] >= '0' && buffer[0] <= '9')
	{
		return 2;
	}

	//identifiers
	else if ((buffer[0] >= 33 && buffer[0] <= 126) && buffer[0] != 60 && buffer[0] != 62)
	{
		int l = strlen(buffer);
		for (int i = 0; i < l; i++)
		{
			//valid identifier
			if (i > 0)
			{
				if ((buffer[i] >= 'A' && buffer[i] <= 'Z') || (buffer[i] >= 'a' && buffer[i] <= 'z') || (buffer[i] >= '0' && buffer[i] <= '9'))
				{
					valid = true;
				}

				else
				{
					//invalid idetifier
					valid = false;
					return -3;
				}
			}

			else
			{

				if ((buffer[i] >= 'A' && buffer[i] <= 'Z') || (buffer[i] >= 'a' && buffer[i] <= 'z'))
				{

					valid = true;
				}

				//idetifier starting from symbol  return
				else
				{
					valid = false;
					goto relational_operator;
					//return -3;
				}
			}
		}

		//valid idetifier
		if (valid == true)
		{
			return 3;
		}

	}


relational_operator:

	if (buffer[0] == '!' || buffer[0] == '<' || buffer[0] == '>' || buffer[0] == '=')
	{

		//relational operator
		if (buffer[1] == '=')
		{
			return 4;
		}

		//input operator
		else if (buffer[1] == '>')
		{
			return 5;
		}

		//valid
		else
		{
			return 4;
		}
	}

	if (valid == false)

		return -3;
}


void lex(string filePath) {

	fstream File;
	ofstream file;
	ofstream file2;
	char ch;
	char brackets[] = { '(',')','{','}','[',']','\0' };
	char Aroperators[] = { '+','-','/','*','\0' };
	char buffer[32];
	int j = 0;
	bool checked = false;
	int val = 0;

	File.open(filePath.c_str(), ios::in);
	file.open("words.txt", ios::app);
	file2.open("symboltable.txt", ios::app);

	//checking tokens which are of 1 character
	if (File.is_open())
	{
		while (!File.eof())
		{

			ch = File.get();


			//single line comment

			if (ch == '/' && File.peek() == '/')
			{
				while (ch != '\n')
				{
					ch = File.get();
				}

				ch = File.get();
			}

			//multi-line comment

			if (ch == '/' && File.peek() == '*')
			{
				ch = File.get();
				while (ch != '/')
				{
					ch = File.get();
				}

				ch = File.get();
			}

			//ignore tabs and newline
			while (ch == ' ' || ch == '\n' || ch == '\t')
			{
				ch = File.get();


				//single line comment

				if (ch == '/' && File.peek() == '/')
				{
					while (ch != '\n')
					{
						ch = File.get();
					}

					ch = File.get();
				}

				//multi-line comment

				if (ch == '/' && File.peek() == '*')
				{
					ch = File.get();
					while (ch != '/')
					{
						ch = File.get();
					}

					ch = File.get();
				}
			}

			for (int i = 0; i < 6; i++)
			{
				//brackets check
				if (ch == brackets[i])
				{

					file << ch << " |Bracket\n";
					checked = true;
					j = 0;
					break;


				}
			}

			//arithmetic operators check
			for (int i = 0; i < 4; i++)
			{
				if (ch == Aroperators[i])
				{

					file << ch << " |Arithmetic Operators\n";
					checked = true;
					j = 0;
					break;
				}
			}


			//commma , colon , semi-colon check
			if (checked == false)
			{
				if (ch == ':')
				{

					if (File.get() == '=')
					{
						file << ":= |assignment operator\n";
						j = 0;
					}

					//file << ch << " |Colon.\n";


				}

				else if (ch == ';')
				{

					file << ch << " |semiColon.\n";
					j = 0;


				}

				else if (ch == ',')
				{


					file << ch << " |comma.\n";
					j = 0;


				}

				else if (ch != ' ' || ch != ';' || ch != ':')
				{
					buffer[j++] = ch;


					if ((File.peek() == ' ' || File.peek() == ':' || File.peek() == ';' || File.peek() == ',' || File.peek() == '\n' || File.peek() == '\t') && j != 0)
					{
						buffer[j++] = '\0';
						j = 0;

						val = checkbuffer(buffer);

						if (val == 1)
						{
							file << buffer << " |datatype " << endl;
						}

						else if (val == 6)
						{
							file << buffer << "|keyword " << endl;
						}

						else if (val == 2)
						{

							file << buffer << " |numeric constant " << endl;

						}

						else if (val == 3)
						{
							file << buffer << " |identifier " << endl;
							file2 << "Identifier| " << buffer << endl;
						}

						else if (val == -3)
						{
							cout << "invalid identifier" << buffer << endl;
						}

						else if (val == 4)
						{
							file << buffer << "|relational operator" << endl;
						}

						else if (val == 5)
						{
							file << buffer << "|input operator" << endl;
						}

						else
						{
							cout << "Not a valid character.\n";
						}
					}

					else
					{
						//cout << "isspace";
					}
				}

				checked = false;

			}

			checked = false;
		}


	}

	else
	{
		cout << "unable to open file.\n";
		return;
	}
}


int main() {



	string filePath;
	
	cout << "Input file path:\n";
	cin >> filePath;
	lex(filePath);

	//filePath = "words.txt";
	filePath = "words.txt";

	populateArray((countLines(filePath)), filePath);
	//cout << endl << tokensArray[5] << endl;

	parser();

	
	populateArray2();

	filePath = "translator-symboltable.txt";
	populateArray3((countLines(filePath)), filePath);
	filePath = "tac.txt";
	VirtualMachine((countLines(filePath)) , filePath);

	system("pause");

	return 0;
}