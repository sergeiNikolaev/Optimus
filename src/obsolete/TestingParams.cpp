/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program. If not, see <http://www.gnu.org/licenses/>.              *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include "TestingParams.h"
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace misc
{

using namespace sofa::defaulttype;

SOFA_DECL_CLASS(TestingParams)

// Register in the Factory
int TestingParamsClass = core::RegisterObject("Optimization Parameters")
#ifndef SOFA_FLOAT
        .add< TestingParams<Vec3d> >(true)
        .add< TestingParams<Vec2d> >()
        .add< TestingParams<Vec1d> >()

        .add< TestingParams<RigidCoord<3, double> > >()
        .add< TestingParams<RigidCoord<2, double> > >()

        .add<TestingParams<double> >()
#endif
#ifndef SOFA_DOUBLE

#endif
;

#ifndef SOFA_FLOAT
template class  TestingParams<Vec3d>;
template class  TestingParams<Vec2d>;
template class  TestingParams<Vec1d>;

template class  TestingParams<RigidCoord<3, double> >;
template class  TestingParams<RigidCoord<2, double> >;

template class TestingParams<double>;
#endif
#ifndef SOFA_DOUBLE


#endif

} // namespace misc

} // namespace simulation

} // namespace sofa
