// This file is part of Agros2D.
//
// Agros2D is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Agros2D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Agros2D.  If not, see <http://www.gnu.org/licenses/>.
//
// hp-FEM group (http://hpfem.org/)
// University of Nevada, Reno (UNR) and University of West Bohemia, Pilsen
// Email: agros2d@googlegroups.com, home page: http://hpfem.org/agros2d/

#ifndef FIELD_H
#define FIELD_H

#include "util.h"
#include "value.h"
#include "sceneedge.h"
#include "scenelabel.h"

namespace Module
{
    struct ModuleAgros;
}

class ProblemConfig;
class CouplingInfo;
class LocalForceValue;
class PluginInterface;
class Value;

class AGROS_API FieldInfo : public QObject
{
    Q_OBJECT

public:
    FieldInfo(QString fieldId = "", const AnalysisType analysisType = AnalysisType_Undefined);
    ~FieldInfo();

    void clear();

    inline PluginInterface *plugin() const { assert(m_plugin); return m_plugin; }

    QString fieldId() const { return m_fieldId; }

    inline Hermes::Hermes2D::Mesh *initialMesh() const { return m_initialMesh; }
    inline void clearInitialMesh() { if (m_initialMesh) delete m_initialMesh; m_initialMesh = NULL; }
    void setInitialMesh(Hermes::Hermes2D::Mesh *mesh);

    AnalysisType analysisType() const { return m_analysisType; }
    void setAnalysisType(const AnalysisType analysisType);

    inline LinearityType linearityType() const {return m_linearityType; }
    void setLinearityType(const LinearityType lt) { m_linearityType = lt; emit changed(); }

    // nonlinear settings
    inline int nonlinearSteps() const { return m_nonlinearSteps; }
    void setNonlinearSteps(const int ns) { m_nonlinearSteps = ns; emit changed(); }

    inline double nonlinearTolerance() const { return m_nonlinearTolerance; }
    void setNonlinearTolerance(const double nt) { m_nonlinearTolerance = nt; emit changed(); }

    // Newton settings
    inline bool newtonAutomaticDamping() const {return m_newtonAutomaticDamping; }
    void setNewtonAutomaticDamping(const bool ad) { m_newtonAutomaticDamping = ad; emit changed(); }

    inline double newtonDampingCoeff() const {return m_newtonDampingCoeff; }
    void setNewtonDampingCoeff(const double df) { m_newtonDampingCoeff = df; emit changed(); }

    inline int newtonDampingNumberToIncrease() const {return m_newtonDampingNumberToIncrease; }
    void setNewtonDampingNumberToIncrease(const int df) { m_newtonDampingNumberToIncrease = df; emit changed(); }

    // Picard settings
    inline bool picardAndersonAcceleration() const {return m_picardAndersonAcceleration; }
    void setPicardAndersonAcceleration(const bool ab) { m_picardAndersonAcceleration = ab; emit changed(); }

    inline double picardAndersonBeta() const {return m_picardAndersonBeta; }
    void setPicardAndersonBeta(const double beta) { m_picardAndersonBeta = beta; emit changed(); }

    inline int picardAndersonNumberOfLastVectors() const { return m_picardAndersonNumberOfLastVectors; }
    void setPicardAndersonNumberOfLastVectors(const int nr) {m_picardAndersonNumberOfLastVectors = nr; emit changed(); }

    inline int numberOfRefinements() const { return m_numberOfRefinements; }
    void setNumberOfRefinements(const int nr) {m_numberOfRefinements = nr; emit changed(); }

    const QMap<SceneEdge *, int> edgesRefinement() { return m_edgesRefinement; }
    int edgeRefinement(SceneEdge *edge);
    void setEdgeRefinement(SceneEdge *edge, int refinement) { m_edgesRefinement[edge] = refinement; }
    void removeEdgeRefinement(SceneEdge *edge) { m_edgesRefinement.remove(edge); }

    const QMap<SceneLabel *, int> labelsRefinement() { return m_labelsRefinement; }
    int labelRefinement(SceneLabel *label);
    void setLabelRefinement(SceneLabel *label, int refinement) { m_labelsRefinement[label] = refinement; }
    void removeLabelRefinement(SceneLabel *label) { m_labelsRefinement.remove(label); }

    const QMap<SceneLabel *, int> labelsPolynomialOrder() { return m_labelsPolynomialOrder; }
    int labelPolynomialOrder(SceneLabel *label);
    void setLabelPolynomialOrder(SceneLabel *label, int order) { m_labelsPolynomialOrder[label] = order; }
    void removeLabelPolynomialOrder(SceneLabel *label) { m_labelsPolynomialOrder.remove(label); }

    inline int polynomialOrder() const { return m_polynomialOrder; }
    void setPolynomialOrder(const int po) { m_polynomialOrder = po; emit changed(); }

    inline AdaptivityType adaptivityType() const { return m_adaptivityType; }
    void setAdaptivityType(const AdaptivityType at) { m_adaptivityType = at; emit changed(); }

    inline int adaptivitySteps() const { return m_adaptivitySteps; }
    void setAdaptivitySteps(const int as) { m_adaptivitySteps = as; emit changed(); }

