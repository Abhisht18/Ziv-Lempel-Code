//Abhisht Bose
//19EC30002 - Digi. Comm. Project
#include<bits/stdc++.h>
#include <fstream>
using namespace std;
#define ll long long int
#define ld long double
#define endl "\n"
#define IOS ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
ll inf = 1000000007 ;
ll facto[1000010];
// ll power(ll a,ll pow){
// 	ll ans=1;
// 	if(a<0) a+=inf;
// 	while(pow){
// 		if(pow%2==1){
// 			ans = ((ans%inf)*(a%inf))%inf;
// 		}
// 		a = ((a%inf)*(a%inf))%inf;
// 		if(a<0) a+=inf;
// 		pow/=2;
// 	}
// 	return ans;
// }
// void fact(int n){
// 	ll fac = 1;
// 	for(int i=1;i<=n;i++){
// 		fac = ((fac%inf)*(i%inf))%inf;
// 		facto[i] = fac;
// 	}
// }
// void djikstra(int nodes){
// 	dist[nodes]=0;
// 	pq.push({-0, nodes});
// 	while(!pq.empty()){
// 		ll node = pq.top().second, dis = -pq.top().first;
// 		pq.pop();
// 		if(vis[node]) continue;
// 		vis[node] = 1;
// 		for(auto it: ed[node]){
// 			if(dist[it.second] == -1 || dist[it.second]> dis+it.first){
// 				dist[it.second] = dis+it.first;
// 				pq.push({-dist[it.second], it.second});
// 			}
// 		}
// 	}
// }
string flenc, ANS;  //flenc is binary encoded string and ANS is the decoded one

string toBinary(int n, int l){  //Code to convert ASCII values to binary string of fixed length
	string ans;
	while(n){
		ans.push_back('0' + n%2);
		n/=2;
	}
	if(l){
		for(int i=ans.size();i<l;i++) ans.push_back('0');
		reverse(ans.begin(), ans.end());
	}
	return ans;
}

int toDecimal(string s){    //Code to convert binary string to decimal value
	int ans = 0, n=s.size()-1;
	for(int i=n;i>=0;i--){
		ans += (s[i]-'0')*(1<<(n-i));
	}
	return ans;
}

