//..............................................................................................................
//..............................................................................................................
//..............................................................................................................
//	IMPLEMENTATION OF INTERVAL TREE
//	CONTAINS FOLLOWING COMMANDS
//	INSERTION, SEARCHING and DELETION
//
//	Reference Taken From : Introduction to algorithm (CLRS)
//..............................................................................................................
//..............................................................................................................
//..............................................................................................................

#include<iostream>
#include<stdio.h>
#include<limits.h>
#include<algorithm>
using namespace std;

//..............................................................................................................
// Structure defined for each node of interval tree
//..............................................................................................................
struct node            
{                      
	node *left,*right,*p;
	int low;
	int high;
	int color;
	int key;
}*root,*nill;


//..............................................................................................................
// LEFT ROTATION FUNCTION
//..............................................................................................................
void rotate_left(node T,node *start)
{   
	node* x=start;
	node* y=(x->right);
	x->right=y->left;
	y->left->p=x;
	y->p=x->p;
	if(x->p==nill)	{root=y;}
	else if(x==(x->p)->left)	{(x->p)->left=y;}
	else	{(x->p)->right=y;}
	y->left=x;
	x->p=y;
}

//..............................................................................................................
//RIGHT ROTATION FUNCTION
//..............................................................................................................
void rotate_right(node T,node* start)
{   
	node* x=start;
	node* y=(x->left);
	x->left=y->right;
	y->right->p=x;
	y->p=x->p;
	if(x->p==nill)	root=y;
	else if(x==(x->p)->left)	(x->p)->left=y;
	else	(x->p)->right=y;
	y->right=x;
	x->p=y;
}

//..............................................................................................................
//Fixing RB insertion tree if any violation found
//..............................................................................................................
void interval_insert_fixup(node T,node* start)
{   
	node*z =start;
	node* y;
	
	while(z->p->color==0)
	{
	        if(z->p==z->p->p->left)
	        {
	            y=z->p->p->right;				// violation : CASE 1
			if(y->color==0)
			{
				z->p->color=1;
				y->color=1;
				z->p->p->color=0;
				z=z->p->p;
			}
			else
			{					// violation : CASE 2
	        	        if(z==z->p->right)
	        	        {
	        	            z=z->p;
	        	            rotate_left(T,z);		// left rotation
	        	        }
	        	        z->p->p->color=0;
	        	        z->p->color=1;
	
	        	        rotate_right(T,z->p->p);	// right rotation
			}
		}

		else
	        {
			y=z->p->p->left;			// violation : CASE 3
			if(y->color==0)
			{
				z->p->color=1;
				y->color=1;
				z->p->p->color=0;
				z=z->p->p;
			}
			else
			{					// violation : CASE 4 
				if(z==z->p->left)
				{
					z=z->p;
					rotate_right(T,z);	//right rotation
				}
				(z->p)->p->color=0;
				z->p->color=1;
				rotate_left(T,z->p->p);		// left rotation
			}
		}
	}
	root->color=1;		// root is always black
}

//..............................................................................................................
//Function to insert any new interval in the interval tree
//..............................................................................................................
void interval_insert(node T,node* start)
{   
	node* z=start;                      
	node* y=nill;
	node* x=root;
	while(x!=nill)
	{
		y=x;
		if(z->low<x->low)	x=x->left;
	        else	x=x->right;
	}
	z->p=y;
	
	if(y==nill)        root=z;
	else if(z->low<y->low)     y->left=z;
	else     y->right=z;
	    z->color=0;

    interval_insert_fixup(T,z);		// calling funcion to handle any violation in insertion tree while inserting 
}




//..............................................................................................................
// helps in picking node during deletion
//..............................................................................................................

node* helper(node* start)
{
	node* z=start;
	node* y;
	if(z->right!=nill)
	{
		while(z->left)
		z=z->left;
		return z;
	}
		y=z->p;
	
	while(y!=nill && z==y->right)
	{
		z=y;
		y=y->p;
	}

	return y;
}

//..............................................................................................................
// Fixing RB interval tree, if any violation occured due to deletion
//..............................................................................................................
void interval_delete_fixup(node T,node* start)
{	
	node* x=start;
	node* y;
	node* w;
    
	while(x!=root && x->color==1)
	{
		if(x==x->p->left)
		{
			w=x->p->right;
			if(w->color==0)
			{
				w->color=1;
				x->p->color=0;
				rotate_left(T,x->p);		//Left rotate
				w=x->p->right;
			}

			if(w->left->color==1 && w->right->color==1)
			{
				w->color=0;
				x=x->p;
			}

			else
			{
 		               if(w->right->color==1)
				{
					w->left->color=1;
					w->color=0;
					rotate_right(T,w);	//Right rotate
					w=x->p->right;
				}

			w->color=x->p->color;
			x->p->color=1;
			w->right->color=1;
			rotate_left(T,x->p);
			x=root;
			}
		}


		else
		{
			w=x->p->left;
			if(w->color==0)
			{
				w->color=1;
				x->p->color=0;
				rotate_right(T,x->p);	//Right Rotate
				w=x->p->left;
			}

			if(w->right->color==1 && w->left->color==1)
			{
				w->color=0;
				x=x->p;
			}
			else
			{
				if(w->left->color==1)
				{
					w->right->color=1;
					w->color=0;
					rotate_left(T,w);	//Left Rotate
					w=x->p->left;
				}
				w->color=x->p->color;
				x->p->color=1;
				w->left->color=1;
				rotate_right(T,x->p);
				x=root;
			}
		}
	}

	x->color=1;
}



