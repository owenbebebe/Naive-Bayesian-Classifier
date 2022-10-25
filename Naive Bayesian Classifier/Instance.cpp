#include <string>
#include <vector>
#include <iostream>
#include "attribute.h"
#include "instance.h"

using namespace std;

//Takes a vector of attribute and store it inside an instance
void Instance::Set_Attributes(const vector<Attribute>& c)
{
	Attribute_List = c; 
	for (int i = 0; i < c.size(); i++)
	{ 
		Element_List.push_back("?");
	}
}

//Adds a value to the according attribute you want to check
//Also returns true or false wheater the data has been stored correctly or not
bool Instance::Add_Value(int attribute_num, string val)
{
	if (Is_Valid_Attribute(attribute_num) == false)
		return false; 
	if (Is_Numeric_Attribute(attribute_num) == false)
	{
		//if this is not a numeric value nor legal value print out error
		if (Attribute_List[attribute_num].In_Domain(val) == false)
		{	
			cout << "unmatch value" << endl; 
			return false; 
		}
			
	}
	Element_List[attribute_num] = val; 
	return true; 
}
//Returns the string of the value of the attribute
string Instance::Get_Nominal_Value(int attribute_num)
{
	//if it is not a valid attribute number
	if (Is_Valid_Attribute(attribute_num) == false)
	{
		return "?";
	}
	if (Is_Numeric_Attribute(attribute_num) == true)
	{
		cout << "Not a nominal value" << endl; 
		return "?";
	}
		
	return Element_List[attribute_num];
}

//Returns the value of the numeric value
double Instance::Get_Numeric_Value(int attribute_num) 
{
	//return -1 for all illegal input
	if (Is_Valid_Attribute(attribute_num) == false)
	{
		return -1; 
	}
	if (Attribute_List[attribute_num].Numeric() == false)
	{
		cout << "Not a Numeric value" << endl; 
		return -1; 
	}
	//calliing stod to convert string to double
	return stod(Element_List[attribute_num]); 
}

//Checks the attribute at the given number Numeric function to see if the Attribute is NUmeric
bool Instance::Is_Numeric_Attribute(int attribute_num)
{
	if (Is_Valid_Attribute(attribute_num) == true)
	{
		if (Attribute_List[attribute_num].Numeric() == true)
		return true; 
	}
	return false; 
}

//Returns the name of the attribute by callign Get_Name() function 
string Instance::Get_Attribute_Name(int attribute_num) 
{
	if (Is_Valid_Attribute(attribute_num) == true)
	{
		return Attribute_List[attribute_num].Get_Name(); 
	}
	cout << "Cant find the attribute name" << endl; 
	return "?"; 
}

//Checks if the attribute number is a legal attribute
bool Instance::Is_Valid_Attribute(int attribute_num)
{
	if (attribute_num < Attribute_List.size() && attribute_num >= 0)
		return true; 
	else {
		cout << "No attribute number found";
		return false; 
	}
}
