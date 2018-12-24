#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

using para = std::pair<int, int>;

struct Tree {
    int numer;
    Tree *lson;
    Tree *rson;
    Tree *father;
    std::vector<para> requests;

    Tree(int n, Tree *father) {
        this->numer = n;
        this->lson = nullptr;
        this->rson = nullptr;
        this->father = father;
    }


};

void create_tree(int *values, Tree *tree, std::vector<Tree*> &pointers) {
    if (tree != nullptr) {
        int current = (tree->numer-1)*2;
        if (values[current] != -1) {
            tree->lson = new Tree(values[current], tree);
            if ((values[current]-1) == 3) {
            }
            pointers[values[current]-1] = tree->lson;
        }
        if (values[current+1] != -1) {
            tree->rson = new Tree(values[current+1], tree);
            pointers[values[current+1]-1] = tree->rson;
        }
        create_tree(values, tree->rson, pointers);
        create_tree(values, tree->lson, pointers);
    }
}

int *read_tree(int n) {
    n *= 2;
    int *result = new int[n];
    for (int i = 0; i < n; i++)
        std::cin >> result[i];
    return result;
}

void delete_tree(Tree *tree) {
    if (tree != nullptr) {
        delete_tree(tree->rson);
        delete_tree(tree->lson);
        delete tree;
    }
}



void add_request(int index, int distance, Tree *node) {
    if (node != nullptr) {
        node->requests.push_back(std::make_pair(distance, index));
    }
}

para give_max(para a, para b, para c) {
    if (a.first >= b.first) {
        if (a.first >= c.first)
            return a;
        else
            return c;
    }
    else {
        if (b.first >= c.first)
            return b;
        else
            return c;
    }
}

para dfs(Tree *tree, int current, bool *visited, Tree *previous) {
    if (tree != nullptr && !visited[tree->numer-1]) {
        visited[tree->numer-1] = true;
        para left = dfs(tree->lson, current+1, visited, tree);
        para right = dfs(tree->rson, current+1, visited, tree);
        para up = dfs(tree->father, current+1, visited, tree);
        return give_max(left, right, up);
    }
    else {
        if (previous != nullptr)
            return std::make_pair(current-1, previous->numer);
        else
            return std::make_pair(-1, -1);
    }
}

void make_false(bool *visited, int n) {
    for (int j = 0; j < n; j++) {
        visited[j] = false;
    }
}

void algorithm(Tree *tree, std::vector<int> &stack, int *requests, bool *visited) {
    if (tree != nullptr && !visited[tree->numer-1]) {
        visited[tree->numer-1] = true;
        stack.push_back(tree->numer);
        if (!tree->requests.empty()) {
            for (para &v : tree->requests) {
                if (v.first < stack.size()) {
                    requests[v.second] = stack[stack.size() - v.first -1 ];
                }
            }
        }
        algorithm(tree->lson, stack, requests, visited);
        algorithm(tree->rson, stack, requests, visited);
        algorithm(tree->father, stack, requests, visited);
        stack.erase(stack.begin() + stack.size()-1);
    }
}
int main()
{
    std::ios_base::sync_with_stdio(0);
      std::cin.tie(0);
    int n;
    std::cin >> n;
    int *values = read_tree(n);
    std::vector<Tree*> pointers(n);
    Tree *tree = new Tree(1, nullptr);
    pointers[0] = tree;
    create_tree(values, tree, pointers);
    int m;
    std::cin >> m;
    int *requests = new int[m];
    int i, node, distance;
    for (i = 0; i < m; i++) {
        requests[i] = -1;
        std::cin >> node;
        std::cin >> distance;
        add_request(i, distance, pointers[node-1]);
    }

    bool *visited = new bool[n];
    make_false(visited, n);
    para test = dfs(tree, 0, visited, nullptr);
    make_false(visited, n);
    para test2 = dfs(pointers[test.second-1], 0, visited, nullptr);
    make_false(visited, n);
    std::vector<int> stack;
    algorithm(pointers[test.second-1], stack, requests, visited);
    make_false(visited, n);
    algorithm(pointers[test2.second-1], stack, requests, visited);
    for (int k = 0; k < m; k++) {
        std::cout << requests[k] << '\n';
    }





    delete_tree(tree);
    delete [] values;
    delete [] requests;
    delete [] visited;

}
