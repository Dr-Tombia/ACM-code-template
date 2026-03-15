#include <bits/stdc++.h>
using namespace std;

// ==========================================
// 1. 全局变量与数据定义 (根据题目要求修改)
// ==========================================
const int MAXN = 200005; // 数组最大长度
const int MAXM = 200005; // 查询最大数量

int n, m;
int a[MAXN];             // 原数组
int ans[MAXM];           // 记录每个查询的最终答案
int current_ans = 0;     // 当前维护的区间答案

// 维护区间状态需要的数据结构 (比如频率数组 cnt 等)
int cnt[MAXN]; 

// ==========================================
// 2. 莫队查询结构体 (固定写法，直接背诵/复制)
// ==========================================
int block_size; // 分块大小

struct Query {
    int l, r, id;
    
    // 排序逻辑：奇偶分块优化 (核心精华，千万别写错)
    bool operator<(const Query& other) const {
        int blk1 = l / block_size;
        int blk2 = other.l / block_size;
        if (blk1 != blk2) {
            return blk1 < blk2; // 第一关键字：按左端点所在的块升序
        }
        // 第二关键字：奇数块右端点升序，偶数块右端点降序 (蛇形走位)
        return (blk1 & 1) ? r < other.r : r > other.r;
    }
};

vector<Query> queries;

// ==========================================
// 3. 状态转移函数 (做题时 90% 的精力花在这里)
// ==========================================

// 将元素 x 加入当前区间
inline void add(int x) {
    // 【修改这里】加入元素后，对 current_ans 产生什么影响？
    /* 例如：
    if (cnt[x] == 0) current_ans++; 
    cnt[x]++;
    */
}

// 将元素 x 移出当前区间
inline void del(int x) {
    // 【修改这里】移出元素后，对 current_ans 产生什么影响？
    /* 例如：
    cnt[x]--;
    if (cnt[x] == 0) current_ans--;
    */
}

// ==========================================
// 4. 主函数逻辑 (基本不需要大改)
// ==========================================
int main() {
    // 1. 优化输入输出
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 2. 读入原数组
    if (!(cin >> n >> m)) return 0;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    // 3. 动态计算最优分块大小 (防被卡常的小技巧)
    // 通常用 sqrt(n)，但如果是 n 和 m 跨度比较大的题，n / sqrt(m) 理论上跑得最快
    block_size = max(1, (int)(n / sqrt(m))); 
    
    // 4. 读入所有查询
    queries.resize(m);
    for (int i = 0; i < m; ++i) {
        cin >> queries[i].l >> queries[i].r;
        queries[i].id = i;
    }

    // 5. 离线排序查询
    sort(queries.begin(), queries.end());

    // 6. 核心：莫队指针移动
    int L = 1, R = 0; // 初始为一个“空区间”
    
    for (int i = 0; i < m; ++i) {
        int ql = queries[i].l;
        int qr = queries[i].r;

        // 【避坑指南】移动指针的四个 while 循环顺序极度重要！
        // 原则：先扩大区间 (add)，再缩小区间 (del)，防止 L > R 导致数组越界出错。
        
        // 扩大区间：L 左移，R 右移
        while (L > ql) add(a[--L]); 
        while (R < qr) add(a[++R]); 
        
        // 缩小区间：L 右移，R 左移
        while (L < ql) del(a[L++]); 
        while (R > qr) del(a[R--]); 

        // 7. 记录当前查询结果
        ans[queries[i].id] = current_ans;
    }

    // 8. 按原始顺序输出答案
    for (int i = 0; i < m; ++i) {
        cout << ans[i] << "\n";
    }

    return 0;
}
