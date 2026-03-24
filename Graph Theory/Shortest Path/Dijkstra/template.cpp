#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int INF = 1e9; // 用一个很大的数表示无穷大

// 定义边的结构体
struct Edge {
    int to;      // 目标节点
    int weight;  // 边的权重
};

// 邻接表的定义：graph[u] 存储了从 u 出发的所有边 {v, w}
vector<vector<pair<int, int>>> graph(n + 1);
for (const auto& edge : times) {
    int u = edge[0];
    int v = edge[1];
    int w = edge[2];
    graph[u].push_back({v, w}); // 有向图
    // graph[v].push_back({u, w});  无向图
}
// 邻接表也是经常使用存图的一种结构
void dijkstra(int start, int n, const vector<vector<Edge>>& graph) {
    // 1. 初始化距离数组，全部赋为无穷大
    vector<int> dist(n, INF);
    dist[start] = 0;

    // 2. 定义优先队列 (最小堆)。
    // 存储的元素是 pair<当前最短距离, 节点编号>
    // greater<pair<int,int>> 确保距离最小的元素永远在队头
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    // 把起点压入队列
    pq.push({0, start});

    while (!pq.empty()) {
        // 取出当前距离最小的节点
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // 优化：如果取出的距离大于当前已知的最短距离，说明这是一个过时的状态，直接跳过
        if (d > dist[u]) continue;

        // 3. 遍历节点 u 的所有邻居进行“松弛”
        for (const auto& edge : graph[u]) {
            int v = edge.to;
            int weight = edge.weight;

            // 如果通过 u 走到 v 的距离更短
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight; // 更新最短距离
                pq.push({dist[v], v});      // 将新的更优状态压入队列
            }
        }
    }

    // 打印结果
    cout << "从起点 " << start << " 到各节点的最短距离：\n";
    for (int i = 0; i < n; ++i) {
        if (dist[i] == INF) cout << "节点 " << i << " : 无法到达\n";
        else cout << "节点 " << i << " : " << dist[i] << "\n";
    }
}
