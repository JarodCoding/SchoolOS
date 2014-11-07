/*
  Info.hpp

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
#include "string"
#include "stdlib.h"
#include "iostream"
#include "fstream"
#ifndef INFO_HPP_
#define INFO_HPP_
namespace SchoolOS{
	// The ID the PC is known by;
	extern int SclientID;
	//The Address of the SchoolOSServer
	extern char *SServerAdress;
	extern int SServerID;
	/*
	 * Reads All Config Files on Startup
	 * Eroor Codes:
	 *		1 = SclientID-File Not Found
	 *		2 = ServerInfo Not Found
	 */
	inline int readConfig(){
		//Define File Inputstream(READ ONLY)
		std::ifstream stream;
		/*Read SclientID*/
			stream.open("/.SchoolOS/.config/.SclientID");
			if(!stream.is_open())return 1;

			stream >> SchoolOS::SclientID;

			stream.close();
		/*Read ServerInformation*/
			stream.open("/.SchoolOS/.config/.ServerAddress");
			if(!stream.is_open())return 2;

			stream >> SServerAdress;
			stream >> SServerID;

			stream.close();


		return 0;
	}
}





#endif /* INFO_HPP_ */
