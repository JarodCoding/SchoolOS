/*
  Network.hpp

   Created on: Jun 28, 2014
       Author: Pascal Kuthe

Copyright (c) <2014> <Pascal Kuthe>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

#include "boost/asio.hpp"
#include "iostream"
#include "boost/array.hpp"
#include "stdlib.h"
#include <thread>
#include "Login.pb.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#ifndef NETWORK_HPP_
#define NETWORK_HPP_

namespace SchoolOS{
	namespace Network{



		// The Connection to the Server (synchron)
		class Connection{
		private:


			boost::asio::io_service *io_service;
			boost::asio::ip::tcp::socket *socket;



		public:

			//Constructor :: Connects to the Server
			Connection(boost::asio::io_service& tmp_io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
			void send(boost::asio::mutable_buffers_1 buf);

			void read(boost::asio::mutable_buffers_1 &buf);
			void read(google::protobuf::Message &message);

			void send(google::protobuf::Message &message);
			//Returns the PacketID of the Recived Packet
			int processPacket();
			/*
			 *
			 * -1 = error
			 *  Packet IDS:
			 *
			 *  1 = Login
			 *  2 = LoginResult
			 *  3 = LoginProcess
			 *  4 = LoginFinished
			 *  5 = Logout
			 *  6 = SwitchkPcLock
			 *  7 = shutdown
			 *
			 */


		};


	/*
	 * Initializes the network on startup
	 *
	 * Error Codes:
	 * 	1: NetworkFail
	 * 	2: SecurityFail
	 */
	int initNetwork();
	}
}


#endif /* NETWORK_HPP_ */
