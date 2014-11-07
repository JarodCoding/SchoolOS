/*
  Programm.h

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

#include "stdint.h"
#include "string"
#ifndef PROGRAMM_H_
#define PROGRAMM_H_

namespace ProgrammDatabase{

	void initDatabase();

	void destroyDatabase();

	std::string getName(uint_fast16_t id);

	int getSecurityLevel(uint_fast16_t id);

}

namespace SchoolOS {

class Programm {
public:
	Programm(uint_fast16_t id);
	virtual ~Programm();
	void install();
	void uinstall();
	std::string getName();
	uint_fast8_t getSecurityLevel();
	uint16_t getId();
private:
	uint16_t id;
	std::string name;
};

} /* namespace SchoolOS */

#endif /* PROGRAMM_H_ */
