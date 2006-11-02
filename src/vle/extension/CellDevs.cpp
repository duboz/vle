/** 
 * @file extension/CellDevs.cpp
 * @brief 
 * @author The vle Development Team
 * @date ven, 27 oct 2006 00:05:03 +0200
 */

/*
 * Copyright (C) 2006 - The vle Development Team
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <vle/extension/CellDevs.hpp>
#include <vle/graph/AtomicModel.hpp>
#include <vle/graph/Model.hpp>
#include <vle/graph/Port.hpp>
#include <vle/value/Double.hpp>
#include <vle/value/Integer.hpp>
#include <vle/value/Boolean.hpp>
#include <vle/value/String.hpp>
#include <vle/utils/Exception.hpp>
#include <vle/utils/XML.hpp>

#include <cassert>

using std::map;
using std::pair;
using std::string;
using std::vector;

namespace vle { namespace extension {

using namespace devs;
using namespace utils::xml;

CellDevs::~CellDevs()
{
    std::map < string ,  pair < value::Value* , bool > >::iterator its =
	m_state.begin();

    while (its != m_state.end())
    {
	if (its->second.first) delete its->second.first;
	++its;
    }

    map < string , map < string , value::Value* > >::iterator it =
	m_neighbourState.begin();

    while (it != m_neighbourState.end())
    {
	map < string , value::Value* >::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
	{
	    if (it2->second) delete it2->second;
	    ++it2;
	}
	++it;
    }
}

/***********************************************************************/
//
// un port d'entree par voisin
// une entree du map m_neighbourState par voisin
// le port de sortie se nomme "out"
// le changement d'etat de la cellule est placee sur le port de sortie
//  apres un certain delai
// les evenements de notification des changements d'etat des cellules
//  voisines peuvent ou non etre synchrones
//
/***********************************************************************/

bool
CellDevs::parseXML(xmlpp::Element* p_dynamicsNode)
{
    // PARAMETER node
    xmlpp::Element* v_neighbourhoodNode = get_children(p_dynamicsNode,
						       "NEIGHBOURHOOD");

    if (!v_neighbourhoodNode)
	throw utils::ParseError("Excepted NEIGHBOURHOOD tag.");

    xmlpp::Node::NodeList lst = v_neighbourhoodNode->get_children("PORT");
    xmlpp::Node::NodeList::iterator it = lst.begin();

    while ( it != lst.end() )
    {
	xmlpp::Element * elt = ( xmlpp::Element* )( *it );
	string v_name = get_attribute(elt,"NAME");

	m_neighbourPortList.push_back(v_name);
	++it;
    }
    return true;
}

Time const &
CellDevs::getSigma() const
{
    return m_sigma;
}


void
CellDevs::setSigma(devs::Time const & p_sigma)
{
    m_sigma = p_sigma;
}

Time const &
CellDevs::getLastTime() const
{
    return m_lastTime;
}


void
CellDevs::setLastTime(devs::Time const & p_lastTime)
{
    m_lastTime = p_lastTime;
}

void
CellDevs::hiddenState(std::string const & p_name)
{
    //*** -> Assertion
    assert(m_state.find(p_name) != m_state.end());
    //*** <- Assertion

    value::Value* v_value = getState(p_name);

    m_state[p_name] = pair < value::Value* , bool >(v_value,false);
}


void
CellDevs::initState(std::string const & p_name,
		    value::Value* p_value,
		    bool p_visible)
{
    //*** -> Assertion
    assert(m_state.find(p_name) == m_state.end());
    //*** <- Assertion

    m_stateNameList.push_back(p_name);
    m_state[p_name] = pair < value::Value* , bool >(p_value,p_visible);
    if (p_visible) m_modified = true;
}

void
CellDevs::initDoubleState(std::string const & p_name,
			  double p_value,
			  bool p_visible)
{
    return initState(p_name,new value::Double(p_value),p_visible);
}


void
CellDevs::initIntegerState(std::string const & p_name,
			   long p_value,
			   bool p_visible)
{
    return initState(p_name,new value::Integer(p_value),p_visible);
}


void
CellDevs::initBooleanState(std::string const & p_name,
			   bool p_value,
			   bool p_visible)
{
    return initState(p_name,new value::Boolean(p_value),p_visible);
}


void
CellDevs::initStringState(std::string const & p_name,
			  std::string const & p_value,
			  bool p_visible)
{
    return initState(p_name,new value::String(p_value),p_visible);
}


void
CellDevs::initNeighbourhood(std::string const & p_stateName,
			    value::Value* p_value)
{
    vector < string >::const_iterator it = m_neighbourPortList.begin();

    while (it != m_neighbourPortList.end())
    {
	initNeighbourState(*it,p_stateName,p_value->clone());
	++it;
    }
    delete p_value;
}

void
CellDevs::initDoubleNeighbourhood(std::string const & p_stateName,
				  double p_value)
{
    initNeighbourhood(p_stateName,new value::Double(p_value));
}


void
CellDevs::initIntegerNeighbourhood(std::string const & p_stateName,
				   long p_value)
{
    initNeighbourhood(p_stateName,new value::Integer(p_value));
}


