import Sofa
import math
import os
import sys
import csv


# Class definition 
class synth1_BCDA(Sofa.PythonScriptController):
    
    def createGlobalComponents(self, node):
        # scene global stuff
        node.createObject('RequiredPlugin', pluginName='Optimus', name='Optimus')
        node.createObject('RequiredPlugin', pluginName='SofaPardisoSolver', name='SofaPardisoSolver')
        node.createObject('RequiredPlugin', pluginName='ImageMeshAux', name='ImageMeshAux')
        node.createObject('RequiredPlugin', pluginName='SofaMJEDFEM')
        # node.createObject('RequiredPlugin', pluginName='image', name='image')
        
        node.findData('gravity').value="0 0 0"
        node.findData('dt').value="0.02"
        
        node.createObject('ViewerSetting', cameraMode='Perspective', resolution='1000 700', objectPickingMethod='Ray casting')
        node.createObject('VisualStyle', name='VisualStyle', displayFlags='showBehaviorModels showForceFields showCollisionModels')

        node.createObject('FilteringAnimationLoop', name="StochAnimLoop", verbose="1")        
        self.filter = node.createObject('ROUKFilter', name="ROUKF", sigmaPointType="simplex", verbose="1")        
            
        node.createObject('MeshVTKLoader', name='objectLoader', filename=self.volumeVTK)
        node.createObject('MeshSTLLoader', name='objectSLoader', filename=self.surfaceSTL)
        node.createObject('MeshVTKLoader', name='obsLoader', filename=self.obsVTK)
        node.createObject('MeshVTKLoader', name='toolLoader', filename=self.toolVTK)                

        return 0
        


    #components common for both master and slave: the simulation itself (without observations and visualizations)
    def createCommonComponents(self, node):                                  
        #node.createObject('StaticSolver', applyIncrementFactor="0")
        #node.createObject('SparsePARDISOSolver')        

        node.createObject('NewtonStaticSolver', name="NewtonStatic", printLog="0", correctionTolerance="1e-8", residualTolerance="1e-8", convergeOnResidual="1", maxIt="2")   
        # node.createObject('StepPCGLinearSolver', name="StepPCG", iterations="10000", tolerance="1e-12", preconditioners="precond", verbose="1", precondOnTimeStep="1")
        node.createObject('SparsePARDISOSolver', name="precond", symmetric="1", exportDataToDir="", iterativeSolverNumbering="0")

        node.createObject('MechanicalObject', src="@/objectLoader", name="Volume")
        node.createObject('TetrahedronSetTopologyContainer', name="Container", src="@/objectLoader", tags=" ")
        node.createObject('TetrahedronSetTopologyModifier', name="Modifier")        
        node.createObject('TetrahedronSetTopologyAlgorithms', name="TopoAlgo")
        node.createObject('TetrahedronSetGeometryAlgorithms', name="GeomAlgo")
        node.createObject('UniformMass', totalMass="0.2513")

        # node.createObject('BoxROI', name='fixedBox1', box='-0.001 -0.001 -0.011 0.011 0.001 0.001', drawBoxes='1')
        node.createObject('BoxROI', name='fixedBox1', box='-0.001 -0.001 -0.011 0.101 0.001 0.001', drawBoxes='1')
        # node.createObject('BoxROI', name="fixedBox1", box="-0.001 0.052 -0.001   0.1631 0.054 0.0131")        
        self.optimParams = node.createObject('OptimParams', name="springStiffness", template="Vector", numParams="@fixedBox1.nbIndices",initValue="0", stdev="40", transformParams="1", optimize="1", printLog="1")
        node.createObject('ExtendedRestShapeSpringForceField', name="fixedSpring", points="@fixedBox1.indices", stiffness="@springStiffness.value", springThickness="3", listening="1", updateStiffness="1", printLog="0")
        node.createObject('ColorMap',colorScheme="Blue to Red")                                                                  
        node.createObject('MJEDTetrahedralForceField', name='FEM', materialName='StVenantKirchhoff', ParameterSet=self.materialParams)
        # node.createObject('TetrahedronFEMForceField', name="FEM", listening="true", updateStiffness="1", youngModulus="1e5", poissonRatio="0.45", method="large")


        toolEmu = node.createChild('toolEmu')        
        toolEmu.createObject('MechanicalObject',name="MO",src="@/toolLoader")
        toolEmu.createObject('SimulatedStateObservationSource', name="ToolA",printLog="1", monitorPrefix=self.toolMonitorPrefix,drawSize="0.0015",controllerMode="1")
        
        node.createObject('Mapped3DoFForceField', mappedFEM="mappedTool/toolSpring", mappedMechObject="mappedTool/MO", mapping="mappedTool/baryMapping", printLog="0")        
        toolMapped = node.createChild('mappedTool');
        toolMapped.createObject('MechanicalObject',name="MO",src="@/toolLoader")
        self.toolSprings=toolMapped.createObject('ExtendedRestShapeSpringForceField', name="toolSpring", stiffness="1e5", external_rest_shape="../toolEmu/MO", springThickness="1", listening="1", updateStiffness="1", springColor="0 1 0 1",startTimeSpringOn="0",numStepsSpringOn="10000")
        toolMapped.createObject('ColorMap',colorScheme="Blue to Red")                                                                  
        toolMapped.createObject('Sphere',radius="0.002",color="0 0 1 1") 
        toolMapped.createObject('BarycentricMapping',name="baryMapping")

        return 0



    def createMasterScene(self, node):
        node.createObject('StochasticStateWrapper',name="StateWrapper",verbose="1")
        
        self.createCommonComponents(node)

        obsNode = node.createChild('obsNode')
        obsNode.createObject('MechanicalObject', name='MO', src="@/obsLoader")        
        obsNode.createObject('Sphere', radius="0.001", color="0.2 0.8 0.2 1")
        obsNode.createObject('BarycentricMapping')                   
        obsNode.createObject('MappedStateObservationManager', name="MOBS", observationStdev="1e-4", listening="1", stateWrapper="@../StateWrapper",doNotMapObservations="1",verbose="1")
        obsNode.createObject('SimulatedStateObservationSource', name="ObsSource", monitorPrefix=self.obsMonitorPrefix, drawSize="0.001")
    
        visNode = node.createChild('ObjectVisualization')
        visNode.createObject('MechanicalObject',src="@/objectSLoader",name="Surface")
        visNode.createObject('TriangleSetTopologyContainer', name="Container", src="@/objectSLoader", tags=" ")
        visNode.createObject('TriangleSetTopologyModifier', name="Modifier")        
        visNode.createObject('TriangleSetTopologyAlgorithms', name="TopoAlgo")
        visNode.createObject('TriangleSetGeometryAlgorithms', name="GeomAlgo")        
        visNode.createObject('Line',color="0 0 0 1")
        visNode.createObject('Triangle',color="1 0 0 1")
        visNode.createObject('BarycentricMapping')
        #visNode.createObject('VTKExporter',filename="vtkExp/beam",XMLformat="true",listening="true",edges="0",triangles="1",quads="0",tetras="0",exportAtBegin="1",exportAtEnd="0",exportEveryNumberOfSteps="1")

        obsVisuNode = node.createChild('ObservationVisualization')
        obsVisuNode.createObject('MechanicalObject', name="aux_Source", position="@../obsNode/MOBS.observations")
        obsVisuNode.createObject('Sphere', radius="0.002", color="0.2 0.8 0.2 1")

        return 0
 
 

    def createSlaveScene(self, node):
        node.createObject('VisualStyle', name='VisualStyle', displayFlags='hideBehaviorModels hideForceFields hideCollisionModels')
        wrapper=node.createObject('StochasticStateWrapper',name="StateWrapper",verbose="1")
        wrapper.findData("name").value = "StochasticWrapperSlave"
        wrapper.findData("slave").value = 1;        
        
        self.createCommonComponents(node)        
        self.m_slaveScenesCreated+=1

        return 0

    

    def createScene(self,node):
        r_slaves = [] # list of created auxiliary nodes
        self.createGlobalComponents(node)
                
        masterNode=node.createChild('MasterScene')
        self.createMasterScene(masterNode)        
        masterNode.createObject('VisualStyle', name='VisualStyle', displayFlags='showBehaviorModels')
        
        slaveSubordinate=node.createChild('SlaveSubordinate')
        for i in range(1,self.m_slaveSceneCount):
            slave=slaveSubordinate.createChild('SlaveScene_'+str(i))
            #slave.createObject('VisualStyle', name='VisualStyle', displayFlags='hideAll')
            self.createSlaveScene(slave)
            r_slaves.append(slave)        
            
        return 0


               
    def createGraph(self,node):          
        self.cameraReactivated=False
        self.rootNode=node              
         
        print  "Create graph called (Python side)\n"

        E=7500
        nu=0.45
        lamb=(E*nu)/((1+nu)*(1-2*nu))
        mu=E/(2+2*nu)
        self.materialParams='{} {}'.format(mu,lamb)
        
        inputDir='observations/brickD_ogrid4/'
        self.volumeVTK=inputDir+'object_0.vtk'
        self.surfaceSTL='../../data/brickD/brickD_536.stl'
        self.obsVTK=inputDir+'observations_0.vtk'
        self.toolVTK=inputDir+'tool_0.vtk'

        self.obsMonitorPrefix=inputDir+'observations'
        self.toolMonitorPrefix=inputDir+'tool'

        self.m_sigmaPointType = 'simplex'
        self.m_slaveSceneCount = 4
        self.m_saveToFile = 0
        self.m_slaveScenesCreated = 0 # auxiliary, circumvents attribute of instance method

        self.saveState = 1;

        if self.saveState:
            self.stateExpFile='out/state_SD40par.txt'
            self.stateVarFile='out/variance_SD40par.txt'
            self.stateCovarFile='out/covariance_SD40par.txt'
            os.system('rm '+self.stateExpFile)
            os.system('rm '+self.stateVarFile)
            os.system('rm '+self.stateCovarFile)

        

        self.createScene(node)
        
        return 0

    #def draw(self):
    #    print 'Draw!!!!'

    def initGraph(self,node):
        print 'Init graph called (python side)'
        self.step    =     0
        self.total_time =     0
        
        # self.process.initializationObjects(node)
        return 0

    def onEndAnimationStep(self, deltaTime):  

        if self.saveState:              
            rs=self.filter.findData('reducedState').value
            reducedState = [val for sublist in rs for val in sublist]
            #print 'Reduced state:'
            #print reducedState

            f1 = open(self.stateExpFile, "a")        
            f1.write(" ".join(map(lambda x: str(x), reducedState)))
            f1.write('\n')
            f1.close()    
                    
            rv=self.filter.findData('reducedVariance').value
            reducedVariance = [val for sublist in rv for val in sublist]
            #print 'Reduced variance:'
            #print reducedVariance

            f2 = open(self.stateVarFile, "a")        
            f2.write(" ".join(map(lambda x: str(x), reducedVariance)))
            f2.write('\n')
            f2.close()

            rcv=self.filter.findData('reducedCovariance').value
            reducedCovariance = [val for sublist in rcv for val in sublist]
            #print 'Reduced Covariance:'
            #print reducedCovariance

            f3 = open(self.stateCovarFile, "a")
            f3.write(" ".join(map(lambda x: str(x), reducedCovariance)))
            f3.write('\n')
            f3.close()

        tsp=self.toolSprings.findData('totalForce').value
        print 'Tool forces:'
        print tsp

        return 0

    def onScriptEvent(self, senderNode, eventName,data):        
        return 0;

