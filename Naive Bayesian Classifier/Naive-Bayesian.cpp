#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "attribute.h"
#include "instance.h"
using namespace std;

//New Attrubute takes the indivdual string line on the file and read in the attribute lists
Attribute New_Attribute(string s)
{
	Attribute att;
	string ss = s.substr(11, s.size());
	//if this is a numeric attribute 
	if (ss.find("numeric") != -1)
	{
		att.Set_Name(ss.substr(0, ss.find("numeric")));
		att.Set_Numeric(true);
	}
	//This is a nominal attribute
	int find = ss.find("{");
	//if there exist an integer that finds '{'
	if (find != -1)
	{
		att.Set_Name(ss.substr(0, find));
		att.Set_Numeric(false);
		int comma = 1; // comma is the place where the string finds a comma
		//while comma found
		while (comma != -1)
		{
			//find +2 to eliminate the first comma we find and the space after that
			find = find + 2;
			ss = ss.substr(find, ss.size());
			comma = ss.find(",");
			//if comma not found means we reach the last items in the list
			if (comma == -1)
			{
				//comma -2 to eliminate '}' and the space before it
				comma = ss.size() - 2;
				att.Add_Category(ss.substr(0, comma));
				//comma = -1 to get out of the loop
				comma = -1;
			}
			else //else if still can find a comma Add the item inside the list
			{
				att.Add_Category(ss.substr(0, comma));
				find = comma;
			}

		}
	}
	return att;
}

//Parse_Data take the Attribute List and the string of line we are reading and input the data
Instance Parse_Data(string s, vector<Attribute> Attribute_List)
{
	//First create an Instance of string s and read in the attribute list
	Instance Instances;
	Instances.Set_Attributes(Attribute_List);
	//if we find a comma string ss is the first instance item we are lookin for
	int comma = s.find(",");
	string ss = s.substr(0, comma);
	//while we can find a comma put the individual item before a comma inside the element list
	while (comma != -1)
	{
		//for every attribute slots 
		for (int i = 0; i < Attribute_List.size(); i++)
		{
			//check if the value is legal or not
			if (Instances.Add_Value(i, s.substr(0, comma)) == false)
				cout << "ADDING VALUE, UNSUCCESSFUL" << endl;
			//comma +2 to elminate the space and the comma before that
			comma = comma + 2;
			//resize the string without the previous element
			s = s.substr(comma, s.size());
			comma = s.find(",");
		}
	}
	return Instances;
}

//Print_Data print out all of the attribute names to the user and set the clasification attribute
int Print_Data(vector<Instance> Instance_List, const vector<Attribute> Attribute_List)
{
	for (int i = 0; i < Attribute_List.size(); i++)
		cout << "Type " << i << " to set " << Attribute_List[i].Get_Name() << "as the classification attribute." << endl;
	int which_attribute;
	cin >> which_attribute;
	return which_attribute;
}
//This Counts the attribute number in every instance
//This takes O(N^4) times
vector<vector<int>> Make_Count_Table(const vector<Attribute>& attribute_list,
	vector<Instance>& example,
	int cur_attribute,
	int check_attribute,
	vector<vector<int>>& Table)
{
	int c = attribute_list[check_attribute].Num_Categories();
	int r = attribute_list[cur_attribute].Num_Categories();
	Table.resize(r, vector<int>(c));
	//Create a cur_attribute list number x check_attribute list number 2d table
	//and fill the table with 0s 
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			Table[i][j] = 0;
		}
	}
	//if matches the attribute and the value we are checking in the
	//check_attribute list that table + 1 
	//(for weather-nominal check yes and no ) 
	//(for nursery check not_recom, recommend, very_recom, priority, spec_prior)
	//This takes triple loop 
	vector<string> Category_List = attribute_list[cur_attribute].Get_Category_List();
	vector<string> Check_List = attribute_list[check_attribute].Get_Category_List();
	for (int i = 0; i < example.size(); i++)
	{
		string test = example[i].Get_Nominal_Value(cur_attribute);
		for (int j = 0; j < r; j++)
		{
			if (test == Category_List[j])
			{
				for (int k = 0; k < c; k++)
				{
					if (example[i].Get_Nominal_Value(check_attribute) == Check_List[k])
						Table[j][k]++;
				}
			}
		}
	}
	return Table; 
}

