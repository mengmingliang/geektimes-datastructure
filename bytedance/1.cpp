class Solution
{
public:
    vector<int> splitCut(vector<vector<int>> &points,int N){
        vector<vector<int>> ans;
        map<int,vector<int>> mapX;
        map<int,vector<int>> mapY;
        int circumference = 0;
        int n = points.size();

        /*sort*/
        for(auto e : points){
            mapX[e[0]].push_back(e[1]);
            mapX[e[1]].push_back(e[0]);
        }
        for(auto it = mapX.begin(); it != mapX.end(); ++it){
            sort(it->second.begin(),it->second.end());
        }
        for(auto it = mapY.begin(); it != mapY.end(); ++it){
            sort(it->second.begin(),it->second.end());
        }
        
        for(int i = 1;i < n; ++i){
            if(points[i][0] == points[i-1][0]){
                circumference += abs(points[i][0] - points[i-1][0]);
            }else{
                circumference += abs(points[i][1] - points[i-1][1]);
            }
        }

        if(points[n-1][0] == points[0][0]){
            circumference += abs(points[n-1][0] - points[0][0]);
        }else{
            circumference += abs(points[n-1][1] - points[0][1]);
        }

        int divd = circumference/n;
        int start = 0;

        for(int i = 1;i < n; ++i){
            if(points[i][0] == points[i-1][0]){
                start += abs(points[i][0] - points[i-1][0]);
            }else{
                start += abs(points[i][1] - points[i-1][1]);
            }


            
        }

        return ans;    
    }
}