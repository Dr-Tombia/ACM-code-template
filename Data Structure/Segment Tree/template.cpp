#include <iostream>
#include <vector>

using namespace std;

// ==========================================
// 1. 全局定义
// ==========================================
const int MAXN = 100005; 
long long a[MAXN];       // 原数组

// 线段树核心数组：大小必须开到原数组的 4 倍！
long long sum[MAXN << 2];  // 维护区间和
long long lazy[MAXN << 2]; // 懒标记 (Lazy Tag)

// 宏定义：利用位运算加速左右儿子的计算，竞赛常用技巧
#define ls (p << 1)       // 左儿子：p * 2
#define rs (p << 1 | 1)   // 右儿子：p * 2 + 1

// ==========================================
// 2. 向上汇报与向下传递 (线段树的灵魂)
// ==========================================

// 向上合并 (Push Up)：用左右儿子的信息更新当前节点 p
inline void push_up(int p) {
    sum[p] = sum[ls] + sum[rs]; // 【根据题目修改】求最大值就换成 max
}

// 向下传递 (Push Down)：把老板(p)压榨的 KPI(懒标记) 下发给两个直系下属(ls, rs)
// l 和 r 是当前节点 p 所管辖的区间边界，mid 是中点
inline void push_down(int p, int l, int r) {
    if (lazy[p] != 0) {
        int mid = l + (r - l) / 2;
        
        // 1. 把懒标记传给左右儿子
        lazy[ls] += lazy[p];
        lazy[rs] += lazy[p];
        
        // 2. 更新左右儿子的真实值
        // 左儿子管辖长度：mid - l + 1
        sum[ls] += lazy[p] * (mid - l + 1); 
        // 右儿子管辖长度：r - mid
        sum[rs] += lazy[p] * (r - mid);     
        
        // 3. 当前节点的懒标记清零 (因为已经传给下属了)
        lazy[p] = 0;
    }
}

// ==========================================
// 3. 核心操作函数
// ==========================================

// 建树 (Build)：初始化线段树
// p: 当前节点编号，l, r: 当前节点管辖的区间
void build(int p, int l, int r) {
    lazy[p] = 0; // 初始化懒标记
    if (l == r) {
        sum[p] = a[l]; // 到了叶子节点，直接赋值
        return;
    }
    int mid = l + (r - l) / 2;
    build(ls, l, mid);       // 递归建左子树
    build(rs, mid + 1, r);   // 递归建右子树
    push_up(p);              // 建完左右子树后，更新自己
}

// 区间修改 (Update)：将区间 [ql, qr] 里的每个数加上 val
// p, l, r 为当前节点信息；ql, qr 为目标区间
void update(int p, int l, int r, int ql, int qr, long long val) {
    // 情况1：当前节点管辖的区间被完全包含在目标区间内
    if (ql <= l && r <= qr) {
        sum[p] += val * (r - l + 1); // 直接更新区间和
        lazy[p] += val;              // 打上懒标记，不再往下递归！(Lazy 核心)
        return;
    }
    
    // 情况2：区间有交集，但没被完全包含，必须把之前的懒标记先下发
    push_down(p, l, r);
    
    int mid = l + (r - l) / 2;
    if (ql <= mid) update(ls, l, mid, ql, qr, val);   // 和左半边有交集
    if (qr > mid)  update(rs, mid + 1, r, ql, qr, val); // 和右半边有交集
    
    push_up(p); // 左右儿子更新完了，最后更新自己
}

// 区间查询 (Query)：查询区间 [ql, qr] 的和
// p, l, r 为当前节点信息；ql, qr 为查询区间
long long query(int p, int l, int r, int ql, int qr) {
    // 情况1：完全包含，直接返回该节点维护的值
    if (ql <= l && r <= qr) {
        return sum[p];
    }
    
    // 情况2：有交集，必须先把懒标记下传，否则查到的儿子数据是过期的！
    push_down(p, l, r);
    
    long long res = 0;
    int mid = l + (r - l) / 2;
    if (ql <= mid) res += query(ls, l, mid, ql, qr);   // 累加左半边的答案
    if (qr > mid)  res += query(rs, mid + 1, r, ql, qr); // 累加右半边的答案
    
    return res;
}

// ==========================================
// 4. 主函数调用示例
// ==========================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m; // n 为数组大小，m 为操作次数
    if (!(cin >> n >> m)) return 0;

    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    // 1. 初始化建树 (根节点编号为1，管辖区间为 [1, n])
    build(1, 1, n);

    while (m--) {
        int op, l, r;
        cin >> op >> l >> r;
        if (op == 1) { // 假设 op==1 是区间加法：将 [l, r] 加上 val
            long long val;
            cin >> val;
            update(1, 1, n, l, r, val);
        } else if (op == 2) { // 假设 op==2 是区间查询：求 [l, r] 的和
            cout << query(1, 1, n, l, r) << "\n";
        }
    }

    return 0;
}