void
CellDevs::initBooleanNeighbourhood(std::string const & p_stateName,
				   bool p_value)
{
    initNeighbourhood(p_stateName,new value::Boolean(p_value));
}

void
CellDevs::initStringNeighbourhood(std::string const & p_stateName,
				  std::string const & p_value)
{
    initNeighbourhood(p_stateName,new value::String(p_value));
}


void
CellDevs::initNeighbourState(std::string const & p_neighbourName,
			     std::string const & p_stateName,
			     value::Value* p_value)
{
    m_neighbourState[p_neighbourName][p_stateName] = p_value;
}

void
CellDevs::initDoubleNeighbourState(std::string const & p_neighbourName,
				   std::string const & p_stateName,
				   double p_value)
{
    initNeighbourState(p_neighbourName,p_stateName,new value::Double(p_value));
}


void
CellDevs::initIntegerNeighbourState(std::string const & p_neighbourName,
				    std::string const & p_stateName,
				    long p_value)
{
    initNeighbourState(p_neighbourName,p_stateName,new value::Integer(p_value));
}

void
CellDevs::initBooleanNeighbourState(std::string const & p_neighbourName,
				    std::string const & p_stateName,
				    bool p_value)
{
    initNeighbourState(p_neighbourName,p_stateName,new value::Boolean(p_value));
}


void
CellDevs::initStringNeighbourState(std::string const & p_neighbourName,
				   std::string const & p_stateName,
				   std::string const & p_value)
{
    initNeighbourState(p_neighbourName,p_stateName,new value::String(p_value));
}


bool
CellDevs::existNeighbourState(std::string const & p_name) const
{
    return m_neighbourState.find(p_name) !=
	m_neighbourState.end();
}


bool
CellDevs::existState(std::string const & p_name) const
{
    vector < string >::const_iterator it = m_stateNameList.begin();

    while (it != m_stateNameList.end())
    {
	if (*it == p_name) return true;
	else ++it;
    }
    return false;
}

bool
CellDevs::isNeighbourEvent(devs::ExternalEvent* event) const
{
    return existNeighbourState(event->getPortName());
}


double
CellDevs::getDelay() const
{
    return m_delay;
}


value::Value*
CellDevs::getState(std::string const & p_name) const
{
    //*** -> Assertion
    assert(existState(p_name));
    assert(m_state.find(p_name) != m_state.end());
    //*** <- Assertion

    return m_state.find(p_name)->second.first;
}


double
CellDevs::getDoubleState(std::string const & p_name) const
{
    return ((value::Double*)getState(p_name))->doubleValue();
}

long
CellDevs::getIntegerState(std::string const & p_name) const
{
    return ((value::Integer*)getState(p_name))->longValue();
}

bool
CellDevs::getBooleanState(std::string const & p_name) const
{
    return ((value::Boolean*)getState(p_name))->boolValue();
}

string CellDevs::getStringState(std::string const & p_name) const
{
    return ((value::String*)getState(p_name))->toString();
}

value::Value*
CellDevs::getNeighbourState(std::string const & p_neighbourName,
			    std::string const & p_stateName) const
{
    //*** -> Assertion
    assert(m_neighbourState.find(p_neighbourName) !=
	   m_neighbourState.end());
    assert(m_neighbourState.find(p_neighbourName)->second.
	   find(p_stateName) != m_neighbourState.
	   find(p_neighbourName)->second.end());
    //*** <- Assertion

    return m_neighbourState.find(p_neighbourName)->second.
	find(p_stateName)->second;
}

double
CellDevs::getDoubleNeighbourState(std::string const & p_neighbourName,
				  std::string const & p_stateName) const
{
    return ((value::Double*)getNeighbourState(p_neighbourName,p_stateName))->
	doubleValue();
}

long
CellDevs::getIntegerNeighbourState(std::string const & p_neighbourName,
				   std::string const & p_stateName) const
{
    return ((value::Integer*)getNeighbourState(p_neighbourName,p_stateName))->
	longValue();
}

bool
CellDevs::getBooleanNeighbourState(std::string const & p_neighbourName,
				   std::string const & p_stateName) const
{
    return ((value::Boolean*)getNeighbourState(p_neighbourName,p_stateName))->
	boolValue();
}

string
CellDevs::getStringNeighbourState(std::string const & p_neighbourName,
				  std::string const & p_stateName) const
{
    return ((value::String*)getNeighbourState(p_neighbourName,p_stateName))->
	toString();
}


unsigned int
CellDevs::getNeighbourStateNumber() const
{
    return m_neighbourState.size();
}

unsigned int
CellDevs::getBooleanNeighbourStateNumber(std::string const & p_stateName,
					 bool p_value) const
{
    //*** -> Assertion
    //*** <- Assertion

    unsigned int v_counter = 0;
    map < string , map < string , value::Value* > >::const_iterator it =
	m_neighbourState.begin();

    while (it != m_neighbourState.end())
    {
	value::Value* v_value = it->second.find(p_stateName)->second;

	if (((value::Boolean*)v_value)->boolValue() == p_value)
	    v_counter++;
	it++;
    }
    return v_counter;
}

