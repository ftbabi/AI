//
// Created by 邵一迪 on 2019/5/3.
//

#ifndef AICODE_SEARCHTREE_H
#define AICODE_SEARCHTREE_H

#define WAITFOREXP -1
#define EXPEND -2
#define CANTEXP -3

#include <iostream>
#include <cstring>
#include <map>
#include <queue>
#include <vector>
#include <fstream>

using namespace std;

struct Score {
    int gn;
    int hn;
    int fn;
    Score() {
        gn = 0;
        hn = 0;
        fn = 0;
    }
    Score(int g, int h) {
        gn = g;
        hn = h;
        fn = g+h;
    }
    bool operator<(const Score &s) {
        return fn < s.fn;
    }
    bool operator==(const Score &s) {
        return fn == s.fn;
    }
    bool operator>(const Score &s) {
        return fn > s.fn;
    }
};

struct SearchNode {
    int seq; // the seq-th node expanded
    Score score;
    string status; //BBBWWWE
    vector<SearchNode*> child;
    int father_seq; // use for draw, -1 is root
    int expand_state; //-1 wait for expand, -2 expand end, -3 can't expand, other same number
    SearchNode() {
        status = "BBBWWWE";
        seq = 0;
        father_seq = -1;
        expand_state = WAITFOREXP;
    }
    SearchNode(string s, int sequence, Score sor, int pseq=-1) {
        status = s;
        seq = sequence;
        score = sor;
        father_seq = pseq;
        expand_state = WAITFOREXP;
    }
    bool operator<(const SearchNode &n) {
        return score < n.score || (score == n.score && seq < n.seq);
    }
    bool operator==(const SearchNode &n) {
        return (score == n.score) && (seq == n.seq);
    }
    bool operator>(const SearchNode &n) {
        return score > n.score || (score == n.score && seq > n.seq);
    }
};

struct CmpSearchNode {
    bool operator()(SearchNode *a, SearchNode *b) {
        return *a > *b;
    }
};


class SearchTree {
public:
    SearchTree();
    SearchTree(const SearchTree &);
    ~SearchTree();

    int getEmpty(SearchNode *cur);
    int expand(SearchNode *cur);
    void buildTree();
    void show();

private:
    SearchNode *root;
    map<string, int> database;
    priority_queue<SearchNode *, vector<SearchNode *>, CmpSearchNode> open_list; // 差一个比较函数
    queue<SearchNode *> close_list;
    const int status_len = 7;
    const int max_step = 3;
    int seq;
    int ans_seq;

    int stepTry(int center, int range, SearchNode *node);
    void expandEnd(SearchNode *cur);
    SearchNode* createNewNode(SearchNode *cur, string status, Score score);
    SearchNode* createPlaceHolder(SearchNode *cur, string status, Score score, int same_num);
    void deleteTree(SearchNode *node);
    int cal_cost(const string &status, int start, int end);
    int cal_hn(const string &status);
    int is_end(const string &status);
    int findLastW(const string &status);
};


#endif //AICODE_SEARCHTREE_H
