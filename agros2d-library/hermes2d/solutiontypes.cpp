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

#include "solutiontypes.h"

#include "util/global.h"

#include "scene.h"
#include "field.h"
#include "block.h"
#include "problem.h"
#include "logview.h"

using namespace Hermes::Hermes2D;

FieldSolutionID BlockSolutionID::fieldSolutionID(FieldInfo* fieldInfo)
{
    bool contains = false;
    foreach(Field* field, group->fields())
    {
        if(field->fieldInfo() == fieldInfo)
            contains = true;
    }
    assert(contains);

    return FieldSolutionID(fieldInfo, timeStep, adaptivityStep, solutionMode);
}

QDebug& operator<<(QDebug output, const FieldSolutionID& id)
{
    output << "(" << id.group->name() << ", timeStep " << id.timeStep << ", adaptStep " <<
              id.adaptivityStep << ", type "<< id.solutionMode << ")";
    return output;
}

BlockSolutionID FieldSolutionID::blockSolutionID(Block *block)
{
    assert(block->contains(this->group));
    return BlockSolutionID(block, timeStep, adaptivityStep, solutionMode);
}

QString FieldSolutionID::toString()
{
    QString str = QString("%1_%2_%3_%4").
            arg(group->fieldId()).
            arg(timeStep).
            arg(adaptivityStep).
            arg(solutionTypeToStringKey(solutionMode));

    return str;
}

// *********************************************************************************************

template <typename Scalar>
MultiArray<Scalar>::MultiArray()
{
}

template <typename Scalar>
MultiArray<Scalar>::~MultiArray()
{
    clear();
}

template <typename Scalar>
void MultiArray<Scalar>::clear()
{
    m_solutions.clear();
    m_spaces.clear();
}

template <typename Scalar>
void MultiArray<Scalar>::append(SpaceSharedPtr<Scalar> space, MeshFunctionSharedPtr<Scalar> solution)
{
    m_spaces.push_back(space);
    m_solutions.push_back(solution);
}

template <typename Scalar>
void MultiArray<Scalar>::append(Hermes::vector<SpaceSharedPtr<Scalar> > spaces, Hermes::vector<MeshFunctionSharedPtr<Scalar> > solutions)
{
    assert(spaces.size() == solutions.size());
    for (int i = 0; i < solutions.size(); i++)
        append(spaces.at(i), solutions.at(i));
}

template <typename Scalar>
MultiArray<Scalar> MultiArray<Scalar>::fieldPart(Block *block, FieldInfo *fieldInfo)
{
    assert(block->contains(fieldInfo));
    MultiArray<Scalar> msa;
    int offset = block->offset(block->field(fieldInfo));
    int numSol = fieldInfo->numberOfSolutions();

    for(int i = offset; i < offset + numSol; i++)
    {
        msa.append(m_spaces.at(i), m_solutions.at(i));
    }
    return msa;
}

template <typename Scalar>
void MultiArray<Scalar>::loadFromFile(const QString &baseName, FieldSolutionID solutionID)
{
    // qDebug() << "void MultiSolutionArray<Scalar>::loadFromFile(FieldSolutionID solutionID)" << solutionID.toString();

    clear();

    // load the mesh file
    Hermes::vector<MeshSharedPtr> meshes = Module::readMeshFromFile(QString("%1.mesh").arg(baseName));
    MeshSharedPtr mesh;
    int i = 0;
    foreach (FieldInfo* fieldInfo, Agros2D::problem()->fieldInfos())
    {
        if (fieldInfo == solutionID.group)
        {
            mesh = meshes.at(i);
            break;
        }
        i++;
    }
    assert(mesh);

    FieldInfo *fieldInfo = solutionID.group;
    Block *block = Agros2D::problem()->blockOfField(fieldInfo);
    //block->createBoundaryConditions();

    for (int i = 0; i < solutionID.group->numberOfSolutions(); i++)
    {
        try
        {
            EssentialBCs<double>* essentialBcs = NULL;
            if(fieldInfo->spaces()[i].type() != HERMES_L2_SPACE)
            {
                essentialBcs = block->bcs().at(i + block->offset(block->field(fieldInfo)));
            }

            SpaceSharedPtr<Scalar> space = Space<Scalar>::load(QString("%1_%2.spc").arg(baseName).arg(i).toLatin1().data(), mesh, false, essentialBcs);

            Solution<Scalar> *sln = new Solution<Scalar>();
            sln->set_validation(false);
            sln->load((QString("%1_%2.sln").arg(baseName).arg(i)).toLatin1().data(), space);

            append(space, sln);
        }
        catch (Hermes::Exceptions::Exception &e)
        {
            Agros2D::log()->printError(QObject::tr("Solver"), QString::fromStdString(e.what()));
            throw;
        }
    }
}

template <typename Scalar>
void MultiArray<Scalar>::saveToFile(const QString &baseName, FieldSolutionID solutionID)
{
    // qDebug() << "void MultiSolutionArray<Scalar>::saveToFile(FieldSolutionID solutionID)" << solutionID.toString();

    // QTime time;
    // time.start();

    Hermes::vector<MeshSharedPtr> meshes;
    foreach(FieldInfo* fieldInfo, Agros2D::problem()->fieldInfos())
    {
        if (fieldInfo == solutionID.group)
            meshes.push_back(m_spaces.at(0)->get_mesh());
        else
            meshes.push_back(fieldInfo->initialMesh());
    }

    Module::writeMeshToFile(QString("%1.mesh").arg(baseName), meshes);

    int solutionIndex = 0;
    for (int i = 0; i < m_solutions.size(); i++)
    {
        QString spaceFN = QString("%1_%2.spc").arg(baseName).arg(solutionIndex);
        m_spaces.at(i)->save(spaceFN.toLatin1().data());

        QString solutionFN = QString("%1_%2.sln").arg(baseName).arg(solutionIndex);
        dynamic_cast<Hermes::Hermes2D::Solution<Scalar> *>(m_solutions.at(i).get())->save(solutionFN.toLatin1().data());

        solutionIndex++;
    }

    // qDebug() << "void MultiSolutionArray<Scalar>::loadFromFile(FieldSolutionID solutionID)" << solutionID.toString() << time.elapsed();
}


template class MultiArray<double>;
template class SolutionID<FieldInfo>;