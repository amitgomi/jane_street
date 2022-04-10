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

int RANGE_MIN = 1;
int RANGE_MAX = 50;
int MAX_SQUARE_SUM = 900;

bool iner_isAmSquare(vector<vector<int> > &square){
    // verify size of square.
    if (square.size()!=3 || 
        square[0].size()!=3 ||
        square[1].size()!=3 ||
        square[2].size()!=3){
        cout<<"not a good square"<<endl;
        return false;
    }
    // verify duplicates in square.
    // verify total sum. total sum is also verified by sum of three elements.
    map<int,int> mp;
    int total_sum = 0;
    for(auto row: square){
        for(int el: row){
            if(el!=-1){
                mp[el]++;
                total_sum+=el;
            }
        }
    }
    if(total_sum > MAX_SQUARE_SUM) return false;
    for(auto it:mp){
        if(it.second >1){
            return false; // there are duplicates in square.
        }
    }

    // verify sum over rows, columns and diagonals.
    ll min_sum = INT_MAX;
    ll max_sum = 0;
    ll max_part_sum = 0;
    // row sum
    for(int i=0;i<3;i++){
        bool skip = false;
        ll sum = 0;
        for(int j=0;j<3;j++){
            sum += square[i][j];
            if(square[i][j] == -1) skip = true;
        }
        if(!skip){
            min_sum = min(min_sum, sum);
            max_sum = max(max_sum, sum);
        }
        else{
            max_part_sum = max(max_part_sum, sum);
        }
    }
    // column sum
    for(int i=0;i<3;i++){
        bool skip = false;
        ll sum = 0;
        for(int j=0;j<3;j++){
            sum += square[j][i];
            if(square[j][i] == -1) skip = true;
        }
        if(!skip){
            min_sum = min(min_sum, sum);
            max_sum = max(max_sum, sum);
        }
        else{
            max_part_sum = max(max_part_sum, sum);
        }
    }
    // diagonal sum
    ll s1 = square[0][0] + square[1][1] + square[2][2];
    ll s2 = square[0][2] + square[1][1] + square[2][0];
    if(square[0][0]!=-1 && square[1][1]!=-1 && square[2][2]!=-1) {
        min_sum = min(min_sum, s1);
        max_sum = max(max_sum, s1);
    }
    else{
        max_part_sum = max(max_part_sum, s1);
    }
    if(square[0][2]!=-1 && square[1][1]!=-1 && square[2][0]!=-1) {
        min_sum = min(min_sum, s2);
        max_sum = max(max_sum, s2);
    }
    else{
        max_part_sum = max(max_part_sum, s2);
    }
    if(min_sum == -1){
        cout<<"not possible condition"<<endl;
        return false;
    }

    if(max_sum > MAX_SQUARE_SUM/3) return false;
    if(min_sum < 15) return false; // minimum sum of row/col can not be less than 15.
    if(max_part_sum > min_sum) return false; // sum of two cells in row/col/dia should not be greater than sum of any row/col/dia.

    
    if(min_sum == INT_MAX && max_sum == 0) return true;
    if(max_sum-min_sum > 1) return false;
    return true;
}

bool isAmSquare(vector<vector<int> > &square){
    bool x = iner_isAmSquare(square);
    // for(auto row:square){
    //     for(int el:row){
    //         cout<<el<<' ';
    //     }
    //     cout<<endl;
    // }
    // cout<<x<<endl<<endl;
    return x;
}

vector<vector<vector<int>>> rec(vector<vector<int>> &square){
    vector<vector<vector<int> > > res;
    // find next index to set;
    int x,y, c = 0;
    for(auto row:square){
        for(int el: row){
            if(el!=-1) c++;
        }
    }
    if(c<2 || c>9){
        cout<<"(c<2 || c>9): Not possible"<<endl;
        return res; // not possible conditions
    }
    if(c == 9) {
        if(isAmSquare(square))
            res.push_back(square);
        return res;
    }
    // elements added in specific order to fail fast.
    if(c == 2) {x = 2;y=0;}
    if(c == 3) {x = 1;y=1;}
    if(c == 4) {x = 2;y=2;}
    if(c == 5) {x = 1;y=2;}
    if(c == 6) {x = 0;y=2;}
    if(c == 7) {x = 0;y=1;}
    if(c == 8) {x = 2;y=1;}

    if(square[x][y] != -1){
        cout<<"not possible condition"<<endl;
        return res; // not possible condition.
    }
    for(int num = RANGE_MIN; num<=RANGE_MAX;num++){
        square[x][y] = num;
        if(isAmSquare(square)){
            vector<vector<vector<int>>> ans = rec(square);
            res.insert(res.end(), ans.begin(), ans.end());
        }
    }
    square[x][y] = -1; // backtracking
    return res;
}

// Returns all possible almost magic squares for given two elements of the square [(0,0), (1,0)].
// Positions of `a` and `b` in square:
// a * *
// b * *
// * * *
vector<vector<vector<int>>> amSquares(int a,int b){
    vector<vector<vector<int> > > res;

    // empty square.
    vector<vector<int> > square(3,vector<int>(3,-1));
    // set `a` and `b`
    square[0][0] = a;
    square[1][0] = b;
    res = rec(square);
    return res;
}

int main(){
	// ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    
    vector<vector<vector<int>>> res;
    #pragma omp parallel for collapse(2)
    for(int i=RANGE_MIN;i<=RANGE_MAX;i++){
        for(int j=RANGE_MIN;j<=RANGE_MAX;j++){
            auto squares = amSquares(i,j);
            if(squares.empty()){
                // cout<<"Not found for "<<i<<' '<<j<<endl;
                continue;
            }
            // cout<<omp_get_thread_num()<<endl;
            // cout<<"Printing almost magic squares for "<<i<<' '<<j<<" total: "<<squares.size()<<endl;
            #pragma omp critical
            {
                res.insert(res.end(),squares.begin(),squares.end());
            }
            
        }
    }
    // cout<<res.size()<<endl;


    // output to file
    ofstream myfile ("output.txt");
    if(!myfile.is_open()){
        cout<<"Unable to open file :("<<endl;
        return 0;
    }
    for(vector<vector<int> > square: res) {
        for(vector<int> row : square){
            for(int elem: row){
                myfile<<elem<<' ';
            }
            myfile<<"\n";
        }
        myfile<<"\n";
    }
    myfile<<"\n";
    myfile.close();
	return 0;
}