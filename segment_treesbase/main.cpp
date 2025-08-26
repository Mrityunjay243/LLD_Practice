#include <bits/stdc++.h>
using namespace std; 

const int MAXV = 400000; 
int t[MAXV]; 

void build(vector<int>& nums, int i, int tl, int tr){
    if (tl==tr){
        t[i] = nums[tl]; 
        return; 
    }
    else {
        int mid = (tl+tr)/2; 
        build(nums, i*2, tl, mid); 
        build(nums, i*2 + 1, mid+1, tr); 
        t[i] = t[i*2] + t[i*2+1];  
    }
}

int sum(int i, int tl, int tr, int l, int r){
    if (l>r) return 0; 
    if (l==tl && r==tr){
        return t[i]; 
    }

    int mid = (tl+tr)/2; 
    return sum(i*2, tl, mid, l, min(mid, r)) + sum(i*2+1, mid+1, tr, max(l, mid+1), r); 
}

void update(int i, int tl, int tr, int pos, int new_val){
    if (tl==tr) t[i] = new_val; 
    else {
        int mid = (tl+tr)/2; 
        if (pos<=mid){
            update(i*2, tl, mid, pos, new_val); 
        } else {
            update(i*2+1, mid+1, tr, pos, new_val); 
        }
        t[i] = t[i*2] + t[i*2+1]; 
    }
}

