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

#ifndef UTIL_H
#define UTIL_H

#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QDomDocument>
#include <QtWebKit>
#include <QtXmlPatterns>
#include <QtPlugin>

#include "hermes_common.h"
#include "hermes2d.h"

#include <typeinfo>
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>

#include <locale.h>
#include <stdlib.h>
#include <common.h>

#include "value.h"
#include "util/point.h"

#include "indicators/indicators.h"

// zero
#define EPS_ZERO 1e-10

// physical constants
#define EPS0 8.854e-12
#define MU0 4*M_PI*1e-7
#define SIGMA0 5.670373e-8
#define PRESSURE_MIN_AIR 20e-6
#define GRAVITATIONAL_ACCELERATION 9.81

#define deg2rad(degrees) (degrees*M_PI/180.0)
#define rad2deg(radians) (radians*180.0/M_PI)

using namespace std;

// sign function
inline int sign(double arg)
{
    if (arg > 0)
        return 1;
    else if (arg < 0)
        return -1;
    else
        return 0;
}

inline Hermes::Ord sign(Hermes::Ord arg) { return Hermes::Ord(); }

// show page
void showPage(const QString &str = "");

// set gui style
void setGUIStyle(const QString &styleName);

// set language
void setLanguage(const QString &locale);

// get available languages
QStringList availableLanguages();

// get icon with respect to actual theme
QIcon icon(const QString &name);

// get datadir
QString datadir();

// get external js functions
QString externalFunctions();

// get temp dir
QString tempProblemDir();

// get temp filename
QString tempProblemFileName();

// convert time in ms to QTime
QTime milisecondsToTime(int ms);

// remove directory content
bool removeDirectory(const QDir &dir);

// sleep function
void msleep(unsigned long msecs);

// read file content
QByteArray readFileContentByteArray(const QString &fileName);
QString readFileContent(const QString &fileName);

// write content into the file
void writeStringContent(const QString &fileName, QString content);
void writeStringContent(const QString &fileName, QString *content);
void writeStringContentByteArray(const QString &fileName, QByteArray content);

// append to the file
void appendToFile(const QString &fileName, const QString &str);

// join version
inline QString versionString(int major, int minor, int sub, int git, int year, int month, int day, bool beta)
{
    return QString("%1.%2.%3.%4 %5 (%6-%7-%8)")
            .arg(major)
            .arg(minor)
            .arg(sub)
            .arg(git)
            .arg(beta ? "beta" : "")
            .arg(year)
            .arg(QString("0%1").arg(month).right(2))
            .arg(QString("0%1").arg(day).right(2));
}

// dirty html unit replace
QString unitToHTML(const QString &str);

class AgrosException
{
public:
    AgrosException(const QString &what)
        : m_what(what)
    {
    }

    inline QString what() { return m_what; }
    inline QString toString(){ return m_what; }

protected:
    QString m_what;
};

struct ScriptResult
{
    ScriptResult()
    {
        text = "";
        traceback = "";
        isError = false;
        this->line = -1;
    }

    ScriptResult(const QString &text, const QString &traceback, bool isError = false, int line = -1)
    {
        this->text = text;
        this->traceback = traceback;
        this->isError = isError;
        this->line = line;
    }

    QString text;
    QString traceback;
    bool isError;
    int line;
};

struct ExpressionResult
{
    ExpressionResult()
    {
        this->error = "";
        this->traceback = "";
        this->value = 0.0;
    }

    ExpressionResult(double value, const QString &traceback, const QString &error)
    {
        this->error = error;
        this->traceback = traceback;
        this->value = value;
    }

    QString error;
    QString traceback;
    double value;
};

enum ErrorResultType
{
    ErrorResultType_None,
    ErrorResultType_Information,
    ErrorResultType_Warning,
    ErrorResultType_Critical
};

class ErrorResult
{
public:
    inline ErrorResultType type() { return m_type; }
    inline QString message() { return m_message; }

    inline ErrorResult()
    {
        m_type = ErrorResultType_None;
        m_message = "";
    }

    inline ErrorResult(ErrorResultType type, QString message)
    {
        m_type = type;
        m_message = message;
    }

    inline bool isError() { return (m_type != ErrorResultType_None); }

    void showDialog()
    {
        switch (m_type)
        {
        case ErrorResultType_None:
            return;
        case ErrorResultType_Information:
            QMessageBox::information(QApplication::activeWindow(), QObject::tr("Information"), m_message);
            break;
        case ErrorResultType_Warning:
            QMessageBox::warning(QApplication::activeWindow(), QObject::tr("Warning"), m_message);
            break;
        case ErrorResultType_Critical:
            QMessageBox::critical(QApplication::activeWindow(), QObject::tr("Critical"), m_message);
            break;
        }
    }

private:
    ErrorResultType m_type;
    QString m_message;
};

