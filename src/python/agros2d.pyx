# from hermes2d import *

from libcpp.vector cimport vector
cdef vector[double] vectd

# script functions
cdef extern from "limits.h":
    int c_INT_MIN "INT_MIN"
    int c_INT_MAX "INT_MAX"
    int c_DOUBLE_MIN "DOUBLE_MIN"
    int c_DOUBLE_MAX "DOUBLE_MAX"

cdef extern from "../pythonlabagros.h":
    # PyParticleTracing
    cdef cppclass PyParticleTracing:
        PyParticleTracing()

        void solve() except +

        int length()
        void x(vector[double] values)

    void pythonMessage(char *str)
    char *pythonVersion()
    char *pythonInput(char *str)
    void pythonQuit()

    char *pythonMeshFileName() except +
    char *pythonSolutionFileName() except +
    # Solution *pythonSolutionObject() except +

    void pythonNewDocument(char *name, char *type, char *physicfield,
                           int numberofrefinements, int polynomialorder, char *adaptivitytype,
                           double adaptivitysteps, double adaptivitytolerance,
                           double frequency,
                           char *analysistype, double timestep, double totaltime, double initialcondition) except +
    void pythonOpenDocument(char *str) except +
    void pythonSaveDocument(char *str) except +
    void pythonCloseDocument()
    
    void pythonAddNode(double x, double y) except +
    void pythonAddEdge(double x1, double y1, double x2, double y2, double angle, char *marker) except +
    void pythonAddLabel(double x, double y, double area, int polynomialOrder, char *marker) except +

    void pythonDeleteNode(int index) except +
    void pythonDeleteNodePoint(double x, double y)
    void pythonDeleteEdge(int index) except +
    void pythonDeleteEdgePoint(double x1, double y1, double x2, double y2, double angle)
    void pythonDeleteLabel(int index) except +
    void pythonDeleteLabelPoint(double x, double y)

    void pythonSelectNone()
    void pythonSelectAll()

    void pythonSelectNodePoint(double x, double y)
    void pythonSelectEdgePoint(double x, double y)
    void pythonSelectLabelPoint(double x, double y)

    void pythonRotateSelection(double x, double y, double angle, int copy)
    void pythonScaleSelection(double x, double y, double scale, int copy)
    void pythonMoveSelection(double dx, double dy, int copy)
    void pythonDeleteSelection()

    void pythonMesh()
    void pythonSolve()

    void pythonZoomBestFit()
    void pythonZoomIn()
    void pythonZoomOut()
    void pythonZoomRegion(double x1, double y1, double x2, double y2)

    void pythonMode(char *str) except +
    void pythonPostprocessorMode(char *str) except +

    void pythonShowScalar(char *type, char *variable, char *component, double rangemin, double rangemax) except +
    void pythonShowGrid(int show)
    void pythonShowGeometry(int show)
    void pythonShowInitialMesh(int show)
    void pythonShowSolutionMesh(int show)
    void pythonShowContours(int show)
    void pythonShowVectors(int show)

    void pythonSetTimeStep(int timestep) except +
    int pythonTimeStepCount()
    void pythonSaveImage(char *str, int w, int h) except +

# particle tracing
cdef class ParticleTracing:
    cdef PyParticleTracing *thisptr

    def __cinit__(self):
        self.thisptr = new PyParticleTracing()
    def __dealloc__(self):
        del self.thisptr

    # solve
    def solve(self):
        self.thisptr.solve()

    def length(self):
        return self.thisptr.length()

    # x
    def x(self):
        out = list()
        cdef vector[double] values
        self.thisptr.x(values)
        for i in range(self.thisptr.length()):
            out.append(values[i])
        return out

    # frequency
    # property frequency:
    #     def __get__(self):
    #         return self.thisptr.getFrequency()
    #     def __set__(self, frequency):
    #         self.thisptr.setFrequency(frequency)

# system

def version():
    return pythonVersion()

def message(char *str):
    pythonMessage(str)

