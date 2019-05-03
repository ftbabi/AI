//
// Created by 邵一迪 on 2019/5/3.
//

#include "SearchTree.h"

SearchTree::SearchTree() {
    root = new SearchNode();
    seq = 0;
    ans_seq = 0;
    root->score.hn = cal_hn(root->status);
    root->score.fn = root->score.hn;
    open_list.push(root);
    database.insert(make_pair(root->status, seq));
    ++seq; // root.seq = 0
}

SearchTree::~SearchTree() {
    this->deleteTree(root);
}

void SearchTree::buildTree() {
    while(!open_list.empty()) {
        SearchNode *cur = open_list.top();
        open_list.pop();
        if(expand(cur)) {
            // 到达目标
            show();
            break;
        }
    }
}

int SearchTree::getEmpty(SearchNode *cur){
    for(int i = 0; i < status_len; ++i) {
        if(cur->status[i] == 'E')
            return i;
    }
    return -1;
}

int SearchTree::expand(SearchNode *cur) {
    int empty = getEmpty(cur);
    int eop = stepTry(empty, max_step, cur);
    expandEnd(cur);
    return eop;
}

int SearchTree::stepTry(int center, int range, SearchNode *node) {
    int down = center-range;
    int up = center+range;
    char ct = node->status[center];
    map<string, int>::iterator it;
    for(int i = down; i <= up; ++i) {
        if(i == center) {
            continue;
        }
        if(i >= 0 && i < status_len) {
            string tmp = node->status;

            tmp[center] = tmp[i];
            tmp[i] = ct;

            Score score(node->score.gn+cal_cost(tmp, i, center), cal_hn(tmp));

            it = database.find(tmp);
            if(it == database.end()) {
                // 新状态
                SearchNode *child = createNewNode(node, tmp, score);
                if(is_end(child->status)) {
                    ans_seq = child->seq;
                    cout << "Find answer node: seq " << child->seq << endl;
                    return 1;
                }
            }
            else {
                // already expanded
                int samenode = it->second;
                cout << "Same: " << samenode << endl;
//                createPlaceHolder(node, tmp, score, samenode);
            }
        }
    }
    if(node->child.empty()) {
        node->expand_state = CANTEXP;
    }
    else{
        node->expand_state = EXPEND;
    }
    return 0;
}

SearchNode* SearchTree::createNewNode(SearchNode *cur, string status, Score score) {
    SearchNode *child = new SearchNode(status, seq, score, cur->seq);
    cur->child.push_back(child);
    open_list.push(child);
    database.insert(make_pair(status, seq));
    ++seq;
    return child;
}

int SearchTree::cal_cost(const string &status, int start, int end) {
    // edge cost
    return abs(end-start);
}

int SearchTree::cal_hn(const string &status) {
    int lastW = findLastW(status);
    int hn = 0;
    if(lastW == (status_len-1)) {
        --lastW;
    }

    // the distance to the right W
    for(int i = 0; i < status_len; ++i) {
        if(status[i] == 'B' && i <= lastW) {
            hn += (lastW-i+1);
        }
    }

    // the number of B in left of W
//    for(int i = 0; i < status_len; ++i) {
//        if(status[i] == 'W') {
//            int count = 0;
//            for(int j = 0; j < i; ++j) {
//                if(status[j] == 'B') {
//                    ++count;
//                }
//            }
//            hn += count;
//        }
//    }

    return hn;
}

void SearchTree::expandEnd(SearchNode *cur) {
    close_list.push(cur);
}

int SearchTree::is_end(const string &status) {
    int lastW = findLastW(status);
    for(int i = 0; i < lastW; ++i) {
        if(status[i] == 'B') {
            return 0;
        }
    }

    return 1;
}

void SearchTree::show() {
    ofstream outfile("searchtree.txt", ios_base::out);
    queue<SearchNode*> candi;
    candi.push(root);
    cout << root->seq << ", "
         << root->status << ", "
         << root->score.gn << "+" << root->score.hn << "=" << root->score.fn << "; "
         << root->father_seq
         << endl;
    outfile << root->seq << ", "
         << root->status << ", "
         << root->score.gn << "+" << root->score.hn << "=" << root->score.fn << "; "
         << root->father_seq
         << "\t";
    while (!candi.empty()) {
        SearchNode* cur = candi.front();
        candi.pop();

        int size = cur->child.size();
        for(int i = 0; i < size; ++i) {
            SearchNode* next = cur->child[i];
            cout << next->seq << ", "
                 << next->status << ", "
                 << next->score.gn << "+" << next->score.hn << "=" << next->score.fn;
            outfile << next->seq << ", "
                    << next->status << ", "
                    << next->score.gn << "+" << next->score.hn << "=" << next->score.fn;
            if(next->expand_state >= 0) {
                cout << ", Same node to " << next->expand_state;
                outfile << ", Same node to " << next->expand_state;
            }
            else if(next->expand_state == CANTEXP) {
                cout << ", Can't expand";
                outfile << ", Can't expand";
            }
            if(ans_seq == next->seq) {
                //answer
                cout << ", Answer!";
                outfile << ", Answer!";
            }
            cout << "; "
                 << next->father_seq
                 << '\t';
            outfile << "; "
                    << next->father_seq
                    << '\t';
            candi.push(next);
        }
        cout << endl;
    }

    outfile.close();
}

int SearchTree::findLastW(const string &status) {
    int lastW = status_len-1;
    for(int i = lastW; i >=0; --i) {
        if(status[i] == 'W') {
            lastW = i;
            break;
        }
    }

    return lastW;
}

SearchNode* SearchTree::createPlaceHolder(SearchNode *cur, string status, Score score, int same_num) {
    SearchNode *child = new SearchNode(status, seq, score, cur->seq);
    child->expand_state = same_num;
    cur->child.push_back(child);
//    open_list.push(child);
//    database.insert(make_pair(status, seq));
    ++seq;
    return child;
}

void SearchTree::deleteTree(SearchNode *node) {
    if(node->child.empty()) {
        delete node;
    }
    else{
        for(int i = 0; i < node->child.size(); ++i) {
            deleteTree(node->child[i]);
        }
        delete node;
    }
}