//..............................................................................................................
// Function to delete any node if present in the interval tree
//..............................................................................................................
void interval_delete(node T, node* start)
{      
	node* z=start;                   
	node *x,*y;

	if(z->left==nill || z->right==nill)	{y=z;}
	else	y=helper(z);
	
	if(y->left!=nill)
		x=y->left;
	else
		x=y->right;
    
	x->p=y->p;
	if(y->p==nill)	root=x;
	else if(y==y->p->left)	y->p->left=x;
	else	y->p->right=x;
	
	if(y!=z)
	{
		z->low=y->low;
	}
    cout<<"	interval succesfully deleted !!!"<<endl;
    if(y->color==1)
        interval_delete_fixup(T,x);
}

//..............................................................................................................
// Function to search an interval, returns any intersecting interval.
//..............................................................................................................
node* interval_search(node T,int l,int r)
{
	node* x=root;
	while(x!=nill && (r<x->low || l>x->high))
	{
		if(x->left!=nill && x->left->key>=l)
		x=x->left;
		else
		x=x->right;
	}
	return x;
}


//..............................................................................................................
// Maintains the key of interval tree
//..............................................................................................................
int maintain_RB_tree(node* x)
{
    if(x==nill)
        return 0;
    x->key=max(max(maintain_RB_tree(x->left),maintain_RB_tree(x->right)),x->high);
    return x->key;
}



//..............................................................................................................
//initialize nill 
//..............................................................................................................
void initialize()
{
	nill=new struct node;	nill->left=NULL;	nill->right=NULL;	nill->p=NULL;
	nill->color=1;	nill->low=-1;	nill->high=-1;	nill->key=0;
}


//..............................................................................................................
// MENU
//..............................................................................................................
void menu()
{
	cout<<"\t\tMENU\n\t1. Insert\n\t2.SEARCH\n\t3.Delete\n\t0.exit\n Enter your desired option ( digit ) :: ";
}

//..............................................................................................................|
//..............................................................................................................|
//  MAIN DRIVER FUNCTION											|
//..............................................................................................................|
//..............................................................................................................|
int main()    
{
   	initialize();
	int x,y,option;
	node T;
 // TREE INITIALISED
	T.left=nill;	T.right=nill;	T.p=nill;	T.color=1;	T.low=-1;	
	T.high=-1;	T.key=0;	root = &T;
    
	node* ptr;
    
	menu();
	cin>>option;

	while(option!=0)
    {
       
       
        if(option==1)
        {
            cout<<"Enter interval "<<endl;
            cin>>x>>y;
  
		node* a=new struct node;
		a->left=nill;	a->right=nill;	a->p=nill;
		a->color=0;	a->low=x;	a->high=y;
		a->key=0;
	interval_insert(T,a);				// inserting new node in interval tree
              
            
        }

	else if(option==2)
        {
            maintain_RB_tree(root);
            cout<<"Enter interval to be searched :: "<<endl;
		cin>>x>>y;
		ptr=interval_search(T,x,y);		//Function to search any intersecting interval with [ x , y]
		if(ptr->low>=0)
			cout<<"Intersecting Interval Is ::  "<<ptr->low<<" "<<ptr->high<<endl;
                else
                    cout<<"Interval is not present "<<endl;
            
        }

        else if(option==3)
        {
		maintain_RB_tree(root);
            
            cout<<"Enter interval to be deleted  :: "<<endl;
                cin>>x>>y;
                ptr=interval_search(T,x,y);		//Function to search any intersecting interval with [ x , y]		
                if(ptr->low>=0)				// if found delete the interval
			{interval_delete(T,interval_search(T,x,y));}
                else
			cout<<"Interval is not present"<<endl;
	}
        
      
        else if(option==0)
        {
           cout<<"Successfully Exited !!"; break;
        }
        else
            {cout<<"Wrong Option !! Not in Range !!  Aborting !!!!xx-----------xx"<<endl; break;}
	
	menu();
	cin>>option;
    }

return 0;
}
