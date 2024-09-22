#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define FIX 5

int K, M;
int map[FIX][FIX];

int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, 1, -1, 0};

vector<int> artifact_nums;
vector<int> results;

vector<vector<int>> all_rotation_choice;

int GetValue(int& map[FIX][FIX], int center_y, int center_x) {
    bool visited[FIX][FIX] = {false};
    queue<pair<int, int>> q;
    int value = 0;

    q.push({y,x});
    visited[y][x] = true;

    while(!q.empty()) {
        int cnt = 0;

        int y = q.front().first;
        int x = q.front().second;
        q.pop();
        
        for(int i=0;i<4;i++) {
            int ny = y + dy[i];
            int nx = x + dx[i];

            if(ny<0 || ny>=FIX || nx<0 || nx>=FIX)
                continue;

            if(!visited[ny][nx] && map[ny][nx] == map[y][x]) {
                q.push({ny, nx});
                visited[ny][nx] = true;
                cnt++;
            }
        }

        if(cnt>=3) {
            value+=cnt;
        }
    }
    return value;
}

int RotateClockwise90(int center_y, int center_x) {
    int temp_map[FIX][FIX];
    for(int i=0;i<FIX;i++) {
        for(int j=0;j<FIX;j++) {
            temp_map[i][j] = map[i][j];
        }
    }

    for(int i=center_y-1;i<center_y+2;i++) {
        for(int j=center_x-1;j<center_x+2;j++) {
            temp_map[i][j] = map[j][FIX-1-i];
        }
    }
    int value = GetValue(temp_map, center_y, center_x);
    return value;
}

int RotateClockwise180(int center_y, int center_x) {
    int temp_map[FIX][FIX];

    for(int i=center_y-1;i<center_y+2;i++) {
        for(int j=center_x-1;j<center_x+2;j++) {
            temp_map[i][j] = map[FIX-1-j][FIX-1-j];
        }
    }


    int value = GetValue(temp_map, center_y, center_x);
    return value;
}

int RotateClockwise270() {
    int temp_map[FIX][FIX];

    for(int i=center_y; i<center_y+2;i++) {
        for(int j=center_x-1; j<center_x+2; j++) {
            temp_map[i][j] = map[FIX-1-j][i];
        }
    }

    return value;
}

bool cmp(const vector<int> &a, const vector<int>&b) {
    if(a[0]!=b[0]) {
        return a[0] > b[0]; 
    }

    if(a[1]!=b[1]) {
        return a[1] < b[1];
    }

    if(a[2]!=b[2]) {
        return a[2] < b[2];
    }
    else {
        return a[3] < b[3];
    }

}

bool StartExploration() {
    bool flag = false;

    all_rotation_choices.clear();

    for(int i=1;i<FIX-1;i++){
        for(int j=1;j<FIX-1;j++) {
            int value_90 = RotateClockwise90(i, j);
            all_rotation_choices.push_back({value, 90, i, j});
            int value_180 = RotateClockwise180(i, j);
            all_rotation_choices.push_back({value, 180, i, j});
            int value_270 = RotateClockwise270(i, j);
            all_rotation_choicespush_back({value, 270, i, j});

            // 조각이 3개이상 연결되는게 1개라도 있는 경우
            if(value_90 != 0 || value_180 != 0 || value_270 != 0) {
                flag = true;
            }
        }
    }
    
    sort(all_rotation_choices.begin(), all_rotation_choices.end(), cmp);
    
    return flag;
}

int GetArtifact() {
    vector<int> best_choice = all_rotation_choices[0];
    
    if(best_choice[1] == 90) {
        RotateClockwise90(best_choice[2], best_choice[3]);
    }

    RemoveArtifact();
    CreateNewArtifact();

    return value;
}

bool Game() {
    if(!StartExploration()) {
        return false; 
    }
    
    while(true) {
        int value = GetArtifact();
        if(value == 0) {
            break;
        }
    }

    results.push_back(value);
}

int main() {
    cin>>K>>M;
    for(int i=0;i<FIX;i++) {
        for(int j=0;j<FIX;j++) {
            cin>>map[i][j];
        }
    }
    
    for(int i=0;i<M;i++) {
        int num;
        cin>>num;
        artifact_nums.push_back(num);
    }

    for(int i=0;i<K;i++) {
        if(!Game()) {
            break;
        }
    }

    for(auto a: results) {
        cout<<a<<" ";
    }

    return 0;
}