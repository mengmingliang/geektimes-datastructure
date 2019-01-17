class Solution
{
public:
    vector<int> maxWater(vector<int> &a){
        int n = a.size();
        int ans = 0;
        vector<int> maxLeft(n,0);
        vector<int> maxRight(n,0);

        int max_left = a[0];
        for(int i = 1;i < n; ++i){
            max_left = max(max_left,a[i-1]);
            maxLeft[i] = max_left;
        }

        int max_right = a[n-1];
        for(int i = n-1;i >= 0; --i){
            max_right = max(max_right,a[i+1]);
            maxRight[i] = max_right;
        }

        for(int i = 1; i <= n-2; ++i){
            ans += min(maxLeft[i],maxRight[i]) > a[i]?min(maxLeft[i],maxRight[i]) - a[i]:0;
        }
        
        return ans;
    }
}