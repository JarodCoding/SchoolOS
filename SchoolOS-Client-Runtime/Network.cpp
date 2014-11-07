/*
 * Network.cpp
 *	
 *  Created on: Sep 9, 2014
 *      Author: Pascal Kuthe
 */

#include "Network.hpp"
#include "string"
#include "FileSystem.hpp"
#include "Info.hpp"
#include "boost/asio.hpp"
namespace SchoolOS{
	namespace Network{
	Connection::Connection(boost::asio::io_service& tmp_io_service,boost::asio::ip::tcp::resolver::iterator endpoint_iterator) : io_service(new boost::asio::io_service()),socket(new boost::asio::ip::tcp::socket(*io_service)){
					//Connect to the Server
					boost::asio::connect((*socket),endpoint_iterator);
		     		boost::array<uint16_t, 1> buf;
			        buf.elems[0] = SchoolOS::SclientID;
				    boost::asio::mutable_buffers_1 tmp = boost::asio::buffer(buf);

				    send(tmp);
				    buf.elems[0] = 0;
				    tmp = boost::asio::buffer(buf);

				    read(tmp);
				    if(buf.elems[0]!=SchoolOS::SclientID){
				    	socket->close();
				    	if(buf.elems[0] == 1)
				    		std::cerr << "Client Autoidentification failed";
				    	else
					    	std::cerr << "Server Autoidentification failed";
				    	system("shutdown now");

				    }
				}
	void Connection::send(boost::asio::mutable_buffers_1 buf){
		boost::asio::write((*socket),buf);

	}
	void Connection::read(boost::asio::mutable_buffers_1 &buf){
					//TODO Error Handing
					socket->read_some(buf);
		}
	void Connection::read(google::protobuf::Message &message){
						boost::array<char,4> Length;
						boost::asio::mutable_buffers_1 Buffer = boost::asio::buffer(Length);
						read(Buffer);
						google::protobuf::io::ArrayInputStream *ais = new google::protobuf::io::ArrayInputStream(Length.elems,4,sizeof(char));
						google::protobuf::io::CodedInputStream *cis = new google::protobuf::io::CodedInputStream(ais);
						uint size = 0;
						cis->ReadVarint32(&size);
						char *arr = new char[size];
						Buffer = boost::asio::buffer(arr,size);
						read(Buffer);
						ais = new google::protobuf::io::ArrayInputStream(arr,size,sizeof(char));
						cis = new google::protobuf::io::CodedInputStream(ais);
						message.ParseFromCodedStream(cis);
	}
	void Connection::send(google::protobuf::Message &message){
					char *arr = new char[message.ByteSize()+4];
					google::protobuf::io::ArrayOutputStream *aos = new google::protobuf::io::ArrayOutputStream(arr,message.ByteSize(),sizeof(char));
					google::protobuf::io::CodedOutputStream *cos = new google::protobuf::io::CodedOutputStream(aos);
					cos->VarintSize32(message.ByteSize());
					message.SerializeToCodedStream(cos);
					send(boost::asio::buffer(arr,message.ByteSize()+4));

	}
	int Connection::processPacket(){
					boost::array<uint16_t,1> id;
					boost::asio::mutable_buffers_1 buffer = boost::asio::buffer(id);
					read(buffer);

					if(id[0] == 1)return -1;//Only send by client
					else if(id[0] == 2){
						boost::array<uint16_t,1> error_Code;
						boost::asio::mutable_buffers_1 buffer = boost::asio::buffer(error_Code);
						read(buffer);
						if(error_Code[0] == 0){
							//TODO Proceed with Login
						}else{
							//TODO Error Handling

						}

					}else if(id[0] == 3){
						//TODO: Security

						LoginProcess process;
						read(process);
						// Check Process

						FileSystem::initFileSystem(process.rootdirectory());
						int i = 0;
						std::string Programms("sudo /.SchoolOS/.bin/SchoolOSLogin");
						while(i < process.programms_size()){
							Programms.append(std::to_string(process.programms().Get(i).pid()));//Convert to string
							i++;
							//Init File Systems
						}
						system(Programms.c_str());
					}

					return id[0];
				}
	int initNetwork(){
			//Create and Resolve Networking Objects
			boost::asio::io_service io_service;
			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(SchoolOS::SServerAdress, "SchoolOS");
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
			Connection conn(io_service,endpoint_iterator);

	    	return 0;
	}

	}
}
