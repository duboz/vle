/**
 * @file devs/InstantaneousEvent.hpp
 * @author The VLE Development Team.
 * @brief Instantaneous event based on the devs::ExternalEvent class and are
 * build by graph::Model to precess Instantaneous event.
 */

/*
 * Copyright (c) 2005 The VLE Development Team.
 *
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef DEVS_INSTANTANEOUSEVENT_HPP
#define DEVS_INSTANTANEOUSEVENT_HPP

#include <vle/devs/ExternalEvent.hpp>

namespace vle { namespace devs {

    /**
     * @brief Instantaneous event based on the devs::ExternalEvent class and are
     * build by graph::Model to precess Instantaneous event.
     *
     */
    class InstantaneousEvent : public ExternalEvent
    {
    public:
	InstantaneousEvent(const std::string& sourcePortName,
                           const Time& time,
                           sAtomicModel* source) :
            ExternalEvent(sourcePortName, time, source)
        { }
            
	InstantaneousEvent(ExternalEvent* event,
                           sAtomicModel* target,
                           const std::string& targetPortName) :
            ExternalEvent(event, target, targetPortName)
        { }

	virtual ~InstantaneousEvent()
        { }

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        virtual bool isInstantaneous() const;
    };

}} // namespace vle devs

#endif