//Print out the Count Table we just created
//THis takes O(N^2) because this is a 2d vector
void Print_Count_Table(vector<vector<vector<int>>> Table)
{
	for (int i = 0; i < Table.size(); i++)
	{
		//secondsize = the size of the inner vector. 
		int secondsize = Table[i].size();
		//Value is 0 bc every second vector is the same size and has atleast one row
		for (int j = 0; j < secondsize; j++)
		{
			int third = Table[i][j].size();
			for (int k = 0; k < third; k++)
			{
				cout << Table[i][j][k] << " ";
			}
			
		}
		cout << endl;
	}
}
vector<int> Make_Outcome_Table(Attribute att, vector<Instance> example, int check_num)
{
	//resize the table and put 0 inside as a default value
	vector<int> Table; 
	vector<string> List = att.Get_Category_List();
	Table.resize(List.size()); 
	for (int i = 0; i < Table.size(); i++)
	{
		Table[i] = 0; 
	}
	for (int i = 0; i < example.size(); i++)
	{
		for (int j = 0; j < List.size(); j++)
		{
			if (example[i].Get_Nominal_Value(check_num) == List[j])
			{
				Table[j]++; 
			} 
		}
	}
	return Table; 
}

double Calc_Observed_Probability(double numerator, double denominator)
{
	
	return numerator/denominator; 
} 

