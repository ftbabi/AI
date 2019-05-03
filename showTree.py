import sys
import re
from graphviz import Digraph


def draw(filepath):
    g = Digraph()
    with open(filepath, "r") as f:
        line = f.readline()
        dots = line.split('\t')
        for dot in dots:
            if dot:
                info = dot.split(';')
                curnum = int(info[0].split(',')[0])
                g.node(str(curnum), info[0])
                father = int(info[1])
                if father >= 0:
                    g.edge(str(curnum), str(father))
    g.view()

if __name__ == '__main__':
    # g = Digraph()
    # g.node('a')
    # g.node('b')
    # g.edge('a', 'b')
    #
    # g.view()
    draw("searchtree.txt")