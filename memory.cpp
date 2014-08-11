#include <iostream>
#include <stdlib.h>
#define MemSize 64

using namespace std;

struct prog//the program going to load to memory
{
	void * p;
	int mem_required;
};

struct Hole//status of hole
{
	int begin;
	int end;
	int status;// 0-free 1-occupied
	struct Hole * previous;
	struct Hole * next;
};

class MemoryManagement
{
	private:
		char mem[MemSize];
		//prog programs[MemSize];
		int is_init;
		Hole * first;
	public:
		MemoryManagement()//done.default constructor
		{
			first = new Hole;
			first->begin = 0;
			first->end = MemSize - 1;
			first->status = 0;
			first->previous = NULL;
			first->next = NULL;
			is_init = 1;
			//int i;
			/*for(i = 0;i < MemSize;i ++)
			{
				programs[i].p = 0;
				programs[i].mem_required = 0;
			}*/
		}
		void* getHole_first(int size)//done.first-fit
		{
			//cout<<"gethole begin"<<endl;
			Hole * temp;
			Hole * temp1;

			temp = first;
			while(temp != NULL)
			{
				if((temp->status == 0) && ((temp->end - temp->begin + 1) > size))
				{
					//find a bigger hole and split it
					Hole * newHole = new Hole;
					newHole->previous = temp;
					newHole->next = temp->next;
					temp1 = temp->next;

					if(temp1 != NULL)
					{
						temp1->previous = newHole;
					}

					temp->next = newHole;
					newHole->end = temp->end;
					temp->end = (temp->begin + size - 1);
					temp->status = 1;
					newHole->begin = (temp->end + 1);
					newHole->status = 0;

					return &mem[temp->begin];
				}
				else if((temp->status == 0) && ((temp->end - temp->begin + 1) == size))
				{
					//use temp directly
					temp->status = 1;
					return &mem[temp->begin];
				}
				else
				{
					//no proper hole found
					temp = temp->next;
				}
				//cout<<"over"<<endl;
			}
			cout<<"wocao"<<endl;
		}
		void* getHole_best(int size)
		{
			Hole * temp;
			Hole * temp1;
			Hole * best;
			int min = MemSize + 1;
			int flag = 0;

			temp = first;
			while(temp != NULL)
			{
				//printf( "temp.status=%d | tmp.end = %d| tmp.begin=%d | size=%d\n", temp->status, temp->end, temp->begin, size );
				if((temp->status == 0) && ((temp->end - temp->begin + 1) == size))
				{
					//use temp directly
					temp->status = 1;
					return &mem[temp->begin];
				}
				else if((temp->status == 0) && ((temp->end - temp->begin + 1) > size))
				{
					//scan one time and find the best hole
					if((temp->end - temp->begin + 1) < min)
					{
						cout<<"min is "<<min<<endl;	
						flag = 1;
						min = temp->end - temp->begin + 1;
						best = temp;
						cout<<"min is "<<min<<endl;
						//temp = temp->next;
					}
					temp = temp->next;
				}	
				else
				{
					if(temp->next == NULL)
						break;
					temp = temp->next;
				}
			}
			if(flag == 1)
			{
				Hole * newHole = new Hole;
				newHole->previous = best;
				newHole->next = best->next;
				temp1 = best->next;
				if(temp1 != NULL)
				{
					temp1->previous = newHole;
				}
				best->next = newHole;
				newHole->end = best->end;
				best->end = (best->begin + size -1);
				best->status = 1;
				newHole->begin = best->end + 1;
				newHole->status = 0;
				return &mem[best->begin];
			}
		}
		void* getHole_worst(int size)
		{
			Hole * temp;
			Hole * temp1;
			Hole * worst;
			int max = 0;
			int flag = 0;

			temp = first;
			while(temp != NULL)
			{
				//printf( "temp.status=%d | tmp.end = %d| tmp.begin=%d | size=%d\n", temp->status, temp->end, temp->begin, size );
				/*if((temp->status == 0) && ((temp->end - temp->begin + 1) == size))
				{
					//use temp directly
					temp->status = 1;
					return &mem[temp->begin];
				}*/
				if((temp->status == 0) && ((temp->end - temp->begin + 1) >= size))
				{
					//scan one time and find the best hole
					if((temp->end - temp->begin + 1) > max)
					{
						//cout<<"max is "<<min<<endl;	
						flag = 1;
						max = temp->end - temp->begin + 1;
						worst = temp;
						//cout<<"max is "<<min<<endl;
						//temp = temp->next;
					}
					temp = temp->next;
				}	
				else
				{
					if(temp->next == NULL)
						break;
					temp = temp->next;
				}
			}
			if(flag == 1)
			{
				Hole * newHole = new Hole;
				newHole->previous = worst;
				newHole->next = worst->next;
				temp1 = worst->next;
				if(temp1 != NULL)
				{
					temp1->previous = newHole;
				}
				worst->next = newHole;
				newHole->end = worst->end;
				worst->end = (worst->begin + size -1);
				worst->status = 1;
				newHole->begin = worst->end + 1;
				newHole->status = 0;
				return &mem[worst->begin];
			}
		}
		int FreeHole( void * p )//0-succeed 1-fail
		{
    	Hole * temp = first;
    	Hole * temp2; 
    	Hole * temp3; 
    	while ( temp != NULL )
    	{
      	if ( (temp->status == 1) && ( &mem[temp->begin] == p) )
      	{
        	temp2 = temp->previous; 
        	//if the hole before is free then merge them
        	if ( (temp2 != NULL) && (temp2->status == 0) )
        	{   
          	temp2->next = temp->next;
          	temp3 = temp->next;
          	temp3->previous = temp2;
          	temp2->end = temp->end;
          	free( temp );      
          	temp = temp2;         
        	}
          temp3 = temp->next;    
          //if the hole after is free then merge them
        	if ( (temp3 != NULL) && (temp3->status == 0) )
        	{
          	temp->next = temp3->next;
          	temp->end = temp3->end;
          	free( temp3 );    
        	}
        
        	temp->status = 0;
        	return 1;
      	}
      	else
        	temp = temp->next;
    	}
    	return 0;
		}	
		bool isFree(int i)//done.true-free false-not free
		{
			Hole * temp;
			temp = first;
			while(temp != NULL)
			{
				if((temp->status == 1) && ( i >= temp->begin ) && ( i <= temp->end ))
				{
					return false;
				}
				else
				{
					temp = temp->next;
				}
			}
			return true;
		}
		int load(prog * ptr,int choice)//done.load program to memory
		{
			int size = 0;
			while(size < 1 || size > MemSize)
			{
				cout<<"How much memory do the program need ? [1-"<<MemSize<<']'<<endl;
				cin>>size;
			}
			while(ptr->p != NULL)
			{
				ptr++;
			}
			ptr->mem_required = size;
			if(choice == 1)
			{
				ptr->p = getHole_first(size);
			}
			else if (choice == 2)
			{
				ptr->p = getHole_best(size);
			}
			else
			{
				ptr->p = getHole_worst(size);
			}
			return (ptr->p != NULL);
		}
		int del(prog * ptr)//delete program from memory
		{
			int i;
			cout<<"Which do you want to delete?"<<endl;
			for(i = 0;i < MemSize;i ++)
			{
				if(ptr[i].p != NULL)
				{
					cout<<i<<'-'<<ptr[i].mem_required<<' ';
				}
			}
			cin>>i;
			if(FreeHole(ptr[i].p))
			{
				//cout<<"delete hole "<<i<<"ok"<<endl;
				ptr[i].p = NULL;
				return 1;
			}
			else
			{
				//ßcout<<"delete hole "<<i<<" not ok"<<endl;
				return 0;
			}
		}
		void PrintMem()//done
		{
			int i;
			cout<<'|';
			for(i = 0;i < MemSize;i ++)
			{
				cout<<'-';
			}
			cout<<"|\n|";
			for(i = 0;i < MemSize;i ++)
			{
				if(isFree(i))
					cout<<' ';
				else
					cout<<'*';
			}
			cout<<"|\n|";
			for(i = 0;i < MemSize;i ++)
			{
				cout<<'-';
			}
			cout<<'|'<<endl;
		}
		void console()
		{
			int i;
			prog programs[MemSize];
			for(i = 0;i < MemSize;i ++)
			{
				programs[i].p = NULL;
				programs[i].mem_required = 0;
			}
			char order = 'x';
			int choice = 0 ;
			cout<<"Choose : 1-first_fit 2-best_fit 3-worst_fit"<<endl;
			cin>>choice;
			//cout<<"Input: l-load programs d-delete programs"<<endl;
			//cout<<"End with EOF"<<endl;
			//order = getchar();
			while((order = getchar()) != EOF)
			{
				if(order == 'l')
				{
					load(programs,choice);
				}
				else if(order == 'd')
				{
					del(programs);
				}
				PrintMem();
				cout<<"Input: l-load programs d-delete programs"<<endl;
				cout<<"End with EOF"<<endl;
				//order = getchar();
				//system("clear");
				//PrintMem();
			}
		}
};

int main(void)
{
	MemoryManagement test;
	test.console();
	return 0;
}