int main()
{
	//open the file
	string filename;
	cout << "Enter the filename: ";
	cin >> filename;
	ifstream fin;
	fin.open(filename.data());
	while (!fin) {
		cout << "File not found" << endl;
		cout << "Enter the filename: ";
		cin >> filename;
		fin.clear();
		fin.open(filename.data());
	}
	//once we get here the file is open
	string s;
	vector<Attribute> Attribute_List;
	vector<Instance> Instance_List;
	//data_mode shows us wheather we are done with readin attributes or not
	bool data_mode = false;
	while (getline(fin, s))
	{
		if (s.size() > 0 && s[0] != '%')
		{
			//once data_mode is on we know we can start reading instance
			//ignore line if empty or comment
			if (data_mode == true) {
				if (s[0] == '%')
					cout << "Error, we are getting a comment instead of data" << endl;
				else
					//call parse data to readin instances
					Instance_List.push_back(Parse_Data(s, Attribute_List));

			}
			else
			{
				//once we found @data means we can reach the instance section
				if (s == "@data")
					data_mode = true;
				//if we found @attribute call New attribute to read in attributes 
				else if (s.substr(0, 10) == "@attribute")
					Attribute_List.push_back(New_Attribute(s));
				else
					cout << "Error we can not recognize this command" << endl;
			}
		}
	}
	//Asking and set the classification attribute first
	int check_number = Print_Data(Instance_List, Attribute_List);
	//create a temp table
	vector<vector<int>> Table;
	vector<vector<vector<int>>> Big_Count_Table; 
	for (int i = 0; i < Attribute_List.size(); i++)
	{
		Big_Count_Table.push_back(Make_Count_Table(Attribute_List, Instance_List, i, check_number, Table)); 
	}
	vector<int> Outcome_Table = 
	Make_Outcome_Table(Attribute_List[check_number], Instance_List, check_number);
	vector<double> Calc_List; 
	//Read in the test data
	//open the file
	cout << "Enter the filename: ";
	cin >> filename;
	ifstream fin2;
	fin2.open(filename.data());
	while (!fin2) {
		cout << "File not found" << endl;
		cout << "Enter the filename: ";
		cin >> filename;
		fin2.clear();
		fin2.open(filename.data());
	}
	//once we get here the file is open
	vector<Instance> Tinstance_List;
	//data_mode shows us wheather we are done with readin attributes or not
	while (getline(fin2, s))
	{
		if (s.size() > 0 && s[0] != '%')
		{
			//once data_mode is on we know we can start reading instance
			//ignore line if empty or comment
					Tinstance_List.push_back(Parse_Data(s, Attribute_List));
		}
	}
	//calculate the sum of the classification attribute
	int outcome_sum = 0; 
	for (int z = 0; z < Outcome_Table.size(); z++)
	{
		outcome_sum = outcome_sum + Outcome_Table[z];
	}
	vector<string> Check_List = Attribute_List[check_number].Get_Category_List(); 
	double high_numerator = 0.; // keeps the counts for the higher probability outcome 
	double high_denominator = 0.;
	double points_score = 0; // keeps the score for the correct points attempt
	//Tinstance_List now holds the instances values for the data set
	//This loop reads in the test instances vector and match it with our attribute list
	//We then use the data we have in the Big_Count_Table and calculate the proabability
	//This loop is time consuming take O(N^8) times
	for (int i = 0; i < Tinstance_List.size(); i++)
	{
		//create a vector to store the likelihood
		vector<double> Prob;
		double rate = 1.0;
		for (int z = 0; z < Outcome_Table.size(); z++)
		{
			int count = Attribute_List.size() - 1;
			for (int j = 0; j < count; j++)
			{
				vector<string> Match = Attribute_List[j].Get_Category_List();
				for (int k = 0; k < Match.size(); k++)
				{
					if (Tinstance_List[i].Get_Nominal_Value(j) == Match[k])
					{
						//apply the Laplace estimator 
						// for every numerator +1 and denomerator + the size of the outcome table 
						rate = rate * Calc_Observed_Probability(Big_Count_Table[j][k][z] + 1, Outcome_Table[z] + count);
					}
				}
			}
			rate = rate * Outcome_Table[z] / outcome_sum;
			Prob.push_back(rate);
			rate = 1.0;
		}
		//Once we reached here we have calculated the likelihood
		//now we have to calculate the probability of the outcome
			//prob_outcome_sum gives us the denominator of the probability of outcome
		double prob_outcome_sum = 0.;
		for (int z = 0; z < Prob.size(); z++)
		{
			prob_outcome_sum = prob_outcome_sum + Prob[z];
		}
		//We now divide the specific case we want to check and devide it by the prob_outcome_sum and store the result in a list
		vector<double> prob_outcome_list;
		for (int z = 0; z < Prob.size(); z++)
		{
			double prob_outcome = (Prob[z] / prob_outcome_sum);
			prob_outcome_list.push_back(prob_outcome);
		}
		//**********Higher probability attempt**********
		//best_pos holds the highest probability outcome
		int best_pos = 0;
		for (int z = 0; z < prob_outcome_list.size() -1; z++)
		{
			if (prob_outcome_list[z] < prob_outcome_list[z + 1])
				best_pos = z + 1;
		}
		//Check for the instance answer and see if correct or not
		//and keep the ratio of corrrectness. 
		if (Tinstance_List[i].Get_Nominal_Value(check_number) == Check_List[best_pos])
			high_numerator++;
		high_denominator++;

		//************Accumulate points attempt**********
		for (int z = 0; z < Check_List.size(); z++)
		{
			if (Tinstance_List[i].Get_Nominal_Value(check_number) == Check_List[z])
				points_score = points_score + prob_outcome_list[z]; 
		}

	}
	//End of reading all test instances
	cout << "The error rate for the higher probability outcome atttempt of this test set is " << high_numerator / high_denominator * 100<<"% "<<endl; 
	cout << "The accumalte points of correctness of this test set is " << points_score << endl; 
}