enum CoordinateType
{
    CoordinateType_Undefined,
    CoordinateType_Planar,
    CoordinateType_Axisymmetric
};

enum AnalysisType
{
    AnalysisType_Undefined,
    AnalysisType_SteadyState,
    AnalysisType_Transient,
    AnalysisType_Harmonic
};

enum AdaptivityType
{
    AdaptivityType_Undefined = 1000,
    AdaptivityType_None = 3,
    AdaptivityType_H = 1,
    AdaptivityType_P = 2,
    AdaptivityType_HP = 0
};

enum TimeStepMethod
{
    TimeStepMethod_Fixed,
    TimeStepMethod_BDF2
};

enum LinearityType
{
    LinearityType_Undefined,
    LinearityType_Linear,
    LinearityType_Picard,
    LinearityType_Newton
};

enum CouplingType
{
    CouplingType_Undefined,
    CouplingType_None,
    CouplingType_Weak,
    CouplingType_Hard
};

enum WeakFormKind
{
    WeakForm_MatVol,
    WeakForm_MatSurf,
    WeakForm_VecVol,
    WeakForm_VecSurf,
    WeakForm_ExactSol
};

enum MeshType
{
    MeshType_Triangle,
    MeshType_Triangle_QuadFineDivision,
    MeshType_Triangle_QuadRoughDivision,
    MeshType_Triangle_QuadJoin,
    MeshType_GMSH_Triangle,
    MeshType_GMSH_Quad,
    MeshType_GMSH_QuadDelaunay_Experimental
};

enum PhysicFieldVariableComp
{
    PhysicFieldVariableComp_Undefined,
    PhysicFieldVariableComp_Scalar,
    PhysicFieldVariableComp_Magnitude,
    PhysicFieldVariableComp_X,
    PhysicFieldVariableComp_Y
};

enum SceneGeometryMode
{
    SceneGeometryMode_OperateOnNodes,
    SceneGeometryMode_OperateOnEdges,
    SceneGeometryMode_OperateOnLabels
};

enum MouseSceneMode
{
    MouseSceneMode_Nothing,
    MouseSceneMode_Pan,
    MouseSceneMode_Rotate,
    MouseSceneMode_Move,
    MouseSceneMode_Add
};

enum SceneModePostprocessor
{
    SceneModePostprocessor_LocalValue,
    SceneModePostprocessor_SurfaceIntegral,
    SceneModePostprocessor_VolumeIntegral
};

enum PaletteType
{
    Palette_Agros2D,
    Palette_Jet,
    Palette_Copper,
    Palette_Hot,
    Palette_Cool,
    Palette_Bone,
    Palette_Pink,
    Palette_Spring,
    Palette_Summer,
    Palette_Autumn,
    Palette_Winter,
    Palette_HSV,
    Palette_BWAsc,
    Palette_BWDesc
};

enum PaletteQuality
{
    PaletteQuality_ExtremelyCoarse,
    PaletteQuality_ExtraCoarse,
    PaletteQuality_Coarser,
    PaletteQuality_Coarse,
    PaletteQuality_Normal,
    PaletteQuality_Fine,
    PaletteQuality_Finer,
    PaletteQuality_ExtraFine
};

enum PaletteOrderType
{
    PaletteOrder_Hermes,
    PaletteOrder_Jet,
    PaletteOrder_Copper,
    PaletteOrder_Hot,
    PaletteOrder_Cool,
    PaletteOrder_Bone,
    PaletteOrder_Pink,
    PaletteOrder_Spring,
    PaletteOrder_Summer,
    PaletteOrder_Autumn,
    PaletteOrder_Winter,
    PaletteOrder_HSV,
    PaletteOrder_BWAsc,
    PaletteOrder_BWDesc
};

enum SceneViewPost3DMode
{
    //SceneViewPost3DMode_Undefined,
    SceneViewPost3DMode_None,
    SceneViewPost3DMode_ScalarView3D,
    SceneViewPost3DMode_ScalarView3DSolid,
    SceneViewPost3DMode_Model,
    SceneViewPost3DMode_ParticleTracing
};

enum SceneTransformMode
{
    SceneTransformMode_Translate,
    SceneTransformMode_Rotate,
    SceneTransformMode_Scale
};

enum SolutionMode
{
    SolutionMode_Normal,
    SolutionMode_Reference,
    SolutionMode_NonExisting,
    SolutionMode_Finer  // used to choose reference if exists, normal otherwise
};

