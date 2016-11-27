/*
Penny has an array of  integers, . She wants to find the number of unique multisets she can form using elements from the array such that the bitwise XOR of all the elements of the multiset is a prime number. Recall that a multiset is a set which can contain duplicate elements.

Given  queries where each query consists of an array of integers, can you help Penny find and print the number of valid multisets for each array? As these values can be quite large, modulo each answer by  before printing it on a new line.

Input Format

The first line contains a single integer, , denoting the number of queries. The  subsequent lines describe each query in the following format:

The first line contains a single integer, , denoting the number of integers in the array.
The second line contains  space-separated integers describing the respective values of .
Constraints




Output Format

On a new line for each query, print a single integer denoting the number of unique multisets Penny can construct using numbers from the array such that the bitwise XOR of all the multiset's elements is prime. As this value is quite large, your answer must be modulo .

Sample Input

1   
3   
3511 3671 4153  
Sample Output

4
Explanation

The valid multisets are:

 is prime.
  is prime.
   is prime.
   , which is prime.
   Because there are four valid multisets, we print the value of  on a new line.
   */
/*
 *this problem can be solved using dynamic programming and pigeonhole principle.
 Using sieve of Eratosthenes, mark all the primes lying between  to .
 Create a hashmap which stores the count of occurrences of all the array elements.
 Note that since the xor-sum of any subset of array elements will not exceed . 
 Using this property, we can write a  dynamic programming solution with  constant 
 factor such that  would store the count of subsets that can be formed with the first  
 elements such that the xor-sum of the elements in the subset is . 
 * 
 *
 */
#include<bits/stdc++.h>
using namespace std;
#define mod 1000000007

int a[5025];
vector<int> v;
bool prime[9025];
long long mem[2][8192];
void sieve( )   
{
   memset(prime, true, sizeof(prime));
   prime[1]=false, prime[0]=false; 
   for(int i=4;i<=9000;i+=2)
      prime[i]=false;
   for (int p=3; p*p<=9000;p+=2)
   {
      if (prime[p] == true)
      {
         for (int i=p*p; i<=9000; i += 2*p)
            prime[i] = false;
      }
   }

}
int main() {
   //freopen("input2.txt","r",stdin);
   ////freopen("output2.txt","w",stdout);
   clock_t begin, end;
   begin = clock();
   int t;
   sieve();
   cin >> t;
   while(t--) {
      int n;
      cin >> n;
      v.clear();
      memset(a,0,sizeof(a));
      for(int i=0;i<n;i++) {
         int x;
         scanf("%d",&x);
         a[x]+=1;
      }
      for(int i=3500;i<4525;i++)
         if(a[i]>=1)
            v.push_back(i);
      memset(mem,0,sizeof(mem));
      mem[0][0]=1;
      int flag=1;
      int k = v.size();
      for(int i=1;i<=k;i++) {
         for(int j=0;j<8192;j++) {
            mem[flag][j] = (mem[flag^1][j]*(1+(a[v[i-1]])/2))%mod + (mem[flag^1][j^v[i-1]]*((a[v[i-1]]+1)/2))%mod;
            if(mem[flag][j]>=mod)
               mem[flag][j]%=mod;
         }
         flag = flag^1;

      }
      long long ans=0;
      long long res=0;
      for(int i=1;i<8192;i++) {
         if(prime[i]){

            res+= mem[flag^1][i];
            res%=mod;
         }
      }
      cout << res << endl;
   }
   end = clock();
   //cout << ((float) (end) - (float) (begin)) / CLOCKS_PER_SEC << endl;
   fclose(stdout);
   return 0;
}
