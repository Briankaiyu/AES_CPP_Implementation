/*
Name: Brian Yu
Date: 12/2/15

This program encrypts a file(read as a single string) using a key from a second file. The result
is stored in a third file. All three files are designated by user.
*/


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdio.h>
using namespace std;
//prints the string to screen and file in block format
string printBlocks(string input);
//prepares string by removing punctuation and spaces
string preprocessing(string input);
//performs basic polyalphabetic substitution using Vigenere cipher on string
string substitution(string input, string key);
//padds string to form 4x4 blocks using A's
string padding(string input);
//Circularly shifts rows in each block. First stays same, second moves one
//left, third moves two left, fourth moves three left
string shiftRows(string input);
//converts each character to numeric value, using MSB as parity bit
void parityBit(string input, unsigned char result[]);
//performs bit multiplication using 2 or 3 (num)
unsigned char rgfMul(unsigned char x, int num);//takes either 2 or 3
//using rgfMul for assisstance, puts string through Rijndael's Galois
//field
void mixColumns(unsigned char text[],int size);

//string printHex(string input);
int main()
{
	//integer to keep track of when to print newlines
	int newline;
	ofstream output;//points to output file
	ifstream input;//points to input file
	ifstream keyfile;//points to file containing key
	string filename;//temporary value to hold filenames
	string text;//holds text read from input file
	string key;//holds text read from key file
	string buffer;//used to read in text from file line by line
	string printed;//holds text in a form acceptable for printing
	
	//Ask for names and open files
	cout<<"Enter the name of the input plaintext file:";
	cin>>filename;
	input.open(filename.c_str());
	//check if file opened	
	if(!input.is_open())
	{
		cout<<"Error: Could not open file."<<endl;
		return 1;
	}

	cout<<"Enter the name of the input key file:";
	cin>>filename;
	keyfile.open(filename.c_str());
	//check if file open
	if(!keyfile.is_open())
	{
		cout<<"Error: Could not open file." <<endl;
		return 1;
	}
	
	cout<<"Enter the name of the output ciphertext file:";
	cin>>filename;
	output.open(filename.c_str());

	//load input into text
	while(getline(input,buffer)){
		text.append(buffer);
	}
	
	//load key 
	getline(keyfile,key);

	//Preprocessing step:
	cout<<"Preprocessing:"<<endl;
	text=preprocessing(text);
	cout<<text<<endl;
	output<<"Preprocessing:"<<endl;
	output<<text<<endl;

	//Substitution step:
	cout<<"Substitution:"<<endl;
	text=substitution(text,key);
	cout<<text<<endl;
	output<<"\nSubstitution:"<<endl;
	output<<text<<endl;

	//Padding step:
	cout<<"Padding:"<<endl;
	text=padding(text);
	//print fancy
	output<<"\nPadding:"<<endl;
	printed=printBlocks(text);
	cout<<printed;
	output<<printed;

	//Shift Rows step
	cout<<"Shift Rows:"<<endl;
	text=shiftRows(text);
	//print fancy
	output<<"\nShift Rows:"<<endl;
	printed=printBlocks(text);
	cout<<printed;
	output<<printed;

	//unfortunately past this point I could no longer 
	//use the print fancy function, as I had trouble saving string
	//in required format

	//Parity Bit step
	//used array for convenience of having each character isolated
	unsigned char newtext[text.length()];
	cout<<"Parity Bit:"<<endl;
	parityBit(text,newtext);
	output<<"\nParity Bit:"<<endl;

	//print string and save to output file in valid format
	newline=0;
	for(int i=0;i<text.length();i++)
	{
		printf("%x ",newtext[i]);
		output<<hex<<(unsigned int)newtext[i]<<" ";
		if(newline==3)
		{
			cout<<endl;
			output<<endl;
			newline=-1;
		}
		newline++;
	}
	cout<<endl;
	output<<endl;	

	//Mix Columns step
	mixColumns(newtext,text.length());
	newline=0;
	cout<<"Mix Columns:"<<endl;
	output<<"\nMix Columns:"<<endl;
	
	//print in acceptable format to both screen and output file
	for(int i=0;i<text.length();i++)
	{
		printf("%x ",newtext[i]);
		output<<hex<<(unsigned int)newtext[i]<<" ";
		if(newline==3)
		{
			cout<<endl;
			output<<endl;
			newline=-1;
		}
		newline++;
	
	}
	//close file pointers
	output.close();
	input.close();
	keyfile.close();

	return 0;

}

