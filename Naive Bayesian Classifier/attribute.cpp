#include <iostream>
#include <string>
#include <vector>
#include "weatherinstance.h"
#include "attribute.h"

using namespace std; 

//Set Name of the Attribute
void Attribute::Set_Name(const string& s)
{
	name = s;

}
//Get the Name of the Attribute
string Attribute::Get_Name() const
{
	return name; 
}
//Decide wheather the attribute is numeric or not
void Attribute::Set_Numeric(bool b)
{
	if (b == true)
		is_numeric = true;
	else
		is_numeric = false;
}
//Return wheather the attribute is numeric or not
bool Attribute::Numeric() const 
{
	if (is_numeric == true)
		return true;
	else return false;
}
//Add a category to the attribute
void Attribute::Add_Category(string s)
{
	categories.push_back(s);
}
//Return how many categories the attribute has
int Attribute::Num_Categories() const
{
	return categories.size();
}
//Check if the category "s" is in the lists of categories
bool Attribute::In_Domain(string s)
{  
	bool testing = false; 
	int i = 0;
	while(i < categories.size() && testing == false)
	{
		if (categories[i] == s)
		{
			testing = true; 
			return testing; 
		}
		i++; 
			
	}
	return testing; 
}
//Return back the category list
vector<string> Attribute::Get_Category_List() const
{
	vector<string> sub; 
	for (int i = 0; i < categories.size(); i++)
	{
		sub.push_back(categories[i]); 
	}
	return sub; 
}