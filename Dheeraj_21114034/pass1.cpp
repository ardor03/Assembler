#include "utility.cpp"  
#include "tables.cpp"   
using namespace std;
string fileName;
bool error_flag=false; // variable to store if the program has some error or not .
int length_of_the_program; // variable to store the length of the program
string *BLocksNumToName;

// Manipulate_EXPRESSION : It uses pass by reference . We use a while loop to get the symbols
// from the expressio . If the symbol is not found in the SYMTAB , we keep the error message in the error file .
// We use a variable count_of_pairs which keeps the account of whether the expression is absolute or relative
// and if the count_of_pairs gives some unexpected value , we print an error message .

void Manipulate_EXPRESSION(string expression, bool& relative,string& dheeraj_kumarOperand,int lineNumber, ofstream& errorFile,bool& error_flag){
  string SINGLEOPERAND="?"; // intializing the string singleoperand
  string SINGLEOPERATOR="?";// initializing the string singleoperator
  string VALUESTRING="";// initializing the string valuestring
  string valuedheeraj_kumar=""; // initializing the string valuedheeraj_kumar
  string writing_the_data=""; // initializing the string writing_the_data
  int the_last_operand=0,lastOperator=0,count_of_pairs=0; // initializing all the var to zero
  char the_last_byte = ' '; // initializing byte with a blank space
  bool Illegal = false; // initializing Illegal with false initially !

  for(int i=0;i<expression.length();){
    SINGLEOPERAND = "";

    the_last_byte = expression[i];
    while((the_last_byte!='+' && the_last_byte!='-' && the_last_byte!='/' && the_last_byte!='*') && i<expression.length()){
      SINGLEOPERAND += the_last_byte;
      the_last_byte = expression[++i];
    }

    if(SYMTAB[SINGLEOPERAND].exists=='y'){
      the_last_operand = SYMTAB[SINGLEOPERAND].relative;
      valuedheeraj_kumar = to_string(convert_string_hex_to_int(SYMTAB[SINGLEOPERAND].address));
    }
    else if((SINGLEOPERAND != "" || SINGLEOPERAND !="?" ) && if_all_num(SINGLEOPERAND)){
      the_last_operand = 0;
      valuedheeraj_kumar = SINGLEOPERAND;
    }
    else{
      writing_the_data = "Line: "+to_string(lineNumber)+" : Can't find symbol. Found "+SINGLEOPERAND;
      write_to_file(errorFile,writing_the_data);
      Illegal = true;
      break;
    }

    if(the_last_operand*lastOperator == 1){//Check expressions legallity
      writing_the_data = "Line: "+to_string(lineNumber)+" : Illegal expression";
      write_to_file(errorFile,writing_the_data);
      error_flag = true;
      Illegal = true;
      break;
    }
    else if((SINGLEOPERATOR=="-" || SINGLEOPERATOR=="+" || SINGLEOPERATOR=="?")&&the_last_operand==1){
      if(SINGLEOPERATOR=="-"){
        count_of_pairs--;
      }
      else{
        count_of_pairs++;
      }
    }

    VALUESTRING += valuedheeraj_kumar;

    SINGLEOPERATOR= "";
    while(i<expression.length()&&(the_last_byte=='+'||the_last_byte=='-'||the_last_byte=='/'||the_last_byte=='*')){
      SINGLEOPERATOR += the_last_byte;
      the_last_byte = expression[++i];
    }

    if(SINGLEOPERATOR.length()>1){
      writing_the_data = "Line: "+to_string(lineNumber)+" : Illegal operator in expression. Found "+SINGLEOPERATOR;
      write_to_file(errorFile,writing_the_data);
      error_flag = true;
      Illegal = true;
      break;
    }

    if(SINGLEOPERATOR=="*" || SINGLEOPERATOR == "/"){
      lastOperator = 1;
    }
    else{
      lastOperator = 0;
    }

    VALUESTRING += SINGLEOPERATOR;
  }

  if(!Illegal){
    if(count_of_pairs==1){
      relative = 1;
      dheerajEvaluateString dheeraj_kumarOBJ(VALUESTRING);
      dheeraj_kumarOperand = convert_int_to_string_hexadecimal(dheeraj_kumarOBJ.getResult());
    }
    else if(count_of_pairs==0){
            relative = 0;
      cout<<VALUESTRING<<endl;
      dheerajEvaluateString dheeraj_kumarOBJ(VALUESTRING);
      dheeraj_kumarOperand = convert_int_to_string_hexadecimal(dheeraj_kumarOBJ.getResult());
    }
    else{
      writing_the_data = "Line: "+to_string(lineNumber)+" : Illegal expression";
      write_to_file(errorFile,writing_the_data);
      error_flag = true;
      dheeraj_kumarOperand = "00000";
      relative = 0;
    }
  }
  else{
    dheeraj_kumarOperand = "00000";
    error_flag = true;
    relative = 0;
  }
}


