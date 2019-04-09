/*
SETT SARVEROTT 2018
Math expression calculator
MIT license
*/


#define TYPE_ADD 0 // +
#define TYPE_SUB 1 // -
#define TYPE_MUL 2 // *
#define TYPE_DIV 3 // /

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct operationQueue //operation queue element
{
    operationQueue *next=NULL;
    int operationtype=0; //default +
    double value=0;
};
double operationAct(double first, double second, int type)
{
    double result=0;
    switch(type)
    {
    case TYPE_ADD:
        result=first+second;
        break;
    case TYPE_SUB:
        result=first-second;
        break;
    case TYPE_MUL:
        result=first*second;
        break;
    case TYPE_DIV:
        result=first/second;
        break;
    default:
        throw invalid_argument("unknown type in operation selection [function operation<number>]");
    }
    return result;
}
string evaluate(string input)
{
    double result=0;
    string subexpressionContent=""; //content inside brackets
    int expressionlevel=0; //bracket pair level
    bool subexpressionWriting=false; //is inside brackets?
    string current_num="";
    operationQueue *first=NULL; //first in queue
    operationQueue *last=NULL; //last in queue
    first=last=new operationQueue;  //assign first element
    for(int i=0; i<input.length(); i++)
    {
        if((input[i]>='0'&&input[i]<='9')||input[i]=='+'||input[i]=='-'||input[i]=='*'||input[i]=='/')
        {
            if(subexpressionWriting)
            {
                subexpressionContent+=input[i];
            }
            else
            {
                if(input[i]>='0'&&input[i]<='9') //if char is a cipher
                {
                    current_num+=input[i]; //add char to string
                }
                else
                {
                    if(current_num=="")
                    {
                        throw invalid_argument("misstake in expression!");
                    }
                    last->value=stod(current_num); //convert string to double and assign as value property of object assigned to "last" pointer
                    last->next=new operationQueue; //create new operation queue object and assign it to "next" property
                    last=last->next; //move pointer "last" to new object
                    if(input[i]=='+') last->operationtype=TYPE_ADD;
                    else if(input[i]=='-') last->operationtype=TYPE_SUB;
                    else if(input[i]=='*') last->operationtype=TYPE_MUL;
                    else if(input[i]=='/') last->operationtype=TYPE_DIV;
                    current_num="";
                }
            }
        }
        else if(input[i]=='(')
        {
            if(subexpressionWriting)
            {
                expressionlevel++; //increase bracket level
                subexpressionContent+=input[i];
            }
            else
            {
                subexpressionWriting=true;
            }
        }
        else if(input[i]==')')
        {
            if(subexpressionWriting)
            {
                if(expressionlevel>0) //is it not floor level?
                {
                    expressionlevel--; //decrease bracket level
                    subexpressionContent+=input[i];
                }
                else
                {
                    subexpressionWriting=false;
                    current_num=evaluate(subexpressionContent); //eval content in brackets
                    subexpressionContent="";
                }
            }
            else
            {
                throw invalid_argument("unexpected \")\" in expression");
            }
        }
    }
    if(current_num=="")
    {
        throw invalid_argument("misstake in expression!");
    }
    last->value=stod(current_num);
    last=first;
    while(last->next!=NULL) //multiply and divide
    {
        if(last->next->operationtype>1)
        {
            last->value=operationAct(last->value, last->next->value, last->next->operationtype);
            last->next=last->next->next; //change object assigned to "next" property to its "next" object in queue
        }
        else
        {
            last=last->next;
        }
    }
    while(first!=NULL) // add and substract
    {
        result=operationAct(result, first->value, first->operationtype);//
        first=first->next;
    }
    ostringstream resultStream; //double
    resultStream << result; //to
    string output = resultStream.str(); //string
    return output;
}
int main(int argc, char **argv)
{
    string expression;
    if(argc==1){ //if there is no programm arguments
        cout<<"Insert your math expression:\n";
        cin>>expression;
        cout<<"\n\nANSWER:\n";
        cout<<evaluate(expression);
        cout<<"\n!bye!\n";
    }else{
        for(int i=1;i<argc;i++){
            expression=argv[i]; //for each argument
            cout<<evaluate(expression)<<"\n"; //make evaluation
        }
    }
    return 0;
}
