//#ifdef test
#include "read.h"
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include "passjoin.h"
#include "Key_M.h"
using namespace std;

<<<<<<< f0de0df0e9f1a71f4d9281a3770714c3dfb0f86e
int len;
int rebase;
struct Key_M
{
 	int seg_length; //the segment length
	char *seg;
	Key_M(const char * seg,int seg_length){
=======
// int len;
// struct Key_M
// {
 	// int seg_length; //the segment length
	// char *seg;
	// Key_M(const char * seg,int seg_length){
>>>>>>> partition by 3 bin histogram 2 for english and one for non english

	// this->seg_length=seg_length; //the segment length
	// this->seg=( char *)seg;
        // hash_val=compute_hash();
// }
	// bool operator==(const Key_M &other) const
  // { 
	// return (strncmp(seg, other.seg,seg_length)==0);
      
	
// }
        // size_t compute_hash() const{
	// size_t result = 2166136261;
         // for(int i=0;i< seg_length;i++)
	// result = (result * 16777619) ^  seg[i] ;
        
	// return result;
// }
		
        // size_t hash_val;
	
// };

// namespace std {

  // template <>
  // struct hash<Key_M>
  // {
    // std::size_t operator()(const Key_M& k) const
    // {
     // return k.hash_val;
    // }
  // };

// };

int  main(int argc, char** argv){

	char * input_file = argv[2];
	unordered_map<Key_M,int>*** map;
	passjoin p;
	string tau(argv[1]);
	stringstream str(tau);
	int tau_main;
	str >> tau_main;
	cout << input_file << endl;
	ReadFile r(input_file);
	r.sortFile();
	int len=strlen(r.words[r.wcnt-1])-strlen(r.words[0])+1;
	int * lenc=new int [len];
	memset(lenc,0,sizeof(lenc));
	map=new unordered_map<Key_M,int>**[len];

	short ** hist_2=new short*[r.wcnt];
		char a = 'a';
		char z = 'z';
		vector<int> *** count=new vector<int>**[len];
		for(int i=0;i<len;i++){
			count[i]=new vector<int> *[2*tau_main+1];
			map[i]=new unordered_map<Key_M,int>*[2*tau_main+1];
			for(int j=0;j<2*tau_main+1;j++){
				count[i][j]=new vector<int>[2*tau_main+1-j];
				map[i][j]=new unordered_map<Key_M,int>[2*tau_main+1-j];
				memset(count[i][j],0,sizeof(count[i][j]));
			}
		}

		for (int i=0; i <r.wcnt; i++){
			lenc[strlen(r.words[i])-strlen(r.words[0])]++;
		}
	
	for (int i=0; i <r.wcnt; i++){
			hist_2[i]=new short[3];
			memset(hist_2[i],0,6);
			for (int j = 0; j < 2*tau_main; j++){
			if (r.words[i][j] >= a&&r.words[i][j] <= a+13){
				hist_2[i][0]++;
			}
			else if (r.words[i][j] >= a+14&&r.words[i][j] <= z){
				hist_2[i][1]++;
			}else if(!(r.words[i][j] >= a&&r.words[i][j] <= z)){
				hist_2[i][2]++;
			}

		}

			 count[strlen(r.words[i])-strlen(r.words[0])][hist_2[i][0]][hist_2[i][1]].push_back(i);
			// Key_M k(r.words[i],2*tau_main);
			// if(map[strlen(r.words[i])-strlen(r.words[0])][hist_2[i][0]][hist_2[i][1]].count(k)){
				// map[strlen(r.words[i])-strlen(r.words[0])][hist_2[i][0]][hist_2[i][1]].at(k)++;
			// }else{
				// map[strlen(r.words[i])-strlen(r.words[0])][hist_2[i][0]][hist_2[i][1]].emplace(k,1);
			// }
	}
	
			

	for(int i=0;i<len;i++){
		if(lenc[i]<=200){
		
		}else{
			for(int j=0;j<2*tau_main+1;j++){
				for(int k=0;k<2*tau_main+1-j;k++){
					if(count[i][j][k].size()>0)
					{
						
					}
					//sum+=count[i][j][k].size();
				}
			}
		}
	}
cout<<sum<<endl;

return 0;
}
//#endif