// to_handle_the_LTORG : It uses pass by reference , We print the literal pool present till time by taking the arguments from the pass1() function .
// We run an iterator to print all the literals present in the LITTAB and then update the line number .
// If for some literal , we did not find the address , we store the present address in the LITTAB
// and then increment the LOCATION_COUNTER on the basis of literal present . 
void to_handle_the_LTORG(string& litPrefix, int& lineNumberDelta,int lineNumber,int& LOCATION_COUNTER, int& LAST_DELTA_LOCATION_COUNTER, int currentBlockNumber){
  string LITADDRESS;
  string LITVALUE;
  litPrefix = "";
  for(auto const& it: LITTAB){
    LITADDRESS = it.second.address;
    LITVALUE = it.second.value;
    if(LITADDRESS!="?"){
          }
    else{
      lineNumber += 5;
      lineNumberDelta += 5;
      LITTAB[it.first].address = convert_int_to_string_hexadecimal(LOCATION_COUNTER);
      LITTAB[it.first].blockNumber = currentBlockNumber;
      litPrefix += "\n" + to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(LOCATION_COUNTER) + "\t" + to_string(currentBlockNumber) + "\t" + "*" + "\t" + "="+LITVALUE + "\t" + " " + "\t" + " ";

      if(LITVALUE[0]=='X'){
        LOCATION_COUNTER += (LITVALUE.length() -3)/2;
        LAST_DELTA_LOCATION_COUNTER += (LITVALUE.length() -3)/2;
      }
      else if(LITVALUE[0]=='C'){
        LOCATION_COUNTER += LITVALUE.length() -3;
        LAST_DELTA_LOCATION_COUNTER += LITVALUE.length() -3;
      }
    }
  }
}


