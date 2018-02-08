from DAOptions import DAOptions
import numpy
import types
import math
import sys
import matplotlib.pyplot as plt
from itertools import repeat


def load_matrix_from_file(f):
    if type(f) == types.StringType:
        fo = open(f, 'r')
        matrix = load_matrix_from_file(fo)
        fo.close()
        return matrix
    elif type(f) == types.FileType:
        file_content = f.read().strip()
        file_content = file_content.replace('\r\n', ';')
        file_content = file_content.replace('\n', ';')
        file_content = file_content.replace('\r', ';')

        return numpy.matrix(file_content)

    raise TypeError('f must be a file object or a file name.')


def load_time_data(f):
    fo = open(f, 'r')
    timeLine = []
    # throw out first line
    fo.readline()
    line = fo.readline()
    line = fo.readline()
    while line:
        tokens = line.split()
        startTime = int(tokens[len(tokens) - 1])
        line = fo.readline()
        tokens = line.split()
        endTime = int(tokens[len(tokens) - 1])
        deltaTime = endTime - startTime
        timeLine.append(deltaTime)
        line = fo.readline()
        line = fo.readline()

    return timeLine

try : 
    sys.argv[0]
except :
    commandLineArguments = []
else :
    commandLineArguments = sys.argv
if (len(commandLineArguments) > 1):
    folder=commandLineArguments[1]
else:
    folder = ['outCyl2_2264_UKFSimCorr_forceField2264Verification']
    folder.append('outCyl2_2264_ROUKF_forceField2264Verification')
    folder.append('outCyl2_2264_UKFClassic_forceField2264Verification')

# plot computation time
fig2 = plt.figure(100)
spl2 = fig2.add_subplot(111)

for generalIndex in range (0, len(folder)):

    options = DAOptions()
    options.parseYaml(folder[generalIndex] + '/daconfig.yml')

    stateExpVal = load_matrix_from_file(options.export.stateExpValFile)
    stateVar = load_matrix_from_file(options.export.stateVarFile)


    # plot stiffnesses with variances
    fig1 = plt.figure(generalIndex)
    spl1 = fig1.add_subplot(111)

    nstate=numpy.size(stateVar[1,:])
    nparams=options.filter.nparams
    nsteps=numpy.size(stateVar[:,1])

    print "Number of steps: ", nsteps
    print "Number of parameters: ", nparams
    print "Size of the state: ", nstate


    rng=xrange(0,nsteps)
    rng=[i*options.model.dt for i in rng]

    cmap = plt.cm.get_cmap('hsv', nparams+1)

    groundTruthStr = options.observations.groundTruth
    groundTruthValues = groundTruthStr.split(' ')
    data = numpy.ones(nsteps)

    averageDiff = numpy.zeros(nsteps)

    for i in range(0, nparams):
        si = i +nstate - nparams;
        # print i,' ',si
        if options.filter.transformParams == 'absolute':        
            ev = abs(stateExpVal[:,si])
            var = abs(stateVar[:,si])
        elif options.filter.transformParams == 'exponential':
            ev = numpy.exp(stateExpVal[:,si])
            var = numpy.exp(stateVar[:,si])        
        else:
            ev = stateExpVal[:,si]
            var = stateVar[:,si]        

        stdev = [math.sqrt(x) for x in var]


    
        vll = numpy.squeeze([x - y for x,y in zip(ev, stdev)])
        vlu = numpy.squeeze([x + y for x,y in zip(ev, stdev)])

        spl1.plot(rng, ev, color=cmap(i),  linestyle='solid', linewidth=4)
        spl1.plot(rng, vll, color=cmap(i),  linestyle='dashed', linewidth=4)
        spl1.plot(rng, vlu, color=cmap(i),  linestyle='dashed', linewidth=4)


        # print options.observations.groundTruth
        #, rng, vll, rng, vlu)
    
    
        # print groundTruthValues    
        groundTruthData = [numpy.int(elem) * groundTruthValues[i] for elem in data]
        # plt.plot(rng, groundTruthData, color=cmap(i), linestyle='None', marker=r'$\clubsuit$', markersize=5)
        spl1.plot(rng, groundTruthData, color=cmap(i), linestyle='dotted', linewidth=4)
        # plt.setp(lines, color=cmap(i), linewidth=2.0)

        spl1.set_title('Params '+folder[generalIndex])

        diffVal = numpy.squeeze([abs(x - int(y)) for x,y in zip(ev, groundTruthData)])
        averageDiff = [x + y for x,y in zip(averageDiff, diffVal)]


    averageDiff = [x / nparams for x in averageDiff]

    # extract time result data
    timeVal = load_time_data(options.time.computationTimeFileName)
    timeShift = []
    timeShift.append(timeVal[0])
    for index in range(1, len(timeVal)):
        timeShift.append(timeShift[len(timeShift) - 1] + timeVal[index])

    # convert to milliseconds
    timeShift = [x / 1000 for x in timeShift]

    cmap = plt.cm.get_cmap('hsv', len(folder) + 1)

    #print averageDiff
    spl2.plot(timeShift, averageDiff, color=cmap(generalIndex),  linestyle='solid', label=options.filter.kind)
    spl2.set_title('General computation time ')
    
legend = spl2.legend(loc='upper center', shadow=True, fontsize='x-large')
legend.get_frame().set_facecolor('#FFFFFF')
plt.show()