def input(char *str):
    return pythonInput(str)

def meshfilename():
    return pythonMeshFileName()

def solutionfilename():
    return pythonSolutionFileName()

def quit():
    pythonQuit()

# document

def newdocument(char *name, char *type, char *physicfield,
               int numberofrefinements = 0, int polynomialorder = 1, char *adaptivitytype = "disabled",
               double adaptivitysteps = 1, double adaptivitytolerance = 0,
               double frequency = 0,
               char *analysistype = "steadystate", double timestep = 0, double totaltime = 0, double initialcondition = 0):
    pythonNewDocument(name, type, physicfield,
                       numberofrefinements, polynomialorder, adaptivitytype,
                       adaptivitysteps, adaptivitytolerance,
                       frequency,
                       analysistype, timestep, totaltime, initialcondition)

def opendocument(char *str):
    pythonOpenDocument(str)

def savedocument(char *str):
    pythonSaveDocument(str)

def closedocument():
    pythonCloseDocument()

# preprocessor

def addnode(double x, double y):
    pythonAddNode(x, y)

def addedge(double x1, double y1, double x2, double y2, double angle = 0, char *marker = "none"):
    pythonAddEdge(x1, y1, x2, y2, angle, marker)

def addlabel(double x, double y, double area = 0, int polynomialorder = 0, char *marker = "none"):
    pythonAddLabel(x, y, area, polynomialorder, marker)

def deletenode(int index):
    pythonDeleteNode(index)

def deletenodepoint(double x, double y):
    pythonDeleteNodePoint(x, y)

def deleteedge(int index):
    pythonDeleteEdge(index)

def deleteedgepoint(double x1, double y1, double x2, double y2, double angle):
    pythonDeleteEdgePoint(x1, y1, x2, y2, angle)

def deletelabel(int index):
    pythonDeleteLabel(index)

def deletelabelpoint(double x, double y):
    pythonDeleteLabelPoint(x, y)

def selectnone():
    pythonSelectNone()

def selectall():
    pythonSelectAll()

def selectnodepoint(double x, double y):
    pythonSelectNodePoint(x, y)

def selectedgepoint(double x, double y):
    pythonSelectEdgePoint(x, y)
    
def selectlabelpoint(double x, double y):
    pythonSelectLabelPoint(x, y)

def rotateselection(double x, double y, double angle, int copy = False):
    pythonRotateSelection(x, y, angle, int(copy))

def scaleselection(double x, double y, double scale, int copy = False):
    pythonScaleSelection(x, y, scale, int(copy))

def moveselection(double dx, double dy, int copy = False):
    pythonMoveSelection(dx, dy, int(copy))

def deleteselection():
    pythonDeleteSelection()

# solver

def mesh():
    pythonMesh()

def solve():
    pythonSolve()

# postprocessor
    
def zoombestfit():
    pythonZoomBestFit()

def zoomin():
    pythonZoomIn()

def zoomout():
    pythonZoomOut()

def zoomregion(double x1, double y1, double x2, double y2):
    pythonZoomRegion(x1, y1, x2, y2)

def mode(char *str):
    pythonMode(str)

def postprocessormode(char *str):
    pythonPostprocessorMode(str)

def showscalar(char *type, char *variable, char *component, double rangemin = -123456, double rangemax = -123456):
    pythonShowScalar(type, variable, component, rangemin, rangemax)

def showgrid(int show):
    pythonShowGrid(int(show))

def showgeometry(int show):
    pythonShowGeometry(int(show))

def showinitialmesh(int show):
    pythonShowInitialMesh(int(show))

def showsolutionmesh(int show):
    pythonShowSolutionMesh(int(show))

def showcontours(int show):
    pythonShowContours(int(show))

def showvectors(int show):
    pythonShowVectors(int(show))

def timestep(int timestep):
    pythonSetTimeStep(timestep)

def timestepcount():
    return pythonTimeStepCount()

def saveimage(char *str, int w = 0, int h = 0):
    pythonSaveImage(str, w, h)

