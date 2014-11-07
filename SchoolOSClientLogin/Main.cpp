/*
  Main.cpp

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
#include "Programm.h"
#include "stdlib.h"
#include "boost/interprocess/shared_memory_object.hpp"
#include "boost/interprocess/mapped_region.hpp"


int main(int argc, char *argv[]) {
	/*Create Shared Memory*/
		boost::interprocess::shared_memory_object smo(boost::interprocess::create_only,"SchoolOsRegistry",boost::interprocess::read_write);
		/*Open Connection to Database*/
		ProgrammDatabase::initDatabase();
		/*Install Programms*/
		smo.truncate(sizeof(uint16_t)*argc);
		int i = 0;
		SchoolOS::Programm *tmp;
		long int length;
		smo.get_size(length);
		boost::interprocess::mapped_region region(smo,boost::interprocess::read_write,0,length);
		uint16_t* pointer = (uint16_t*) region.get_address();
		while(i < argc){
			tmp = new SchoolOS::Programm(atoi(argv[i]));
			tmp->install();
			pointer[i] = tmp->getId();
			delete tmp;
			i++;
		}
		return 0;

}





