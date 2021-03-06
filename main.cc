
    #include "read.h"
    #include "expr.h"
    #include "passjoin.h"
    #include<chrono>
    #include <sstream>
    #include <map>
    #include <iostream>
    #include "key_Mod.h"
    #include "utils.h"
    #include <math.h>
#include <thread>
    using namespace std;
	passjoin * p;
	ReadFile * r1;
	int *numthread;
	int *numlen;
	void call_from_thread(pair<int,pair<int, int>>start_end){
		//cout<<
		//cout << start_end.second.second << endl;
		for (int i = start_end.second.first; i < start_end.second.second; i++){
			p->self_join(i + p->start, r1->words,start_end.first);
		}
	}
	void call_from_thread_1(pair<int, int>start_end){
		//cout<<
		//cout << "ssssss"<<start_end.first << endl;
		//cout << *numlen << endl;

		for (int i = start_end.second; i < *numlen; i+=(p->tau)*(*(numthread))){
			
			for (int j = (i - p->tau)>0 ? (i - p->tau) : 0; j < i; j++){
				//cout << j << endl;
				p->R_S_join(j + p->start, i + p->start, r1->words, start_end.first);
			
			}
			//cout << i << endl;
		}
	}
    bool length_sort(pair<int, int> x, pair<int, int> y){

        return x.first < y.first || x.first == y.first && x.second <= y.second;

    }
    bool less_vector(pair<int, vector<int>>x, pair<int, vector<int>>y){
        return x.second.size() > y.second.size();
    }
    inline int ceiling1(int x, int y){
        return 1 + ((x - 1) / (y));
    }

    void init(int tau, ReadFile r, int first_length, int first_index, passjoin & p,int k){
        utils utils1;
        p.tau = tau;
        p.verified = new int[r.wcnt];
        p.dist=new int[strlen(r.words[r.wcnt-1])+2];
		p.candidates_l = 0;
		
        memset(p.dist, 0, sizeof(p.dist));

        p.first_match_pair = new pair<int, int>[r.wcnt];

        int clen = 0;
        first_length = strlen(r.words[0]);
        for (int i = 0; i < r.wcnt; i++){
            //p.lenc[strlen(r.words[i]) - first_length]++;
          if(clen!=strlen(r.words[i])){
            for(int k=clen+1;k<=strlen(r.words[i]);k++){
                p.dist[k]=i;
            }
            clen=strlen(r.words[i]);
          }
            p.first_match_pair[i] = make_pair(-1, -1);
        }
        p.dist[strlen(r.words[r.wcnt-1])+1]=r.wcnt;

        p.filter_operation = 0;
        int array_size = 900;
		p.v0 = new int[array_size];
		p.v1 = new int[array_size];
		p.e = new int[array_size];

		p.v01 = new int*[8];
		p.v11 = new int*[8];
		p.e1 = new int*[8];
		for (int i = 0; i < 8; i++){
			p.v01[i] = new int[array_size];
			p.v11[i] = new int[array_size];
			p.e1[i] = new int[array_size];
		}


        size_t ind_tree_size = strlen(r.words[r.wcnt - 1]) - first_length + 1;
        //cout << first_length << endl;
        p.lenc = new int[ind_tree_size];
        memset(p.lenc, 0, sizeof(p.lenc));
        p.curr_i = new int[ind_tree_size];
        p.seg_count = new int[ind_tree_size];
        p.strings_indexed=new vector<int>**[ind_tree_size];
        p.strings_len_indexed=new vector<int>[ind_tree_size];
		p.results = new pair<long, long>[ind_tree_size];
        p.L2 = new InvList_Seg *[ind_tree_size];
        p.L_B0_B1 = new InvList_Seg ***[ind_tree_size];
       // cout << "dddd" << endl;
        for (int i = 0; i < ind_tree_size; i++){
            p.strings_indexed[i]=new vector<int> *[2*p.tau+1];
			p.results[i] = make_pair(0, 0);
            p.L_B0_B1[i] = new InvList_Seg**[2*p.tau+1];
            for(int j=0;j<2*p.tau+1;j++){
                    p.strings_indexed[i][j]=new vector<int>[2*p.tau+1-j];
                    p.L_B0_B1[i][j] = new InvList_Seg*[2*p.tau+1-j];
                    for(int k1=0;k1<2*p.tau+1-j;k1++){
                        p.L_B0_B1[i][j][k1] = new InvList_Seg[p.tau + k];
                    }
            }
            p.L2[i] = new InvList_Seg[p.tau + k];
        }
        //cout << "ssss" << endl;
       // memset(p.coutChar, 0, (r.wcnt) * 4);
        p.histograms = new int*[r.wcnt];
        p.histograms_SSE = new float*[r.wcnt];
        p.histograms_first= new float*[r.wcnt];
        p.histograms_second = new float*[r.wcnt];
        p.hist_2=new short*[r.wcnt];

        char a = 'a';
        char z = 'z';
   /*     for (int i = first_index; i < r.wcnt; i++){
            p.coutChar[i] = utils1.countChar(r.words[i]);
        }*/
      //  cout << "wwww" << endl;
        for (int i = first_index; i < r.wcnt; i++){

            p.histograms_first[i]= new float[28];
            p.histograms_second[i]= new float[28];
            memset(p.histograms_first[i], 0, sizeof(p.histograms_first[i]));
            memset(p.histograms_second[i], 0, sizeof(p.histograms_second[i]));
            for (int j = 0; j < strlen(r.words[i])/2; j++){
                if (r.words[i][j] >= a&&r.words[i][j] <= z){
                    p.histograms_first[i][r.words[i][j] - a] ++;
                }
                else{
                    p.histograms_first[i][26]++;
                }

            }
            for (int j = strlen(r.words[i])/2; j < strlen(r.words[i]); j++){
                if (r.words[i][j] >= a&&r.words[i][j] <= z){
                    p.histograms_second[i][r.words[i][j] - a] ++;
                }
                else{
                    p.histograms_second[i][26]++;
                }

            }
        }
        /*cout << "fffff" << endl;*/
        for (int i = first_index; i < r.wcnt; i++){

            p.histograms[i] = new int[28];
            p.histograms_SSE[i] = new float[28];

            memset(p.histograms[i], 0, sizeof(p.histograms[i]));
            memset(p.histograms_SSE[i], 0, sizeof(p.histograms_SSE[i]));

            for (int j = 0; j < strlen(r.words[i]); j++){
                if (r.words[i][j] >= a&&r.words[i][j] <= z){
                    p.histograms[i][r.words[i][j] - a] ++;
                    p.histograms_SSE[i][r.words[i][j] - a] ++;

                }
                else{
                    p.histograms[i][26]++;
                    p.histograms_SSE[i][26]++;

                }

            }


        }
        for (int i=first_index; i <r.wcnt; i++){
            p.hist_2[i] = new short[3];
                memset(p.hist_2[i],0,6);
                for (int j = 0; j < 2*p.tau; j++){
                if (r.words[i][j] >= a&&r.words[i][j] <= a+13){
                    p.hist_2[i][0]++;
                }
                else if (r.words[i][j] >= a+14&&r.words[i][j] <= z){
                    p.hist_2[i][1]++;
                }else if(!(r.words[i][j] >= a&&r.words[i][j] <= z)){
                p.hist_2[i][2]++;
                }

            }
				
                p.lenc[strlen(r.words[i]) - strlen(r.words[0])]++;

                p.strings_len_indexed[strlen(r.words[i]) - strlen(r.words[0])].push_back(i);
                 p.strings_indexed[strlen(r.words[i])-strlen(r.words[0])][p.hist_2[i][0]][p.hist_2[i][1]].push_back(i);
        }
        cout << "wwwwwwwww" << endl;
        p.start = first_length;
        p.substrings = new unordered_map<Key_M, int>***[r.wcnt];
        //p.substringss = new Trie***[r.wcnt];
        unordered_map<Key_M, int> **curr;
        //Trie **curr;
        long res = 0;

        p.end = strlen(r.words[r.wcnt - 1]);
        p.candidates = 0;
        p.overallSubstrings = 0;
        p.matched_pair = 0;
        p.matched_strings.reserve(4000000);
        //p.matched_groups.reserve(4000000);
        memset(p.verified, -1, (r.wcnt) * 4);

       // memset(p.first_match, -1, (r.wcnt) * 4);
        //memset(p.mark_covered, 0, (r.wcnt) * 4);

        //memset(p.mark_overlapping, -1, (r.wcnt) * 4);
        memset(p.v0, 0, array_size * 4);
        memset(p.v1, 0, array_size * 4);
        memset(p.e, 0, array_size * 4);

    }


    int  main(int argc, char** argv){

        char * input_file = argv[2];

        //passjoin p;
        string tau(argv[1]);
        stringstream str(tau);
        int tau_main;
        str >> tau_main;

        char* output_file = argv[3];

        cout << input_file << endl;
        ReadFile r(input_file);
        r.sortFile();

        cout << EXPR::sort_time << endl;
        int first_length = 0;
        int left = 0;
        int right = r.wcnt - 1;
        int first_index = 0;

        first_index = 0;
        first_length = 6;

    passjoin  p1;
	p = &p1;
	r1 = &r;
        //init(tau_main, r, first_length, first_index, p,1);
        init(tau_main , r, first_length, first_index, p1,2);
        //p1.myfile.open("E:\\programs\\github\\bin\Release\\out.txt");
    p1.myfile.open(output_file);
        auto start=chrono::high_resolution_clock::now();

            int len=strlen(r.words[r.wcnt-1])-strlen(r.words[0])+1;
			int default1 = r.wcnt / 8;
			int * arr = new int[8];
			int sum = 0;
			int i2 = 0;
			for (int l = 0; l < len; l++){
				
				if (abs(sum-default1)<p1.lenc[l]&&i2<7){
					arr[i2] = l;
					i2++;
					sum = p1.lenc[l];
				}
				else{
					sum += p1.lenc[l];
				}
			}
			arr[i2] = len;
			//for (int i = 0; i < r.wcnt; i++){
			//	p1.candidates += p1.candidate_tau_plus_two_hist_len_tree_ind(r.words[i], strlen(r.words[i]), i, r.words);
			//	p1.partion_tau_2_tree_ind(r.words[i], strlen(r.words[i]), i);
			//}
			//cout <<" SS "<< p1.candidates_l << endl;
			//thread t1(call_from_thread, make_pair(0,make_pair(0, len / 8)));
			//thread t2(call_from_thread, make_pair(1,make_pair(len / 8, len / 4)));
			//thread t3(call_from_thread, make_pair(2,make_pair(len / 4, 3*len/8)));
			//thread t4(call_from_thread, make_pair(3,make_pair(3 * len / 8, len/2)));
			//thread t5(call_from_thread, make_pair(4, make_pair(len/2, 5*len / 8)));
			//thread t6(call_from_thread, make_pair(5, make_pair(5 * len / 8, 6 * len / 8)));
			//thread t7(call_from_thread, make_pair(6, make_pair(6 * len / 8, 7 * len / 8)));
			//thread t8(call_from_thread, make_pair(7, make_pair(7 * len / 8, len)));
			thread t1(call_from_thread, make_pair(0, make_pair(0, arr[0])));
			thread t2(call_from_thread, make_pair(1, make_pair(arr[0], arr[1])));
			thread t3(call_from_thread, make_pair(2, make_pair(arr[1], arr[2])));
			thread t4(call_from_thread, make_pair(3, make_pair(arr[2], arr[3])));
			thread t5(call_from_thread, make_pair(4, make_pair(arr[3], arr[4])));
			thread t6(call_from_thread, make_pair(5, make_pair(arr[4], arr[5])));
			thread t7(call_from_thread, make_pair(6, make_pair(arr[5], arr[6])));
			thread t8(call_from_thread, make_pair(7, make_pair(arr[6], arr[7])));
			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
    //        for (int i = 0; i<len; i++){
    //            //cout << i << endl;
    //            if (p1.lenc[i]>0){

				//	 p1.self_join(i+p1.start,r.words);
				//}


    //        }
			for (int i = 0; i < len; i++){
				p1.candidates += p1.results[i].second;
				p1.results[i].second = 0;
				p1.matched_pair += p1.results[i].first;
				p1.results[i].first = 0;
			}
			//cout << " SS " << p1.candidates << endl;

			thread **t=new thread*[p1.tau];
			int size = 0;
			if (len > 8){
				size = 8;
			}else{
				size = len;
	}
			numlen = &len;
			numthread = &size;
			for (int k = 0; k < p1.tau; k++){
				t[k] = new thread[size];

				for (int i = 0; i < size; i++){
					t[k][i] = thread(call_from_thread_1, make_pair(i, (p->tau)*i + k));
					//call_from_thread_1(make_pair(i, (p->tau)*i+k));
					
				}
				for (int i = 0; i < size; i++){
					t[k][i].join();
					

				}
			}

			for (int i = 0; i < len; i++){
				p1.candidates += p1.results[i].second;
				p1.matched_pair += p1.results[i].first;
			}
                 long  cnt=(2*p1.tau+2)*(2*p1.tau+1)/2 *len;
                    cout << cnt;
                    auto end = chrono::high_resolution_clock::now();
                    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                    cout << "1 running candidates filter_operation " << ms << "\t" << p1.matched_pair << "\t" << p1.candidates << endl;

                    /*sort(p1.matched_strings.begin(), p1.matched_strings.begin() + p1.matched_pair, length_sort);
                    cout << "sss" << endl;
                    for (auto it = p1.matched_strings.begin(); it != p1.matched_strings.end(); ++it){
                        p1.myfile << (*it).first << " , " << (*it).second << endl;
                    }*/
                    p1.myfile.close();

    return 0;
    }



