//amitgomi
#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
 
#define ll long long int
#define fi first
#define se second
#define pb push_back
#define all(x) x.begin(),x.end()
#define mp(x,y) make_pair(x,y)
#define mod 998244353
#define sc(x) scanf("%d", &x)
#define sl(x) scanf("%lld", &x)
#define pi pair<int,int>
 
///////////////////////////////////////////////////////////
// ll bin_expo(ll a,ll b, ll m){ ll res =1; a %= m; while(b!=0){ if(b%2==1) res = (res*a)%mod; a = (a*a)%mod; b = b/2; } return res; }
// ll gcd(ll a, ll b) { if (b == 0) return a; return gcd(b, a % b); } 
///////////////////////////////////////////////////////////

vector<vector<vector<int>>> best_sol;
int best_score = INT_MAX;

int getSum(const vector<vector<vector<int>>> &shape){
    int sum = 0;
    for(auto square:shape){
        for(int i=0;i<square.size();i++){
            for(int j=0;j<square[0].size();j++){
                if((i==0 && j==0)||(i==1 && j==0)) continue;
                sum += square[i][j];
            }
        }
    }
    return sum;
}

void printSol(vector<vector<vector<int>>> shape){
    cout<<"Printing sol"<<endl;
    for(auto square: shape){
        for(auto row : square){
            for(auto el:row){
                cout<<el<<' ';
            }
            cout<<endl;
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<getSum(shape)<<endl;
}

map<pair<int,int> , vector<vector<vector<int>>>> readAmSquares(){
    ifstream my_file("output.txt");
    map<pair<int,int> , vector<vector<vector<int>>>> res;
    while(!my_file.eof()){
        vector<vector<int>> square(3,vector<int>(3, 0));
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                my_file>>square[i][j];
            }
        }
        if(square[0][0]!=0)
            res[make_pair(square[0][0], square[1][0])].push_back(square);
    }
    return res;
}

bool mayBecomeValidShape(vector<vector<vector<int>>> &shape){
    int n = shape.size();
    for(int i=0;(i<n-1)||(n == 4 && i==3);i++){
        vector<vector<int>> current_square = shape[i];
        vector<vector<int>> next_square = shape[(i+1)%4];
        if(current_square[2][0] != next_square[1][0] || 
           current_square[2][1] != next_square[0][0]){
            return false;
        }
    }

    // validate duplicates
    set<int> st;
    for(auto square:shape){
        for(auto row:square){
            for(auto el:row){
                st.insert(el);
            }
        }
    }
    if((shape.size() == 0 && st.size() ==0) ||
       (shape.size() == 1 && st.size() ==9) ||
       (shape.size() == 2 && st.size() ==16) ||
       (shape.size() == 3 && st.size() ==23) ||
       (shape.size() == 4 && st.size() ==28)){
        // do nothing
    }
    else{
        return false;
    }
    return true;
}



vector<vector<vector<int>>> selectBetter(vector<vector<vector<int>>> &s1, vector<vector<vector<int>>> &s2){
    if(s1.size() == 0) return s2;
    if(s2.size() == 0) return s1;
    if(getSum(s1) > getSum(s2)) return s2;
    return s1;
}

vector<vector<vector<int>>> rec(vector<vector<vector<int>>> &shape, map<pair<int,int> , vector<vector<vector<int>>>> &am_squares){
    vector<vector<vector<int>>> res;
    if(!mayBecomeValidShape(shape)) return res;
    if(shape.size() == 4){
        if(mayBecomeValidShape(shape)){
            if(best_score > getSum(shape)){
                best_score = getSum(shape);
                best_sol = shape;
                printSol(best_sol);
            }
            return shape;
        }
        return res;
    }
    vector<vector<int>> last_square = shape[shape.size()-1];
    for(auto square:am_squares[make_pair(last_square[2][1], last_square[2][0])]){
        shape.push_back(square);
        auto sol = rec(shape, am_squares);
        res = selectBetter(res, sol);
        shape.pop_back(); // back tracking
    }
    return res;
}

vector<vector<vector<int>>> solve(map<pair<int,int> , vector<vector<vector<int>>>> &am_squares){
    vector<vector<vector<int>>> res;

    // map to vector for paralalization.
    vector<vector<vector<int>>> all_squares;
    for(auto it:am_squares){
        all_squares.insert(all_squares.end(), it.second.begin(),it.second.end());
    }

#pragma omp parallel for
    for (int i=0;i<all_squares.size();i++){
        vector<vector<vector<int>>> shape;
        shape.push_back(all_squares[i]);
        auto sol = rec(shape, am_squares);
#pragma omp critical
        {
            res = selectBetter(res, sol);
        }
    }

    return res;
}

int main(){
	// ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    map<pair<int,int> , vector<vector<vector<int>>>> am_squares;
    am_squares = readAmSquares();
    cout<<am_squares.size()<<endl;

    vector<vector<vector<int>>> ans = solve(am_squares);

    printSol(ans);
	return 0;
}