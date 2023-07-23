#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>

const int INF = 1e7; // Updated to a larger value
int w, d;

std::vector<int> gen(int state) {
    int s0 = (state - 1) & state;
    std::vector<int> result;
    while (s0 != state) {
        if (s0 < state) {
            result.push_back(s0);
        }
        s0 = (s0 - 1) & state;
    }
    return result;
}

int minimum_steiner_tree(int N, std::vector<std::vector<std::pair<int, int>>>& G,
    int L, std::vector<int>& V) {

    std::vector<std::vector<int>> E =
        std::vector<std::vector<int>>(N, std::vector<int>(N, INF));

    for (int v = 0; v < N; v++) {
        for (int g = 0; g < G[v].size(); g++) {
            std::tie(w, d) = G[v][g];
            E[v][w] = d;
        }
        E[v][v] = 0; // Set diagonal elements to 0
    }

    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                E[i][j] = std::min(E[i][j], E[i][k] + E[k][j]);
            }
        }
    }

    std::vector<std::vector<int>> S =
        std::vector<std::vector<int>>(N, std::vector<int>(1 << L, INF));

    for (int i = 0; i < L; i++) {
        int state = 1 << i;
        for (int v = 0; v < N; v++) {
            S[v][state] = E[v][V[i]];
        }
        S[V[i]][state] = 0;
    }

    for (int i = 0; i < N; i++) {
        S[i][0] = 0;
    }

    std::vector<int> us = std::vector<int>(N, INF);

    for (int state = 1; state < (1 << L); state++) {
        std::vector<int> ss = gen(state);
        us.assign(N, INF); // Initialize us with INF values
        for (int v = 0; v < N; v++) {
            int u = INF;
            for (int s = 0; s < ss.size(); s++) {
                int s0 = ss[s];
                u = std::min(u, S[v][s0] + S[v][s0 ^ state]);
            }
            for (int i = 0; i < N; i++) {
                us[i] = std::min(us[i], u + E[v][i]);
            }
        }
        for (int w = 0; w < N; w++) {
            S[w][state] = us[w];
        }
    }

    int ALL = (1 << L) - 1;
    int ans = INF;
    for (int v = 0; v < N; v++) {
        ans = std::min(ans, S[v][ALL]);
    }
    return ans;
}

int minPseudoPolyominoSize(const std::vector<std::vector<std::vector<int>>>& target) {
    int N = target.size() * target[0].size() * target[0][0].size();
    std::vector<std::vector<std::pair<int, int>>> G =
        std::vector<std::vector<std::pair<int, int>>>(N);
    for (int i = 0; i < target.size(); i++) {
        for (int j = 0; j < target[0].size(); j++) {
            for (int k = 0; k < target[0][0].size(); k++) {
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        for (int z = -1; z <= 1; z++) {
                            if (x == 0 && y == 0 && z == 0) {
                                continue;
                            }
                            if (i + x < 0 || i + x >= target.size()) {
                                continue;
                            }
                            if (j + y < 0 || j + y >= target[0].size()) {
                                continue;
                            }
                            if (k + z < 0 || k + z >= target[0][0].size()) {
                                continue;
                            }
                            G[i * target[0].size() * target[0][0].size() +
                                j * target[0][0].size() + k]
                                .push_back(std::make_pair(
                                    (i + x) * target[0].size() * target[0][0].size() +
                                    (j + y) * target[0][0].size() + (k + z),
                                    1));
                        }
                    }
                }
            }
        }
    }

    std::vector<int> V;
    for (int i = 0; i < target.size(); i++) {
        for (int j = 0; j < target[0].size(); j++) {
            for (int k = 0; k < target[0][0].size(); k++) {
                if (target[i][j][k] == 1) {
                    V.push_back(i * target[0].size() * target[0][0].size() +
                        j * target[0][0].size() + k);
                }
            }
        }
    }
    int L = V.size();
    int ans;
    if (L < 60) {
        ans = minimum_steiner_tree(N, G, L, V);
    }
    else {
        ans = L;
    }
    //std::cout << ans << std::endl;
    return 0;
}
