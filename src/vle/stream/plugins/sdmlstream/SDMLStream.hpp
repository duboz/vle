/**
 * @file SDMLStream.hpp
 * @author The VLE Development Team.
 * @brief Base class of net visualisation plugin.
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

#ifndef OBSERVER_SDML_STREAM_HPP
#define OBSERVER_SDML_STREAM_HPP

#include <vle/devs/Stream.hpp>
#include <vle/value/Value.hpp>
#include <fstream>
#include <string>



namespace vle { namespace stream {

    /**
     * Observer to store VLE output into a SDML stream.
     *
     */
    class SDMLStream : public devs::Stream
    {
    public:
	SDMLStream();
	virtual ~SDMLStream() { }

        virtual void init(const std::string& outputPlugin,
                          const std::string& outputType,
                          const std::string& outputLocation,
                          const std::string& parameters);

	virtual void close();
	virtual void writeData();
        virtual void writeHead(const std::vector < devs::Observable >&
                               variableNameList);
	virtual void writeTail();
        virtual void writeValue(const devs::Time& time,
                                value::Value value);

	virtual void writeValues(const devs::Time& time,
				 const devs::StreamModelPortValue& valuelst,
                                 const devs::ObservableList& obslst);

    private:
	std::string m_fileName;
	std::ofstream m_stream;
	std::fstream m_tmpStream;
	unsigned int size;
    };

}} // namespace vle stream

#endif

