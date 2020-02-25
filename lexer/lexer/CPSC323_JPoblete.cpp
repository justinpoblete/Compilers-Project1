/*Justin Poblete
Professor Le
CPSC 323-03
02/25/2020
Lexical Analyzer*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

struct holder {									//made some containers to hold the tokens and lexemes
	std::vector <std::string> token;
	std::vector <std::string> lexeme;
};

std::string checkKeyword(std::string lex) {		//checks if the identifier is a keyword
	std::string keys[18] = { "int", "float", "bool", "if", "else", "then", "endif", "while", 
		"whileend", "do","doend", "for", "forend", "input", "output", "and", "or", "function" };
	for (int i = 0; i < 18; ++i) {
		if (lex == keys[i]) {
			return "Keyword\t";
		}
	}
	return "Identifier";
}

bool isOperator(char lex) {						//two types of "other" symbols, so checks if char is an operator
	char ops[8] = { '+','-','*','/','>','<','%','=' };
	for (int i = 0; i < 8; ++i) {
		if (lex == ops[i]) {
			return true;
		}
	}
	return false;
}

bool isSeperator(char lex) {					//checks if char is a seperator
	int asciiVal[12] = { 33,39,40,41,44,46,58,59,91,93,123,125 };
	for (int i = 0; i < 12; ++i) {
		if (lex == asciiVal[i]) {
			return true;
		}
	}
	return false; 
}

holder lexer(std::string file) {				//lexer takes in the file name and returns containers of tokens and lexemes
	holder output;
	std::ifstream myFile;
	myFile.open(file);

	if (myFile.is_open()) {
		while (!myFile.eof()) {
			std::string line;
			getline(myFile, line);				//reads in a line at a time
			int length = line.length() + 1;
			char* arr = new char[length];		//create char array to hold in string
			strcpy_s(arr, length, line.c_str());


			int fsm[7][6] = {					//finite state machine that takes in chars
				{ 1, 3, 0, 0, 3, 5 },
				{ 1, 1, 1, 4, 0, 5 },
				{ 0, 2, 0, 4, 3, 5 },
				{ 0, 3, 0, 4, 0, 5 },
				{ 0, 0, 0, 0, 0, 5 },
				{ 5, 5, 5, 5, 5, 6 },
				{ 0, 0, 0, 0, 0, 0 }
			};
			int input = 0;						//input and state start at 0
			int state = 0;
			std::string temp;					//string that holds concatenation of chars

			for (int i = 0; i < length; ++i) {	
				if (isalpha(arr[i])) {			//if char is alpha then update state and concatenate to string
					input = 0;
					state = fsm[state][input];
					temp += arr[i];
				}
				else if (isdigit(arr[i])) {		//if char is digit ^^
					input = 1;
					state = fsm[state][input];
					temp += arr[i];
				}
				else if (arr[i] == '$') {		//if char is '$' ^^
					input = 2;
					state = fsm[state][input];
					temp += arr[i];
				}
				else if (arr[i] == '.') {		//if char is '.' ^^
					input = 4;
					state = fsm[state][input];
					temp += arr[i];
				}
				else if (arr[i] == '!') {		//if char is '!' ^^
					input = 5;
					state = fsm[state][input];
					temp += arr[i];
				}
				else {							//else case that handles all 'other' symbols
					if (state == 0) {			//checks if only char is operator or seperator
						if (isOperator(arr[i])) {	
							output.token.push_back("Operator"); 
							std::string x;
							x += arr[i];
							output.lexeme.push_back(x);
						}
						else if (isSeperator(arr[i])) {
							output.token.push_back("Seperator");
							temp += arr[i];
							std::string x;
							x += arr[i];
							output.lexeme.push_back(x);
						}
					}
					else if (state == 1) {		//check if string with an 'other' char at the end of it is an identifier, push back both lexeme and 'other'
						output.token.push_back(checkKeyword(temp));
						output.lexeme.push_back(temp);
						temp.clear();
						if (isOperator(arr[i])) {
							output.token.push_back("Operator"); 
							std::string x;
							x += arr[i];
							output.lexeme.push_back(x);
						}
						else if (isSeperator(arr[i])) {
							output.token.push_back("Seperator");
							std::string x;
							x += arr[i];
							output.lexeme.push_back(x);
						}
					}
					else if (state == 2) {		//check if string with an 'other' char at the end of it is an int, push back both lexeme and 'other'
						output.token.push_back("Integer");
						output.lexeme.push_back(temp);
						temp.clear();
						if (isOperator(arr[i])) {
							output.token.push_back("Operator");
							std::string x;
							x += arr[i];
							output.lexeme.push_back(x);
						}
						else if (isSeperator(arr[i])) {
							output.token.push_back("Seperator");
							std::string x;
							x += arr[i];
							output.lexeme.push_back(x);
						}
					}
					else if (state == 3) {		//check if string with an 'other' char at the end of it is a float, push back both lexeme and 'other'
						output.token.push_back("Float\t");
						output.lexeme.push_back(temp);
						temp.clear();
						if (isOperator(arr[i])) {
							output.token.push_back("Operator"); 
							std::string x;
							x += arr[i];
							output.lexeme.push_back(x);
						}
						else if (isSeperator(arr[i])) {
							output.token.push_back("Seperator");
							std::string x;
							x += arr[i];
							output.lexeme.push_back(x);
						}
					}
					else if (state == 4) {		//check if string is just a space with 'other'char at the end, push back just the 'other' char
						if (isOperator(arr[i])) {
							output.token.push_back("Operator"); 
							std::string x;
							x += arr[i];
							output.lexeme.push_back(x);
						}
						else if (isSeperator(arr[i])) {
							output.token.push_back("Seperator");
							std::string x;
							x += arr[i];
							output.lexeme.push_back(x);
						}
					}
					else if (state == 5) {		//check if string is at the start and in between comment
						temp.clear();
					}
					else if (state == 6) {		//check if string is at the end of comment
						temp.clear();
					}
					input = 3;					//char is 'other' then update state
					state = fsm[state][input];
				}

			}
		}
	}
	myFile.close();
	return output;
}

int main() {
	
	std::string fName;
	holder final;

	std::cout << "Enter file name (test.txt): ";	//prompt user to enter file name
	std::cin >> fName;

	final = lexer(fName);

	std::cout << "Tokens" << "\t\t\t" << "Lexemes\n" << std::endl;		//print out tokens and lexemes
	for (unsigned int i = 0; i < final.token.size(); ++i) {
		std::cout << final.token[i] << "\t=\t" << final.lexeme[i] << std::endl;
	}

	std::cout << "A text file was also generated (output.txt).\nThank you and Have a Good Day." << std::endl;	//also generate an output file with tokens and lexemes
	std::ofstream outFile;
	outFile.open("output.txt");
	outFile << "Tokens" << "\t\t\t" << "Lexemes\n" << std::endl;
	for (unsigned int i = 0; i < final.token.size(); ++i) {
		outFile << final.token[i] << "\t=\t" << final.lexeme[i] << std::endl;
	}
	outFile.close();
	system("PAUSE");
	return 0;
}