int main(){
	facto[0]=1;
	string s;
	string rfilename, wfilename, myText;   //Variable to store files name
	cout<<"Enter the file name from where we can take input: FORMAT - [Name of File only without extension]"<<endl;
	cin>>rfilename;
	cout<<"Enter the file name where we can print the final result: FORMAT - [Name of File only without extension]"<<endl;
	cin>>wfilename;
	rfilename.append(".txt");
	wfilename.append(".txt");
	cout<<"Enter '1' if you want to encode a txt file or enter '0' if you want to decode that txt file content:"<<endl;
	int choice; // Enter choice whether to encode file or decode it.
	cin>>choice;
	int window = 1024;   //Size of window for fixed length encoding

	if(choice){ //If chosen procedure is to Encode
		ifstream MyReadFile(rfilename, ios::binary); //Read the file
		while (getline (MyReadFile, myText)) {
			s.append(myText);
		}
		MyReadFile.close();

		for(int i=0;i<min(window,int(s.length()));i++){  //Fixed Length Encoding for window
			int x = int(s[i]);
			string bin = toBinary(x, 8);
			flenc.append(bin);
		}

		vector<pair<pair<int,int>,char>> dictionary;  //Defined Dictionary here

		for(int i=window;i<s.length();i++){
			vector<int> pos;  //Storing all occurence of current element in the window
			int maxlen = 0, idx = 0;
			for(int j=i-1;j>i-1-window;j-=1){
				if(s[j] == s[i]) pos.push_back(j);
			}

			if(!pos.size()){   //If that element do not exists then fixed length encoding that element
				dictionary.push_back({{0,0}, s[i]});
				continue;
			}

			for(auto it: pos){ //Greedy method to find max value of N
				int len = 1;
				while(i+len < s.size()){
					if(s[i+len] != s[it+len]) break;
					len+=1;
				}
				if(maxlen < len){
					maxlen = len;
					idx = it;
				}
			}

			if(maxlen == 1){ //If value of N is less than 2, then I am using fixed length encoding for it
				dictionary.push_back({{1,0}, s[i]});
				continue;
			} 

			else dictionary.push_back({{maxlen,i-idx}, s[i]}); //Else push back value of U and N
			i+=maxlen-1;
		}

		for(auto it: dictionary){ //Encoding the remaining txt using dictionary
			if(it.first.first == 0 || it.first.first == 1){ //Appending 1 to show fixed length encoding
				flenc.push_back('1');
				int x = int(it.second);
				string bin = toBinary(x, 8);
				flenc.append(bin);
				continue;
			}
			string bin = toBinary(it.first.first, 0);  //Encoding dictionary as unary(N) + 10 bit encoding of U
			int sz = bin.size();
			for(int i=0;i<sz-1;i++){
				bin.push_back('0');
			}
			reverse(bin.begin(), bin.end());
			flenc.append(bin);
			bin = toBinary(it.first.second-1, 10);
			flenc.append(bin);
		}

		if(flenc.length()%8){ //padding of zero if It is not a multiple of 8
			for(int i=0;i<8-flenc.length()%8;i++) flenc.push_back('0');
		}

		string encodedString; //encoded string to be stored. Compressed file
		for(int i=0;i<flenc.size();i+=8){
			encodedString.push_back(char(toDecimal(flenc.substr(i,8))));
		}

		ofstream MyFile(wfilename);
		MyFile<<dictionary.size()<<endl; //Passing length of dictionary as 1st parameter and rest string as second
		MyFile<<encodedString;
		MyFile.close();

		cout<<"Enter '1' if you want to see the dictionary else enter '0':"<<endl;
		int chc;
		cin>>chc;
		if(chc){
			cout<<"Length of Dictionary: "<<dictionary.size()<<endl;
			for(auto it: dictionary){
				cout<<'<'<<it.first.first<<','<<it.first.second<<'>'<<' '<<it.second<<endl;
			}
		}

	}

	//Decoding_________________________________________________________
	else{
		ifstream MyReadFile(rfilename, ios::binary); //Reading file that needed to eb decoded
		int lines = 0, lenOfDictionary = 0;

        getline (MyReadFile, myText);
        lenOfDictionary = stoi(myText); //Storing Dictionary length

		char cc =0;
		while (getline (MyReadFile, myText)) { 
            s.append(myText);
			cc = *myText.rbegin();
			s.pop_back();  //This step is needed to remove /n/r from the string and append /n only
			s.push_back('\n');
		}
		s.pop_back();
		s.push_back(cc); //Since last line wont contain /n/r we should push back actual character back
		MyReadFile.close();

		for(auto it:s){ //Creating flenc that will contain binary equivalent of txt file
			int x = int(it);
			if(x<0) x+=256;
			flenc.append(toBinary(x, 8));
		}
	
		for(int i=0;i<window*8;i+=8){ //Decoding the fixed initial window
		    ANS.push_back(char(toDecimal(flenc.substr(i, 8))));
		}
		
		int idx = window*8;
		vector<pair<pair<int,int>,char>> constructedDictionary; 
		while(idx < flenc.size()){
			if(flenc[idx]=='0'){ //If first character is 0, it means that segment was encoded using E(n,u) method
				int sz = 1;
				while(flenc[idx]=='0'){
					sz+=1;
					idx+=1;
				}
				int valn = toDecimal(flenc.substr(idx, sz)); //Extracting value of N
				idx+=sz;
				int valu = toDecimal(flenc.substr(idx, 10))+1; //Extracting value of U
				string ref = ANS.substr(ANS.size()-valu, min(valn, valu));
				constructedDictionary.push_back({{valn, valu}, ref[0]}); //Constructing Dictionary
				if(valu >= valn){
					ANS.append(ref);
				}
				else{
					for(int i=0;i<valn;i++){
						ANS.push_back(ref[i%valu]);
					}
				}
				idx += 10;
			}
			else{ //Else it was encoded using its ASCII value
				ANS.push_back(char(toDecimal(flenc.substr(idx+1, 8))));
				constructedDictionary.push_back({{0,0}, *ANS.rbegin()});
				idx+=9;
			}
			if(constructedDictionary.size() == lenOfDictionary) break; //If the dictionary constructed so far mathces the length then break the loop as left it are just padded ones.
		}

        ofstream MyFile1(wfilename);
		MyFile1<<ANS;
		MyFile1.close();

		cout<<"Enter '1' if you want to see the constructed dictionary else enter '0':"<<endl;
		int chc;
		cin>>chc;
		if(chc){
			cout<<"Length of Dictionary: "<<constructedDictionary.size()<<endl;
			for(auto it: constructedDictionary){
				cout<<'<'<<it.first.first<<','<<it.first.second<<'>'<<' '<<it.second<<endl;
			}
		}
	}
	return 0;
}
