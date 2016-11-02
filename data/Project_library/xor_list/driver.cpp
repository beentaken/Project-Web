#include "xor_list.h"
#include "iostream"

class student
{
    public:
    	friend std::ostream& operator<<(std::ostream& os, const student& rhs);

	    const char* name_;
	    int age_;
	    double GPA_;
};

std::ostream& operator<<(std::ostream& os, const student& rhs)
{
	os << "(Name: " << rhs.name_ << " Age: " << rhs.age_ << " GPA: " << rhs.GPA_ << ")";
	return os;
}

int main (int argc, char** argv)
{
	int a_size = 4;
	xor_list<const char*> xorlist_a;
	const char* a[] = {"ONE", "TWO", "THREE", "FOUR"};

	int b_size = 8;
	xor_list<int> xorlist_b;
	int b[] = {1, 2, 3, 4, 5, 6, 7, 8};

	int c_size = 6;
	xor_list<double> xorlist_c;
	double c[] = {1.11, 2.22, 3.33, 4.44, 5.55, 6.66};

	int d_size = 2;
	xor_list<student> xorlist_d;
	student d[3];
	d[0].name_ = "Damien ->";
	d[0].age_ = 100;
	d[0].GPA_ = 4.0;
	d[1].name_ = "Victoria ->";
	d[1].age_ = 100;
	d[1].GPA_ = 4.0;

	const char* msgs[] = {"Push_Front", "Push_Back", "Pop_Front"};

	if(argc == 1)
	{
		std::cout << "Usage: [EXE NAME] [OPTION]" << std::endl;
		std::cout << "------------------------------------------------" << std::endl;
		std::cout << "  Options:                                      " << std::endl;
		std::cout << "      1 - Const Char* Test                      " << std::endl;
		std::cout << "      2 - Integer Test                          " << std::endl;
		std::cout << "      3 - Double Test                           " << std::endl;
		std::cout << "      4 - Student Class Test                    " << std::endl;
        std::cout << "      5 - Stress Test                           " << std::endl;
		std::cout << "------------------------------------------------" << std::endl;
	}
	else
	{
        if(atoi(argv[1]) == 1 || atoi(argv[1]) == 0)
        {
        	  // ONE
        	  // TWO   -- ONE
        	  // THREE -- TWO   -- ONE 
        	  // FOUR  -- THREE -- TWO -- ONE
        	std::cout << "Push_Front <const char*>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < a_size; i++)
        	{
        	    xorlist_a.push_front(&a[i]);     // Add an element onto the front
        	    xorlist_a.displayList(msgs[0]);  // Display the list
        	}
        	  // FOUR -- THREE -- TWO -- ONE -- ONE
        	  // FOUR -- THREE -- TWO -- ONE -- ONE -- TWO
        	  // FOUR -- THREE -- TWO -- ONE -- ONE -- TWO -- THREE
        	  // FOUR -- THREE -- TWO -- ONE -- ONE -- TWO -- THREE -- FOUR
        	std::cout << "Push_Back <const char*>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < a_size; i++)
        	{
        	    xorlist_a.push_back(&a[i]);      // Add an element onto the back
        	    xorlist_a.displayList(msgs[0]);  // Display the list
        	}
        	  // THREE -- TWO -- ONE -- ONE   -- TWO   -- THREE -- FOUR
        	  // TWO   -- ONE -- ONE -- TWO   -- THREE -- FOUR
        	  // ONE   -- ONE -- TWO -- THREE -- FOUR
        	std::cout << "Pop_Front <const char*>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < a_size; i++)
        	{
        	    xorlist_a.pop_front();          // Delete an element from the front
        	    xorlist_a.displayList(msgs[0]); // Display the list
        	}
        	  // ONE -- ONE -- TWO -- THREE
        	  // ONE -- ONE -- TWO
        	  // ONE -- ONE
        	  // ONE
        	std::cout << "Pop_Back <const char*>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < a_size; i++)
        	{
        	    xorlist_a.pop_back();            // Delete an element from the back
        	    xorlist_a.displayList(msgs[0]);  // Display the list
        	}
        }
        if(atoi(argv[1]) == 2 || atoi(argv[1]) == 0)
        {
        	  // 1
        	  // 2 -- 1
        	  // 3 -- 2 -- 1 
        	  // 4 -- 3 -- 2 -- 1
        	  // 5 -- 4 -- 3 -- 2 -- 1
        	  // 6 -- 5 -- 4 -- 3 -- 2 -- 1
        	  // 7 -- 6 -- 5 -- 4 -- 3 -- 2 -- 1
        	  // 8 -- 7 -- 6 -- 5 -- 4 -- 3 -- 2 -- 1
        	std::cout << "Push_Front <int>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < b_size; i++)
        	{
        	    xorlist_b.push_front(&b[i]);     // Add an element onto the front
        	    xorlist_b.displayList(msgs[0]);  // Display the list
        	}
        	  // 8 -- 7 -- 6 -- 5 -- 4 -- 3 -- 2 -- 1 -- 1
        	  // 8 -- 7 -- 6 -- 5 -- 4 -- 3 -- 2 -- 1 -- 1 -- 2
        	  // 8 -- 7 -- 6 -- 5 -- 4 -- 3 -- 2 -- 1 -- 1 -- 2 -- 3
        	  // 8 -- 7 -- 6 -- 5 -- 4 -- 3 -- 2 -- 1 -- 1 -- 2 -- 3 -- 4
        	  // 8 -- 7 -- 6 -- 5 -- 4 -- 3 -- 2 -- 1 -- 1 -- 2 -- 3 -- 4 --5
        	  // 8 -- 7 -- 6 -- 5 -- 4 -- 3 -- 2 -- 1 -- 1 -- 2 -- 3 -- 4 --5 -- 6
        	  // 8 -- 7 -- 6 -- 5 -- 4 -- 3 -- 2 -- 1 -- 1 -- 2 -- 3 -- 4 --5 -- 6 -- 7
        	  // 8 -- 7 -- 6 -- 5 -- 4 -- 3 -- 2 -- 1 -- 1 -- 2 -- 3 -- 4 --5 -- 6 -- 7 -- 8
        	std::cout << "Push_Back <int>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < b_size; i++)
        	{
        	    xorlist_b.push_back(&b[i]);      // Add an element onto the back
        	    xorlist_b.displayList(msgs[0]);  // Display the list
        	}
        	  // 7 -- 6 -- 5 -- 4 -- 3 -- 2 -- 1 -- 1 -- 2 -- 3 -- 4 -- 5 -- 6 -- 7 -- 8
        	  // 6 -- 5 -- 4 -- 3 -- 2 -- 1 -- 1 -- 2 -- 3 -- 4 -- 5 -- 6 -- 7 -- 8
        	  // 5 -- 4 -- 3 -- 2 -- 1 -- 1 -- 2 -- 3 -- 4 -- 5 -- 6 -- 7 -- 8
        	  // 4 -- 3 -- 2 -- 1 -- 1 -- 2 -- 3 -- 4 -- 5 -- 6 -- 7 -- 8
        	  // 3 -- 2 -- 1 -- 1 -- 2 -- 3 -- 4 -- 5 -- 6 -- 7 -- 8
        	  // 2 -- 1 -- 1 -- 2 -- 3 -- 4 -- 5 -- 6 -- 7 -- 8
        	  // 1 -- 1 -- 2 -- 3 -- 4 -- 5 -- 6 -- 7 -- 8
        	  // 1 -- 2 -- 3 -- 4 -- 5 -- 6 -- 7 -- 8
        	std::cout << "Pop_Front <int>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < b_size; i++)
        	{
        	    xorlist_b.pop_front();          // Delete an element from the front
        	    xorlist_b.displayList(msgs[0]); // Display the list
        	}
        	  // 1 -- 2 -- 3 -- 4 -- 5 -- 6 -- 7
        	  // 1 -- 2 -- 3 -- 4 -- 5 -- 6
        	  // 1 -- 2 -- 3 -- 4 -- 5
        	  // 1 -- 2 -- 3 -- 4
        	  // 1 -- 2 -- 3
        	  // 1 -- 2
        	  // 1
        	  // [ Empty List ]
        	std::cout << "Pop_Back <int>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < b_size; i++)
        	{
        	    xorlist_b.pop_back();            // Delete an element from the back
        	    xorlist_b.displayList(msgs[0]);  // Display the list
        	}
        }
        if(atoi(argv[1]) == 3 || atoi(argv[1]) == 0)
        {
        	  // 1.11
        	  // 2.22 -- 1.11
        	  // 3.33 -- 2.22 -- 1.11 
        	  // 4.44 -- 3.33 -- 2.22 -- 1.11
        	  // 5.55 -- 4.44 -- 3.33 -- 2.22 -- 1.11
        	  // 6.66 -- 5.55 -- 4.44 -- 3.33 -- 2.22 -- 1.11
        	std::cout << "Push_Front <double>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < c_size; i++)
        	{
        	    xorlist_c.push_front(&c[i]);     // Add an element onto the front
        	    xorlist_c.displayList(msgs[0]);  // Display the list
        	}
        	  // 6.66 -- 5.55 -- 4.44 -- 3.33 -- 2.22 -- 1.11 -- 1.11
        	  // 6.66 -- 5.55 -- 4.44 -- 3.33 -- 2.22 -- 1.11 -- 1.11 -- 2.22
        	  // 6.66 -- 5.55 -- 4.44 -- 3.33 -- 2.22 -- 1.11 -- 1.11 -- 2.22 -- 3.33
        	  // 6.66 -- 5.55 -- 4.44 -- 3.33 -- 2.22 -- 1.11 -- 1.11 -- 2.22 -- 3.33 -- 4.44
        	  // 6.66 -- 5.55 -- 4.44 -- 3.33 -- 2.22 -- 1.11 -- 1.11 -- 2.22 -- 3.33 -- 4.44 --5.55
        	  // 6.66 -- 5.55 -- 4.44 -- 3.33 -- 2.22 -- 1.11 -- 1.11 -- 2.22 -- 3.33 -- 4.44 --5.55 -- 6.66
        	std::cout << "Push_Back <double>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < c_size; i++)
        	{
        	    xorlist_c.push_back(&c[i]);      // Add an element onto the back
        	    xorlist_c.displayList(msgs[0]);  // Display the list
        	}
        	  // 5.55 -- 4.44 -- 3.33 -- 2.22 -- 1.11 -- 1.11 -- 2.22 -- 3.33 -- 4.44 -- 5.55 -- 6.66
        	  // 4.44 -- 3.33 -- 2.22 -- 1.11 -- 1.11 -- 2.22 -- 3.33 -- 4.44 -- 5.55 -- 6.66
        	  // 3.33 -- 2.22 -- 1.11 -- 1.11 -- 2.22 -- 3.33 -- 4.44 -- 5.55 -- 6.66
        	  // 2.22 -- 1.11 -- 1.11 -- 2.22 -- 3.33 -- 4.44 -- 5.55 -- 6.66
        	  // 1.11 -- 1.11 -- 2.22 -- 3.33 -- 4.44 -- 5.55 -- 6.66
        	  // 1.11 -- 2.22 -- 3.33 -- 4.44 -- 5.55 -- 6.66
        	std::cout << "Pop_Front <double>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < c_size; i++)
        	{
        	    xorlist_c.pop_front();          // Delete an element from the front
        	    xorlist_c.displayList(msgs[0]); // Display the list
        	}
        	  // 1.11 -- 2.22 -- 3.33 -- 4.44 -- 5.55
        	  // 1.11 -- 2.22 -- 3.33 -- 4.44
        	  // 1.11 -- 2.22 -- 3.33
        	  // 1.11 -- 2.22
        	  // 1.11
        	  // [ Empty List ]
        	std::cout << "Pop_Back <double>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < c_size; i++)
        	{
        	    xorlist_c.pop_back();            // Delete an element from the back
        	    xorlist_c.displayList(msgs[0]);  // Display the list
        	}
        }
        if(atoi(argv[1]) == 4 || atoi(argv[1]) == 0)
        {
              // (Name: Damien -> Age: 100 GPA: 4)
              // (Name: Victoria -> Age: 100 GPA: 1) -- (Name: Damien -> Age: 100 GPA: 4)
        	std::cout << "Push_Front <student class>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < d_size; i++)
        	{
        	    xorlist_d.push_front(&d[i]);     // Add an element onto the front
        	    xorlist_d.displayList(msgs[0]);  // Display the list
        	}
        	  // (Name: Victoria -> Age: 100 GPA: 1) -- (Name: Damien -> Age: 100 GPA: 4) -- (Name: Damien -> Age: 100 GPA: 4)
              // (Name: Victoria -> Age: 100 GPA: 1) -- (Name: Damien -> Age: 100 GPA: 4) -- (Name: Damien -> Age: 100 GPA: 4) -- (Name: Victoria -> Age: 100 GPA: 1)
        	std::cout << "Push_Back <student class>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < d_size; i++)
        	{
        	    xorlist_d.push_back(&d[i]);      // Add an element onto the back
        	    xorlist_d.displayList(msgs[0]);  // Display the list
        	}
        	  // (Name: Damien -> Age: 100 GPA: 4) -- (Name: Damien -> Age: 100 GPA: 4) -- (Name: Victoria -> Age: 100 GPA: 1)
              // (Name: Damien -> Age: 100 GPA: 4) -- (Name: Victoria -> Age: 100 GPA: 1)
        	std::cout << "Pop_Front <student class>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < d_size; i++)
        	{
        	    xorlist_d.pop_front();          // Delete an element from the front
        	    xorlist_d.displayList(msgs[0]); // Display the list
        	}
              // (Name: Damien -> Age: 100 GPA: 4)
              // [ Empty List ]
        	std::cout << "Pop_Back <student class>" << std::endl;
        	std::cout << "==========================================================" << std::endl << std::endl;
        	for(int i = 0; i < d_size; i++)
        	{
        	    xorlist_d.pop_back();            // Delete an element from the back
        	    xorlist_d.displayList(msgs[0]);  // Display the list
        	}
        }
    }

	return 1;
}