void pass1(){
  ifstream sourceFile;
  ofstream intermediateFile, errorFile;

  sourceFile.open(fileName);
  if(!sourceFile){
    cout<<"Unable to open file: "<<fileName<<endl;
    exit(1);
  }

  intermediateFile.open("intermediate_" + fileName);
  if(!intermediateFile){
    cout<<"Unable to open file: intermediate_"<<fileName<<endl;
    exit(1);
  }
  write_to_file(intermediateFile,"Line\tAddress\tLabel\tOPCODE\tOPERAND\tComment");
  errorFile.open("error_"+fileName);
  if(!errorFile){
    cout<<"Unable to open file: error_"<<fileName<<endl;
    exit(1);
  }
  write_to_file(errorFile,"************PASS1************");

  string fileLine;
  string writing_the_data,writing_the_dataSuffix="",writing_the_dataPrefix="";
  int index=0;

  string currentBlockName = "";
  int currentBlockNumber = 0;
  int totalBlocks = 1;

  bool dheeraj_status_code;
  string label,opcode,operand,comment;
  string dheeraj_kumarOperand;

  int startAddress,LOCATION_COUNTER,saveLOCATION_COUNTER,lineNumber,LAST_DELTA_LOCATION_COUNTER,lineNumberDelta=0;
  lineNumber = 0;
  LAST_DELTA_LOCATION_COUNTER = 0;

  getline(sourceFile,fileLine);
  lineNumber += 5;
  while(check_comment_line(fileLine)){
    writing_the_data = to_string(lineNumber) + "\t" + fileLine;
    write_to_file(intermediateFile,writing_the_data);
    getline(sourceFile,fileLine);
    lineNumber += 5;
    index = 0;
  }

  read_first_non_white_space(fileLine,index,dheeraj_status_code,label);
  read_first_non_white_space(fileLine,index,dheeraj_status_code,opcode);

  if(opcode=="START"){
    read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);
    read_first_non_white_space(fileLine,index,dheeraj_status_code,comment,true);
    startAddress = convert_string_hex_to_int(operand);
      LOCATION_COUNTER = startAddress;
    writing_the_data = to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER) + "\t" + to_string(currentBlockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment;
    write_to_file(intermediateFile,writing_the_data); 

    getline(sourceFile,fileLine); 
    lineNumber += 5;
    index = 0;
    read_first_non_white_space(fileLine,index,dheeraj_status_code,label); 
    read_first_non_white_space(fileLine,index,dheeraj_status_code,opcode);
  }
  else{
    startAddress = 0;
    LOCATION_COUNTER = 0;
  }

  while(opcode!="END"){
    if(!check_comment_line(fileLine)){
      if(label!=""){

        if(SYMTAB[label].exists=='n'){
          SYMTAB[label].name = label;
          SYMTAB[label].address = convert_int_to_string_hexadecimal(LOCATION_COUNTER);
          SYMTAB[label].relative = 1;
          SYMTAB[label].exists = 'y';
          SYMTAB[label].blockNumber = currentBlockNumber;
        }
        else{
          writing_the_data = "Line: "+to_string(lineNumber)+" : Duplicate symbol for '"+label+"'. Previously defined at "+SYMTAB[label].address;
          write_to_file(errorFile,writing_the_data);
          error_flag = true;
        }
      }
      if(OPTAB[REAL_OPCODE(opcode)].exists=='y'){
        if(OPTAB[REAL_OPCODE(opcode)].format==3){
          LOCATION_COUNTER += 3;
          LAST_DELTA_LOCATION_COUNTER += 3;
          if(FORMAT_FLAG(opcode)=='+'){
            LOCATION_COUNTER += 1;
            LAST_DELTA_LOCATION_COUNTER += 1;
          }
          if(REAL_OPCODE(opcode)=="RSUB"){
            operand = " ";
          }
          else{
            read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);
            if(operand[operand.length()-1] == ','){
              read_first_non_white_space(fileLine,index,dheeraj_status_code,dheeraj_kumarOperand);
              operand += dheeraj_kumarOperand;
            }
          }

          if(FORMAT_FLAG(operand)=='='){
            dheeraj_kumarOperand = operand.substr(1,operand.length()-1);
            if(dheeraj_kumarOperand=="*"){
              dheeraj_kumarOperand = "X'" + convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER,6) + "'";
            }
            if(LITTAB[dheeraj_kumarOperand].exists=='n'){
              LITTAB[dheeraj_kumarOperand].value = dheeraj_kumarOperand;
              LITTAB[dheeraj_kumarOperand].exists = 'y';
              LITTAB[dheeraj_kumarOperand].address = "?";
              LITTAB[dheeraj_kumarOperand].blockNumber = -1;
            }
          }
        }
        else if(OPTAB[REAL_OPCODE(opcode)].format==1){
          operand = " ";
          LOCATION_COUNTER += OPTAB[REAL_OPCODE(opcode)].format;
          LAST_DELTA_LOCATION_COUNTER += OPTAB[REAL_OPCODE(opcode)].format;
        }
        else{
          LOCATION_COUNTER += OPTAB[REAL_OPCODE(opcode)].format;
          LAST_DELTA_LOCATION_COUNTER += OPTAB[REAL_OPCODE(opcode)].format;
          read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);
          if(operand[operand.length()-1] == ','){
            read_first_non_white_space(fileLine,index,dheeraj_status_code,dheeraj_kumarOperand);
            operand += dheeraj_kumarOperand;
          }
        }
      }
      else if(opcode == "WORD"){
        read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);
        LOCATION_COUNTER += 3;
        LAST_DELTA_LOCATION_COUNTER += 3;
      }
      else if(opcode == "RESW"){
        read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);
        LOCATION_COUNTER += 3*stoi(operand);
        LAST_DELTA_LOCATION_COUNTER += 3*stoi(operand);
      }
      else if(opcode == "RESB"){
        read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);
        LOCATION_COUNTER += stoi(operand);
        LAST_DELTA_LOCATION_COUNTER += stoi(operand);
      }
      else if(opcode == "BYTE"){
        readByteOperand(fileLine,index,dheeraj_status_code,operand);
        if(operand[0]=='X'){
          LOCATION_COUNTER += (operand.length() -3)/2;
          LAST_DELTA_LOCATION_COUNTER += (operand.length() -3)/2;
        }
        else if(operand[0]=='C'){
          LOCATION_COUNTER += operand.length() -3;
          LAST_DELTA_LOCATION_COUNTER += operand.length() -3;
        }
      }
      else if(opcode=="BASE"){
        read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);
      }
      else if(opcode=="LTORG"){
        operand = " ";
        to_handle_the_LTORG(writing_the_dataSuffix,lineNumberDelta,lineNumber,LOCATION_COUNTER,LAST_DELTA_LOCATION_COUNTER,currentBlockNumber);
      }
      else if(opcode=="ORG"){
        read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);

        char the_last_byte = operand[operand.length()-1];
        while(the_last_byte=='+'||the_last_byte=='-'||the_last_byte=='/'||the_last_byte=='*'){
          read_first_non_white_space(fileLine,index,dheeraj_status_code,dheeraj_kumarOperand);
          operand += dheeraj_kumarOperand;
          the_last_byte = operand[operand.length()-1];
        }

        int dheeraj_kumarVariable;
        dheeraj_kumarVariable = saveLOCATION_COUNTER;
        saveLOCATION_COUNTER = LOCATION_COUNTER;
        LOCATION_COUNTER = dheeraj_kumarVariable;

        if(SYMTAB[operand].exists=='y'){
          LOCATION_COUNTER = convert_string_hex_to_int(SYMTAB[operand].address);
        }
        else{
          bool relative;
        
          error_flag = false;
          Manipulate_EXPRESSION(operand,relative,dheeraj_kumarOperand,lineNumber,errorFile,error_flag);
          if(!error_flag){
            LOCATION_COUNTER = convert_string_hex_to_int(dheeraj_kumarOperand);
          }
          error_flag = false;//reset error_flag
        }
      }
      else if(opcode=="USE"){

        read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);
        BLOCKS[currentBlockName].LOCATION_COUNTER = convert_int_to_string_hexadecimal(LOCATION_COUNTER);

        if(BLOCKS[operand].exists=='n'){

          BLOCKS[operand].exists = 'y';
          BLOCKS[operand].name = operand;
          BLOCKS[operand].number = totalBlocks++;
          BLOCKS[operand].LOCATION_COUNTER = "0";
        }

        currentBlockNumber = BLOCKS[operand].number;
        currentBlockName = BLOCKS[operand].name;
        LOCATION_COUNTER = convert_string_hex_to_int(BLOCKS[operand].LOCATION_COUNTER);

      }
      else if(opcode=="EQU"){
        read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);
        dheeraj_kumarOperand = "";
        bool relative;

        if(operand=="*"){
          dheeraj_kumarOperand = convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER,6);
          relative = 1;
        }
        else if(if_all_num(operand)){
          dheeraj_kumarOperand = convert_int_to_string_hexadecimal(stoi(operand),6);
          relative = 0;
        }
        else{
          char the_last_byte = operand[operand.length()-1];

          while(the_last_byte=='+'||the_last_byte=='-'||the_last_byte=='/'||the_last_byte=='*'){
            read_first_non_white_space(fileLine,index,dheeraj_status_code,dheeraj_kumarOperand);
            operand += dheeraj_kumarOperand;
            the_last_byte = operand[operand.length()-1];
          }
          Manipulate_EXPRESSION(operand,relative,dheeraj_kumarOperand,lineNumber,errorFile,error_flag);
        }

        SYMTAB[label].name = label;
        SYMTAB[label].address = dheeraj_kumarOperand;
        SYMTAB[label].relative = relative;
        SYMTAB[label].blockNumber = currentBlockNumber;
        LAST_DELTA_LOCATION_COUNTER = LOCATION_COUNTER - convert_string_hex_to_int(dheeraj_kumarOperand);
      }
      else{
        read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);
        writing_the_data = "Line: "+to_string(lineNumber)+" : Invalid OPCODE. Found " + opcode;
        write_to_file(errorFile,writing_the_data);
        error_flag = true;
      }
      read_first_non_white_space(fileLine,index,dheeraj_status_code,comment,true);
      if(opcode=="EQU" && SYMTAB[label].relative == 0){
        writing_the_data = writing_the_dataPrefix + to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writing_the_dataSuffix;
      }
      else{
        writing_the_data = writing_the_dataPrefix + to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER) + "\t" + to_string(currentBlockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writing_the_dataSuffix;
      }
      writing_the_dataPrefix = "";
      writing_the_dataSuffix = "";
    }
    else{
      writing_the_data = to_string(lineNumber) + "\t" + fileLine;
    }
    write_to_file(intermediateFile,writing_the_data);

    BLOCKS[currentBlockName].LOCATION_COUNTER = convert_int_to_string_hexadecimal(LOCATION_COUNTER);//Update LOCATION_COUNTER of block after every instruction
    getline(sourceFile,fileLine); //Read next line
    lineNumber += 5 + lineNumberDelta;
    lineNumberDelta = 0;
    index = 0;
    LAST_DELTA_LOCATION_COUNTER = 0;
    read_first_non_white_space(fileLine,index,dheeraj_status_code,label); //Parse label
    read_first_non_white_space(fileLine,index,dheeraj_status_code,opcode);//Parse OPCODE

  }
    /*  if(opcode=="END"){
      operand = " ";
        to_handle_the_LTORG(writing_the_dataSuffix,lineNumberDelta,lineNumber,LOCATION_COUNTER,LAST_DELTA_LOCATION_COUNTER,currentBlockNumber);
    }*/
  //to_handle_the_LTORG(writing_the_dataSuffix,lineNumberDelta,lineNumber,LOCATION_COUNTER,LAST_DELTA_LOCATION_COUNTER,currentBlockNumber);
  read_first_non_white_space(fileLine,index,dheeraj_status_code,operand);
  read_first_non_white_space(fileLine,index,dheeraj_status_code,comment,true);
  //
  currentBlockName = "";
  currentBlockNumber = 0;
  LOCATION_COUNTER = convert_string_hex_to_int(BLOCKS[currentBlockName].LOCATION_COUNTER);