//prints text in block format
string printBlocks(string text)
{
	string output;//used to hold output for return
	int newline=0;//used to track when to print new lines
	//go through text
	for(int x = 0; x<text.length();x+=4)
        {	
		//for every four values, print
                for(int y=0; y<4; y++)
                {
                        output+=text[x+y];
                }
		//insert new line
                output+='\n';
		//double on new line if block is finished
                if(newline==3)
                {
                        output+='\n';
			newline=-1;
                }
		//reset new line
		newline++;
        }
	return output;

}
//prepares string by removing punctuation and spaces
string preprocessing(string input)
{
	string temp;//holds string to return
	//for each character in input
	for(char &c : input)
	{
		//if c is a valid letter
		if((c>='A')&&(c<='Z'))
		{
			//keep c
			temp+=c;
		}
	}
	return temp;
}
//performs polyalphabetic substituion from vigenere cipher
string substitution(string input, string key)
{
	string output;//holds string to return
	char a,b;//where a is a character from input, b is a character from key
	int key_count=0;//keeps track of which position in key is being examined
	//for the string
	for(int i=0; i<input.length(); i++)
	{
		//store the letter
		a=input[i];
		//perform operations on a and save to output
		output+=(a+key[key_count] -2 * 'A') % 26 + 'A';
		//increment by one, unless end in which case it resets to 0
		key_count=(key_count+1) % key.length();
	}
	return output;
}
//pads string by inserting A's until string can form perfect 4x4 blocks
string padding(string input)
{
	//check if we need to pad
	if(input.length()%16==0)
	{	//string already makes up 4 x 4 block
		return input;
	}
	else//must padd string with 'A'
	{
		int counter=0;//counter to keep track of how many is being set in block
		while(counter<input.length())
		{
			counter+=16; //find next amount that would make 4 x 4 block
 		}	
		counter=counter-input.length();//find number of 'A's to add
		//add those A's
		for(int i=0;i<counter;i++)
		{
			input+='A';
		}
		return input;
	}
}
//shifts rows in each block. First by none, second left one, third left two, fourth
//left three
string shiftRows(string input)
{
	char temp[4];//save values to remember which values to place in correct spot
	//operate on 16 size blocks at a time
	for(int i=0; i<input.length();i=i+16)
	{
		//I is first letter, i+15 is last letter
		//first row doesn't change
		//start with second row
		for(int x=0; x<4;x++)
			temp[x]=input[x+i+4];
		input[i+4]=temp[1];
		input[i+5]=temp[2];
		input[i+6]=temp[3];
		input[i+7]=temp[0];

		//switch up third row		
		for(int x=0; x<4; x++)
			temp[x]=input[x+i+8];
		input[i+8]=temp[2];
		input[i+9]=temp[3];
		input[i+10]=temp[0];
		input[i+11]=temp[1];

		//switch up fourth row
		for(int x=0; x<4;x++)
			temp[x]=input[x+i+12];
		input[i+12]=temp[3];
		input[i+13]=temp[0];
		input[i+14]=temp[1];
		input[i+15]=temp[2];
	}
	return input;
}
//convert each character to number and use MSB as parity bit
void parityBit(string input,unsigned char newtext[])
{
	unsigned int count;//tracks how many one's in a character
	int i=0;//tracks where to inset characters into new array
        //string output;
        for(char &c : input)
        {
                //reset count
		count =0;
		unsigned int temp=c;//temp value to perform operations on c
		while(temp)
		{
			//if current value is 0, count does not increase
			//if current value is 1, count increases
			count+=temp&1;
			temp>>=1;
		}
		if(count%2!=0)//if number of 1s is odd
		{
			//change MSB to 1
			c+= 1<<((sizeof(unsigned char)*8)-1);
			newtext[i]=c;
		}//otherwise leave alone
		else
		{
			newtext[i]=c;
		}
		//increment i
		i++;
        }
    
	
}
//performs bit multiplication
unsigned char rgfMul(unsigned char x, int num)
{	
	int MSB;//holds value of MSB
	unsigned char temp;//temporary storage
	unsigned char result= x<<1;//going to hae to multiply by 2 anyways, so do so
	//additional step to multiply by 3
	if(num==3)
	{
		result=result^x;
	}
	//either way, se temp value to hold x
	temp=x;
	//shunt bits from temp value until end up with MSB
	for(int i=(sizeof(unsigned char) *8); i>0; i--)
	{
		MSB=(temp&1);
		temp>>=1;
	}
	//if MSB of ORIGINAL value is 1, must XOR
	if(MSB==1)
	{
		result=result^27;
	}
	return result;
}

//Diffuses data in Rijndael's Galois field
void mixColumns(unsigned char text[],int size)
{

	unsigned char temp[4];//temp storage to save the values needed to insert
	//loop should go four at a time
	for(int i=0;i<size;i=i+16)
{

		//for each column
		for(int x=0; x<4; x++)
		{
			//save values
			temp[0]=text[x+i];
			temp[1]=text[x+i+4];
			temp[2]=text[x+i+8];
			temp[3]=text[x+i+12];
			//SET VALUE 1
			text[x+i]=rgfMul(temp[0],2)^rgfMul(temp[1],3)^temp[2]^temp[3];
		
			//SET VALUE 2
			text[x+i+4]=temp[0]^rgfMul(temp[1],2)^rgfMul(temp[2],3)^temp[3];
			//SET VALUE 3
			text[x+i+8]=temp[0]^temp[1]^rgfMul(temp[2],2)^rgfMul(temp[3],3);
			//SET VALUE 4
			text[x+i+12]=rgfMul(temp[0],3)^temp[1]^temp[2]^rgfMul(temp[3],2);
		}
	}
}
