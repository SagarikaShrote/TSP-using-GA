//Desktop\padhai ka saman\AI UAD\texas road net dataset\AI-20201013T172548Z-001\AI2

#include<bits/stdc++.h>
#include <random>
using namespace std;
#define file "input1.txt"		//input filename for adjescency matrix
#define n 5			//no. of cities
#define GA_iter 50		//no. of iterations/generations
#define ni 10			//no. of individuals in a generation
typedef struct individual
{
	int seq[n];
	int cost;			//cost of path 
	int fitness;		//sum of elements in adjescency matrix-cost
	int lprob,rprob;	//probabilty range for this individual
}ind;
ind final;		//at any point, stores the best soln seen till now
int fitGen;		//sum/total fitness of a generation
int dMsum=0;		//sum of elements in adjescency matrix
int elite;			//stores the best individual in the previous generation so that it can be taken forward

//helper fn to print soln
void printSoln(){
	cout<<"The sequence is: "<<endl;
	for(int i=0;i<n;i++){
		cout<<final.seq[i]<<" ";
	}
	cout<<endl;
	cout<<"The Cost of traversal is: "<<final.cost<<endl;
	cout<<"The fitness is: "<<final.fitness<<endl;
}

//helper function to check if seq is unique
bool isUnique(int * a){
	int temp[n]={0};
	bool u=true;		//unique
	for(int i=0;i<n && u;i++){
		if(temp[a[i]]!=0){
			u=false;
		}
		else{
			temp[a[i]]++;
		}
	}
	return u;
}
//helper fn to check if cost<final.cost and is the sequence unique
//fn to check if this is better than the soln found so far
void check_soln(vector<ind>& gen){
	// int flag=1;
	for(int x=0;x<ni;x++){
		bool bour=isUnique(gen[x].seq);
		bool bfinal=isUnique(final.seq);
		if(bour && bfinal && final.cost>gen[x].cost){
				//copy_individual()
				//copying seq
				for(int i=0;i<n;i++){
					final.seq[i]=gen[x].seq[i];
				}
				//copying cost
				final.cost=gen[x].cost;
				final.fitness=gen[x].fitness;
				// flag=0;
		}
		else if(bour && !bfinal){
				//copy_individual()
				//copying seq
				for(int i=0;i<n;i++){
					final.seq[i]=gen[x].seq[i];
				}
				//copying cost
				final.cost=gen[x].cost;
				final.fitness=gen[x].fitness;
		}
		else if(!bour && !bfinal && final.cost>gen[x].cost){
				//copy_individual()
				//copying seq
				for(int i=0;i<n;i++){
					final.seq[i]=gen[x].seq[i];
				}
				//copying cost
				final.cost=gen[x].cost;
				final.fitness=gen[x].fitness;
		}
	}
	return;
}
//helper fn to set fitness of an individual
void set_fitness_and_cost(ind& i,int**dM){
	//seting cost
	i.cost=0;
	for(int j=0;j<n;j++){
		i.cost+=dM[i.seq[j%n]][i.seq[(j+1)%n]];
	}
	//setting fitness
	i.fitness=dMsum-i.cost;
	// i.fitness=100/i.cost;
	return;
}
//helper fn to set the fitness of the generation
void set_fitGen(vector<ind>& gen){
	fitGen=0;
	int max=gen[0].fitness;
	for(int i=0;i<ni;i++){
		fitGen+=gen[i].fitness;
		if(gen[i].fitness>max){
			max=gen[i].fitness;
			elite=i;
		}
	}
	return;
}
//helper fn to set probabilities of individuals
void set_probs(vector<ind>& gen){
	int temp=-1;
	for(int i=0;i<ni;i++){
		gen[i].lprob=temp+1;
		gen[i].rprob=temp+1+((gen[i].fitness/fitGen)*100);
		temp=gen[i].rprob;
	}
	return;
}
//initializing the first generation
void set_initial_generation(vector<ind>& gen,int ** dM){
	//cout<<"hello 1"<<endl;
	for(int i=0;i<ni;i++){
		for(int j=0;j<n;j++){
			gen[i].seq[j]=rand()%n;
		}
		set_fitness_and_cost(gen[i],dM);
	}
	return;
}
//crossover fn
void crossover(vector<ind>& gen){
	vector<ind> temp;
	//crossing over for half time population
	for(int j=0;j<ni/2;j++){
		int roulette_wheel1=rand()%100;
		int roulette_wheel2=rand()%100;
		int a=0,b=1;
		for(int x=0;x<ni;x++){
			if(roulette_wheel1>=gen[x].lprob && roulette_wheel1<=gen[x].rprob){
				a=x;
			}
			if(roulette_wheel2>=gen[x].lprob && roulette_wheel2<=gen[x].rprob){
				b=x;
			}
		}
		int cpt=rand()%n;
		ind t1,t2;
		//actual crossover
		for(int x=0;x<cpt;x++){
			t1.seq[x]=gen[a].seq[x];
			t2.seq[x]=gen[b].seq[x];
		}
		for(int x=cpt;x<n;x++){
			t1.seq[x]=gen[b].seq[x];
			t2.seq[x]=gen[a].seq[x];
		}
		temp.push_back(t1);
		temp.push_back(t2);
	}
	//replacing the old generation with the new
	if(temp.size()<ni){
		ind tind;
		for(int h=0;h<n;h++){
			tind.seq[h]=gen[elite].seq[h];
		}
		temp.push_back(tind);
	}
	gen.clear();
	for(int x=0;x<ni;x++){
		ind tind;
		for(int h=0;h<n;h++){
			tind.seq[h]=temp[x].seq[h];
		}
		gen.push_back(tind);
	}
	temp.clear();
}
//mutation fn
void mutate(vector<ind>& gen,int**dM){
	for(int x=0;x<ni;x++){
		int r=rand()%2;
		if(r==1){
			//mutate
			int pos=rand()%n;
			int city=rand()%n;
			gen[x].seq[pos]=city;
		}
		set_fitness_and_cost(gen[x],dM);
	}
	return;
}