to_handle_the_LTORG(writing_the_dataSuffix,lineNumberDelta,lineNumber,LOCATION_COUNTER,LAST_DELTA_LOCATION_COUNTER,currentBlockNumber);
 BLOCKS[""].LOCATION_COUNTER = convert_int_to_string_hexadecimal(LOCATION_COUNTER);
  writing_the_data = to_string(lineNumber) + "\t" + convert_int_to_string_hexadecimal(LOCATION_COUNTER-LAST_DELTA_LOCATION_COUNTER) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writing_the_dataSuffix;
  write_to_file(intermediateFile,writing_the_data);

  int LOCATION_COUNTERArr[totalBlocks];
  BLocksNumToName = new string[totalBlocks];
  for(auto const& it: BLOCKS){
    LOCATION_COUNTERArr[it.second.number] = convert_string_hex_to_int(it.second.LOCATION_COUNTER);
    BLocksNumToName[it.second.number] = it.first;
  }

  for(int i = 1 ;i<totalBlocks;i++){
    LOCATION_COUNTERArr[i] += LOCATION_COUNTERArr[i-1];
  }

  for(auto const& it: BLOCKS){
    if(it.second.startAddress=="?"){
      BLOCKS[it.first].startAddress= convert_int_to_string_hexadecimal(LOCATION_COUNTERArr[it.second.number - 1]);
    }
  }

  length_of_the_program = LOCATION_COUNTERArr[totalBlocks - 1] - startAddress;
  //to_handle_the_LTORG(writing_the_dataSuffix,lineNumberDelta,lineNumber,LOCATION_COUNTER,LAST_DELTA_LOCATION_COUNTER,currentBlockNumber);
  sourceFile.close();
  intermediateFile.close();
  errorFile.close();
}

