/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include "FilteringUpdateEngine.h"


namespace sofa
{
namespace component
{
namespace engine
{


/**
 *        FilteringUpdateEngine::FilteringUpdateEngine
 * <p>
 *   description:
 *       constructor
 * @see none
 *
 *  arguments:
 * @param  none
 *
 * @return  none
 *
 */
template <class DataTypes>
FilteringUpdateEngine<DataTypes>::FilteringUpdateEngine()
    : d_updateData(initData(&d_updateData, "updateData", "flag to update filter state after adding new data"))
{
    this->f_listening.setValue(true);
    d_updateData.setValue(0);
}



/**
 *        FilteringUpdateEngine::init
 * <p>
 *   description:
 *     initialise data
 * @see none
 *
 *  arguments:
 * @param  none
 *
 * @return  none
 *
 */
template <class DataTypes>
void FilteringUpdateEngine<DataTypes>::init() {
    Inherit::init();
    gnode = dynamic_cast<sofa::simulation::Node*>(this->getContext());
}



/**
 *        FilteringUpdateEngine::bwdInit
 * <p>
 *   description:
 *     initialise data
 * @see none
 *
 *  arguments:
 * @param  none
 *
 * @return  none
 *
 */
template <class DataTypes>
void FilteringUpdateEngine<DataTypes>::bwdInit() {
    assert(this->gnode);

    sofa::simulation::Node::SPtr root = dynamic_cast<simulation::Node*>(this->gnode->getRootContext());
    std::cout << "root node: " << root->getName() << std::endl;
    /// get filter
    root->get(filter, core::objectmodel::BaseContext::SearchDown);
    if (filter) {
        std::cout << "found filter: " << filter->getName() << std::endl;
    } else
        std::cout << "no filter found!" << std::endl;

    /// get state wrapper
    root->template get<component::stochastic::StochasticStateWrapperBaseT<DataTypes> >(&stateWrappers, this->getTags(), sofa::core::objectmodel::BaseContext::SearchDown);
    std::cout <<  "found " << stateWrappers.size() << " state wrappers" << std::endl;
    for (size_t i = 0; i < stateWrappers.size(); i++) {
        std::cout << "found stochastic state wrapper: " << stateWrappers[i]->getName() << std::endl;
    }

    /// get optim params (multiple per node, at least one)
    vecOptimParams.clear();
    root->template get<component::container::OptimParamsBase>(&vecOptimParams, core::objectmodel::BaseContext::SearchDown );
    std::cout << "OptimParams found " << vecOptimParams.size() << std::endl;
    for (size_t i = 0; i < vecOptimParams.size(); i++)
        std::cout << "[OptimParams] " << vecOptimParams[i]->getName() << std::endl;
}



/**
 *        FilteringUpdateEngine::handleEvent
 * <p>
 *   description:
 *     perform update operation
 * @see none
 *
 *  arguments:
 * @param  none
 *
 * @return  none
 *
 */
template<class DataTypes>
void FilteringUpdateEngine<DataTypes>::handleEvent(sofa::core::objectmodel::Event* event)
{

    if (sofa::simulation::AnimateBeginEvent::checkEventType(event))
    {
        // if needed search for new planes
        if (d_updateData.getValue() == 1) {
            for (size_t index = 0; index < vecOptimParams.size(); index++) {
                vecOptimParams[index]->appendParameters();
            }
        
            for (size_t index = 0; index < stateWrappers.size(); index++) {
                stateWrappers[index]->updateState();
            }
            filter->updateState();

            d_updateData.setValue(0);
        }
    }
}


} // engine

} // component

} // sofa