unsigned int
CellDevs::getIntegerNeighbourStateNumber(std::string const & p_stateName,
					 long p_value) const
{
    //*** -> Assertion
    //*** <- Assertion

    unsigned int v_counter = 0;
    map < string , map < string , value::Value* > >::const_iterator it =
	m_neighbourState.begin();

    while (it != m_neighbourState.end())
    {
	value::Value* v_value = it->second.find(p_stateName)->second;

	if (((value::Integer*)v_value)->longValue() == p_value)
	    v_counter++;
	it++;
    }
    return v_counter;
}

void
CellDevs::setState(std::string const & p_name,
		   value::Value* p_value)
{
    //*** -> Assertion
    assert(existState(p_name));
    //*** <- Assertion

    std::map < string ,  pair < value::Value* , bool > >::iterator it =
	m_state.find(p_name);
    bool v_visible = it->second.second;

    if (it != m_state.end() && it->second.first != NULL)
	delete it->second.first;
    m_state[p_name] = pair < value::Value* , bool >(p_value,v_visible);
    if (v_visible) m_modified = true;
}


void
CellDevs::setDoubleState(std::string const & p_name,double p_value)
{
    setState(p_name,new value::Double(p_value));
}


void
CellDevs::setIntegerState(std::string const & p_name,long p_value)
{
    setState(p_name,new value::Integer(p_value));
}


void
CellDevs::setBooleanState(std::string const & p_name,bool p_value)
{
    setState(p_name,new value::Boolean(p_value));
}


void
CellDevs::setStringState(std::string const & p_name,
			 std::string const & p_value)
{
    setState(p_name,new value::String(p_value));
}

void
CellDevs::setNeighbourState(std::string const & p_neighbourName,
			    std::string const & p_stateName,
			    value::Value* p_value)
{
    //*** -> Assertion
    assert(m_neighbourState.find(p_neighbourName) !=
	   m_neighbourState.end());
    //*** <- Assertion

    std::map < string , value::Value* >& v_state = m_neighbourState.
	find(p_neighbourName)->second;
    std::map < string , value::Value* >::iterator it = v_state.find(p_stateName);

    if (it != v_state.end() && it->second != NULL)
	delete it->second;
    m_neighbourState[p_neighbourName][p_stateName] = p_value;
}

//***********************************************************************
//***********************************************************************
//
//  DEVS Methods
//
//***********************************************************************
//***********************************************************************

devs::Time
CellDevs::init()
{
  setLastTime(Time(0));
  m_neighbourModified = false;
  return Time::infinity;
}

devs::Time
CellDevs::getTimeAdvance()
{
    return m_sigma;
}


ExternalEventList*
CellDevs::getOutputFunction(devs::Time const & time)
{
    if (m_modified)
    {

//	std::cout << time.getValue() << " - [out] : " << getModelName() << std::endl;

	ExternalEventList* v_eventList=new ExternalEventList();
	ExternalEvent* e=new ExternalEvent("out",
					   time,
					   getModel());
	map < string , pair < value::Value* , bool > >::const_iterator it =
	    m_state.begin();

	while (it != m_state.end())
	{
	    if (it->second.second)
	      e->putAttribute(it->first,it->second.first->clone());
	    //	      e << attribute(it->first,it->second.first->clone());
	    ++it;
	}
	v_eventList->addEvent(e);
	m_modified = false;
	return v_eventList;
    }
    else return noEvent();
}


void
CellDevs::processExternalEvent(devs::ExternalEvent* event)
{
    string v_portName = event->getPortName();

//     std::cout << event->getTime().getValue() << " " << getModelName()
//	      << ":" << v_portName<< " -> ext" << std::endl;

    if (existNeighbourState(v_portName))
    {

//	std::cout << "ok" << std::endl;

	map < string , value::Value* >::const_iterator it =
	    m_neighbourState[v_portName].begin();

	while (it != m_neighbourState[v_portName].end())
	{
	    string v_name = it->first;
	    value::Value* v_value = event->getAttributeValue(v_name)->clone();

//	    std::cout << "ext : " << event->getTime().getValue()
//		      << " " << getModelName() << ":" << v_portName
//		      << " = " << v_value->toString()
//		      << std::endl;

	    setNeighbourState(v_portName,v_name,v_value);
	    ++it;
	}
	m_neighbourModified = true;
	updateSigma(event);
    }
    else // c'est une perturbation
	processPerturbation(event);
}


void
CellDevs::processInitEvent(devs::InitEvent* event)
{
    string v_name = event->getPortName();
    value::Value* v_value = event->getAttributeValue(v_name);

//     cout << "init[" << getModelName() << ":" << v_name
//	 << "] = " << v_value->toString() << endl;

    initState(v_name,v_value->clone());
}


value::Value*
CellDevs::processStateEvent(devs::StateEvent* event) const
{

//     cout << event->getTime().getValue() << ":" << getModelName()
//	  << "->" << event->getPortName() << " = "
//	 << getState(event->getPortName())->toString()
//	  << endl;

    if (existState(event->getPortName()))
	return getState(event->getPortName())->clone();
    else return 0;
}

}} // namespace vle extension
