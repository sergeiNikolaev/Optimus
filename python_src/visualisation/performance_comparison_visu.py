import sys
import os
import numpy
import types
import math
import json
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from itertools import repeat
import pygraphviz as PG

# select the visual style for images
plt.style.use('classic')
#print(plt.style.available)

inputList = ['cyl3gravity_Euler1/ROUKF_obs33_0_proj2']
inputList.append('cyl3gravity_Euler1/ROUKF_obs33_1_proj2')


only_leaves_performance = 0

def create_tree(node, nodeName, graph, nodesList):
    leafNode = 1
    if not isinstance(node, float) and len(node) > 0:
        for item in node.keys():
            if item == "end_time" or item == "start_time":
                continue
            ##### fix due to empty nodes in result file #####
            if len(item) < 1:
                continue
            ##### fix due to empty nodes in result file #####
            graph.add_edge(nodeName, item)
            leafNode = 0
            create_tree(node[item], item, graph, nodesList)
    if only_leaves_performance == 0 or leafNode == 1:
        nodesList.append(nodeName)


def compute_performance(node, nodeName, nodesList, executedTime):
    if not isinstance(node, float) and len(node) > 0:
        for item in node.keys():
            if item == "start_time" or len(item) < 1:
                continue
            if item == "end_time":
                for index in range(0, len(nodesList)):
                    if nodesList[index] == nodeName:
                        executedTime[index] = executedTime[index] + float(node['end_time']) - float(node['start_time'])
            else:
                compute_performance(node[item], item, nodesList, executedTime)



try : 
    sys.argv[0]
except :
    commandLineArguments = []
else :
    commandLineArguments = sys.argv

if (len(commandLineArguments) > 1):
    folder = commandLineArguments[1]
else:
    folder = inputList

print("Command line arguments for python : " + str(commandLineArguments))

for generalIndex in range (0, len(folder)):

    dataFile = folder[generalIndex] + '/computationTime.txt' 
    with open(dataFile, 'r') as stream:
        try:
            statistics = json.load(stream)

        except statistics.JSONError as exc:
            print(exc)
            sys.exit()


    ### create a tree of dependences
    fullNode = statistics['1']['records']
    insertionsGraph = PG.AGraph(directed=True, strict=True)
    nodesList = []

    create_tree(fullNode, 'TOTAL', insertionsGraph, nodesList)

    # pygraphviz renders graphs in neato by default, 
    # so you need to specify dot as the layout engine
    insertionsGraph.layout(prog='dot')

    # save image with graph
    insertionsGraph.draw('insertions' + str(generalIndex) + '.png')

    # load image and show it
    img = mpimg.imread('insertions' + str(generalIndex) + '.png')
    fig1 = plt.figure(2 * generalIndex)
    spl1 = fig1.add_subplot(111)
    imgplot = spl1.imshow(img)
    spl1.set_title('Graph of nested timesteps')


    ### create histogramm of performance
    executedTime = numpy.zeros(len(nodesList))
    for index in range(1, len(statistics) + 1):
        fullNode = statistics[str(index)]['records']
        compute_performance(fullNode, 'TOTAL', nodesList, executedTime)


    fig2 = plt.figure(2 * generalIndex + 1)
    spl2 = fig2.add_subplot(111)
    amount = numpy.arange(len(executedTime))
    width = 1
    rects = spl2.bar(amount, executedTime, width, color='r')

    # Label the functions below  bars
    for index in range(0, len(rects)):
        rect = rects[index]
        height = rect.get_height()
        position = numpy.array((rect.get_x(), -1.05))
        trans_angle = plt.gca().transData.transform_angles(numpy.array((90,)), position.reshape((1, 2)))[0]
        spl2.text(rect.get_x() + rect.get_width() / 2.0, 0, nodesList[index], rotation=trans_angle, rotation_mode='anchor', ha='center', va='bottom')

    spl2.set_ylabel('general time, ms', fontsize=40)
    spl2.set_title('Computational time after ' + str(len(statistics)) + ' iterations')


# Give ourselves some more room at the bottom of the plot
plt.subplots_adjust(bottom=0.15)
plt.show()