QString stringListToString(const QStringList &list);

// keys
void initLists();

// error norm
QString errorNormString(Hermes::Hermes2D::ProjNormType projNormType);

// coordinate type
QString coordinateTypeString(CoordinateType coordinateType);
QStringList coordinateTypeStringKeys();
QString coordinateTypeToStringKey(CoordinateType coordinateType);
CoordinateType coordinateTypeFromStringKey(const QString &coordinateType);

// analysis type
QString analysisTypeString(AnalysisType analysisType);
QStringList analysisTypeStringKeys();
QString analysisTypeToStringKey(AnalysisType analysisType);
AnalysisType analysisTypeFromStringKey(const QString &analysisType);

// coupling type
QString couplingTypeString(CouplingType couplingType);
QStringList couplingTypeStringKeys();
QString couplingTypeToStringKey(CouplingType couplingType);
CouplingType couplingTypeFromStringKey(const QString &couplingType);

// weakform type
QString weakFormString(WeakFormKind weakForm);
QStringList weakFormStringKeys();
QString weakFormToStringKey(WeakFormKind weakForm);
WeakFormKind weakFormFromStringKey(const QString &weakForm);

// mesh type
QString meshTypeString(MeshType meshType);
QStringList meshTypeStringKeys();
QString meshTypeToStringKey(MeshType meshType);
MeshType meshTypeFromStringKey(const QString &meshType);

// physic field variable component
QString physicFieldVariableCompString(PhysicFieldVariableComp physicFieldVariableComp);
QStringList physicFieldVariableCompTypeStringKeys();
QString physicFieldVariableCompToStringKey(PhysicFieldVariableComp physicFieldVariableComp);
PhysicFieldVariableComp physicFieldVariableCompFromStringKey(const QString &physicFieldVariableComp);

// adaptivity type
QString adaptivityTypeString(AdaptivityType adaptivityType);
QStringList adaptivityTypeStringKeys();
QString adaptivityTypeToStringKey(AdaptivityType adaptivityType);
AdaptivityType adaptivityTypeFromStringKey(const QString &adaptivityType);

// time step method
QString timeStepMethodString(TimeStepMethod timeStepMethod);
QStringList timeStepMethodStringKeys();
QString timeStepMethodToStringKey(TimeStepMethod timeStepMethod);
TimeStepMethod timeStepMethodFromStringKey(const QString &timeStepMethod);

// solution mode
QString solutionTypeString(SolutionMode solutionMode);
QStringList solutionTypeStringKeys();
QString solutionTypeToStringKey(SolutionMode solutionType);
SolutionMode solutionTypeFromStringKey(const QString &solutionType);

// matrix solver type
QString matrixSolverTypeString(Hermes::MatrixSolverType matrixSolverType);
QStringList matrixSolverTypeStringKeys();
QString matrixSolverTypeToStringKey(Hermes::MatrixSolverType matrixSolverType);
Hermes::MatrixSolverType matrixSolverTypeFromStringKey(const QString &matrixSolverType);

// space type
QStringList spaceTypeStringKeys();
QString spaceTypeToStringKey(Hermes::Hermes2D::SpaceType spaceType);
Hermes::Hermes2D::SpaceType spaceTypeFromStringKey(const QString &spaceType);

// linearity type
QString linearityTypeString(LinearityType linearityType);
QStringList linearityTypeStringKeys();
QString linearityTypeToStringKey(LinearityType linearityType);
LinearityType linearityTypeFromStringKey(const QString &linearityType);

// scene view 3d mode
QStringList sceneViewPost3DModeStringKeys();
QString sceneViewPost3DModeToStringKey(SceneViewPost3DMode sceneViewPost3DMode);
SceneViewPost3DMode sceneViewPost3DModeFromStringKey(const QString &sceneViewPost3DMode);

// palette type
QStringList paletteTypeStringKeys();
QString paletteTypeToStringKey(PaletteType paletteType);
PaletteType paletteTypeFromStringKey(const QString &paletteType);
QString paletteTypeString(PaletteType paletteType);

// palette quality
QStringList paletteQualityStringKeys();
QString paletteQualityToStringKey(PaletteQuality paletteQuality);
PaletteQuality paletteQualityFromStringKey(const QString &quality);
double paletteQualityToDouble(PaletteQuality paletteQuality);
QString paletteQualityString(PaletteQuality paletteQuality);

// palette order
QStringList paletteOrderTypeStringKeys();
QString paletteOrderTypeToStringKey(PaletteOrderType paletteType);
PaletteOrderType paletteOrderTypeFromStringKey(const QString &paletteType);

#endif // UTIL_H
