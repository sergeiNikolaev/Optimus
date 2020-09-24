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
#ifndef SOFA_TESTING_PARAMS_H
#define SOFA_TESTING_PARAMS_H

#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/defaulttype.h>
#include <sofa/core/objectmodel/BaseObject.h>

#include <string.h>

namespace sofa
{

namespace component
{

namespace misc
{

template <class DataTypes>
struct templateName
{
    std::string operator ()(void) { return("generic"); }
};

template<>
struct templateName<double>
{
    std::string operator ()(void) { return("double"); }
};


template<>
struct templateName<sofa::defaulttype::RigidCoord<3,double> >
{
    std::string operator ()(void) { return("Rigid3d"); }
};


template<>
struct templateName<sofa::defaulttype::RigidCoord<2,double> >
{
    std::string operator ()(void) { return("Rigid2d"); }
};

template<>
struct templateName<sofa::defaulttype::Vec3d>
{
    std::string operator ()(void) { return("Vec3d"); }
};


template<>
struct templateName<sofa::defaulttype::Vec2d>
{
    std::string operator ()(void) { return("Vec2d"); }
};


template<>
struct templateName<sofa::defaulttype::Vec1d>
{
    std::string operator ()(void) { return("Vec1d"); }
};


template <class DataTypes>
class TestingParams: public sofa::core::objectmodel::BaseObject
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(TestingParams,DataTypes), BaseObject);

    typedef sofa::core::objectmodel::BaseObject Inherit;

    Data<DataTypes> initValue;

    TestingParams() : Inherit()
      , initValue( initData(&initValue, "initValue", "initValue"))
    {
    }

    virtual std::string getTemplateName() const
    {
      return templateName(this);
    }

    static std::string templateName(const TestingParams<DataTypes>* = NULL)
    {
        std::string name = sofa::component::misc::templateName<DataTypes>()();
        return(name);
    }

};


}

}

}

#endif