    inline double adaptivityTolerance() const { return m_adaptivityTolerance; }
    void setAdaptivityTolerance(const double at) { m_adaptivityTolerance = at; emit changed(); }

    inline int adaptivityBackSteps() const { return m_adaptivityBackSteps; }
    void setAdaptivityBackSteps(const int abs) { m_adaptivityBackSteps = abs; emit changed(); }

    inline int adaptivityRedoneEach() const { return m_adaptivityRedoneEach; }
    void setAdaptivityRedoneEach(const int re) { m_adaptivityRedoneEach = re; emit changed(); }

    inline Value initialCondition() const { return m_initialCondition; }
    void setInitialCondition(const Value& value) { m_initialCondition = value; emit changed(); }

    inline Value timeSkip() const { return m_timeSkip; }
    void setTimeSkip(const Value& value) { m_timeSkip = value; emit changed(); }

    // refine mesh
    void refineMesh(Hermes::Hermes2D::Mesh *mesh, bool refineGlobal, bool refineTowardsEdge, bool refineArea);

    // name
    QString name() const;

    // description
    QString description() const;

    // deformable shape
    bool hasDeformableShape() const;

    // number of solutions
    inline int numberOfSolutions() const { return m_numberOfSolutions; }

    // latex equation
    QString equation() const;

    // constants
    QMap<QString, double> constants() const;

    // macros
    QMap<QString, QString> macros() const;

    QMap<AnalysisType, QString> analyses() const;

    // spaces
    QMap<int, Module::Space> spaces() const;

    // material type
    QList<Module::MaterialTypeVariable> materialTypeVariables() const;
    // variable by name
    bool materialTypeVariableContains(const QString &id) const;
    Module::MaterialTypeVariable materialTypeVariable(const QString &id) const;

    // boundary conditions
    QList<Module::BoundaryType> boundaryTypes() const;
    // default boundary condition
    Module::BoundaryType boundaryTypeDefault() const;
    // variable by name
    bool boundaryTypeContains(const QString &id) const;
    Module::BoundaryType boundaryType(const QString &id) const;
    Module::BoundaryTypeVariable boundaryTypeVariable(const QString &id) const;

    // force
    Module::Force force() const;

    // material and boundary user interface
    Module::DialogUI materialUI() const;
    Module::DialogUI boundaryUI() const;

    // local point variables
    QList<Module::LocalVariable> localPointVariables() const;
    // view scalar and vector variables
    QList<Module::LocalVariable> viewScalarVariables() const;
    QList<Module::LocalVariable> viewVectorVariables() const;
    // surface integrals
    QList<Module::Integral> surfaceIntegrals() const;
    // volume integrals
    QList<Module::Integral> volumeIntegrals() const;

    // variable by name
    Module::LocalVariable localVariable(const QString &id) const;
    Module::Integral surfaceIntegral(const QString &id) const;
    Module::Integral volumeIntegral(const QString &id) const;

    // default variables
    Module::LocalVariable defaultViewScalarVariable() const;    
    Module::LocalVariable defaultViewVectorVariable() const;

    // weak forms
    QList<FormInfo> wfMatrixVolume() const;
    QList<FormInfo> wfVectorVolume() const;

signals:
    void changed();

private:
    /// plugin
    PluginInterface *m_plugin;

    /// pointer to problem info, whose this object is a "subfield"
    ProblemConfig *m_parent;

    /// unique field info
    QString m_fieldId;

    // initial mesh
    Hermes::Hermes2D::Mesh *m_initialMesh;

    // analysis type
    AnalysisType m_analysisType;
    // number of solutions cache
    int m_numberOfSolutions;

    // linearity
    LinearityType m_linearityType;
    double m_nonlinearTolerance; // percent
    int m_nonlinearSteps;

    // newton
    double m_newtonDampingCoeff;
    bool m_newtonAutomaticDamping;
    int m_newtonDampingNumberToIncrease;

    // picard
    bool m_picardAndersonAcceleration;
    double m_picardAndersonBeta;
    int m_picardAndersonNumberOfLastVectors;

    int m_numberOfRefinements;
    int m_polynomialOrder;

    // TODO: (Franta) gmsh
    QMap<SceneEdge *, int> m_edgesRefinement;
    QMap<SceneLabel *, int> m_labelsRefinement;

    QMap<SceneLabel *, int> m_labelsPolynomialOrder;

    AdaptivityType m_adaptivityType;
    int m_adaptivitySteps;
    double m_adaptivityTolerance; // percent
    int m_adaptivityBackSteps;
    int m_adaptivityRedoneEach;

    // transient
    Value m_initialCondition;
    Value m_timeSkip;
};

ostream& operator<<(ostream& output, FieldInfo& id);

class Field
{
public:
    Field(FieldInfo* fieldInfo);
    bool solveInitVariables();

    FieldInfo* fieldInfo() { return m_fieldInfo; }

    void addCouplingInfo(CouplingInfo *couplingInfo) { m_couplingInfos.append(couplingInfo); }
    QList<CouplingInfo* > couplingInfos() { return m_couplingInfos; }

    // mesh
    void setMeshInitial(Hermes::Hermes2D::Mesh *meshInitial);

private:
    QList<CouplingInfo* > m_couplingInfos;
    FieldInfo* m_fieldInfo;
};

#endif // FIELD_H