//wrapper function
void GA(vector<ind>& generation,int** distMatrix){
	//cout<<"hello GA"<<endl;
	check_soln(generation);
	cout<<"The best solution found in initial generation is :"<<endl;
	printSoln();
	for(int i=0;i<GA_iter;i++){
		//cout<<"hello 1 "<<i<<endl;
		crossover(generation);
		//cout<<"hello 2 "<<i<<endl;
		mutate(generation,distMatrix);
		//cout<<"hello 3 "<<i<<endl;
		set_fitGen(generation);
		//cout<<"hello 4 "<<i<<endl;
		set_probs(generation);
		//cout<<"hello 5 "<<i<<endl;
		check_soln(generation);
		cout<<"-------------------------ended generation "<<i+1<<"-------------------------"<<endl;
		cout<<"The best solution found till now is :"<<endl;
		printSoln();
	}
	cout<<"-----------------------------------------------------------------------------------"<<endl;
	cout<<"The best solution found is:"<<endl;
	printSoln();
}

int main(){
	//seeding the random no. generator
	srand(time(0));
	for(int i=0;i<n;i++){
		final.seq[i]=0;
	}
	final.cost=INT_MAX;
	int **distMatrix=new int*[n];
	for(int i=0;i<n;i++){
		distMatrix[i]=new int[n];
	}
	ifstream is(file);
	cout<<"Enter adjescency matrix:\n";
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			is >>distMatrix[i][j];
			dMsum+=distMatrix[i][j];
		}
	}
	is.close();
	vector<ind> generation(ni);
	set_initial_generation(generation,distMatrix);
	set_fitGen(generation);
	set_probs(generation);
	GA(generation,distMatrix);
	return 0;
}