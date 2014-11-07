/*
  Programm.cpp

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
#include "sqlite3.h"
#include "string.h"
#include "stdlib.h"
#include "iostream"
#include "sstream"
namespace ProgrammDatabase{
	sqlite3 **ref;





	void initDatabase(){
		sqlite3_open_v2("/.SchoolOS/.Registry/.ProgrammsInfo.db",ref,SQLITE_OPEN_READONLY,"");

	}

	void destroyDatabase(){
		sqlite3_close_v2(*ref);
		//TODO: Research avoid memory la

	}

	std::string getName(uint_fast16_t id){
		sqlite3_stmt **sttmt;
		std::stringstream ss;

		ss << "SELECT name FROM main WHERE id = "<< id;

		sqlite3_prepare_v2(*ProgrammDatabase::ref,ss.str().c_str(),-1,sttmt,NULL);
		sqlite3_step(*sttmt);
		if(sqlite3_step(*sttmt)==SQLITE_ROW){
			return std::string((const char *)sqlite3_column_text(*sttmt,0));

		}else{
			return std::string("Error:No value returned from Database");
		}
		}
	int getSecurityLevel(uint_fast16_t id){
			sqlite3_stmt **stmt;
			std::stringstream ss;
			ss << "SELECT icon FROM main WHERE id = "<< id;

			sqlite3_prepare_v2(*ProgrammDatabase::ref,ss.str().c_str(),-1,stmt,NULL);
			if(sqlite3_step(*stmt)==SQLITE_ROW)
				return sqlite3_column_int(*stmt,0);
			return 0;
		}

}

namespace SchoolOS {


Programm::Programm(uint_fast16_t id):id(id),name("Database Connection Failed!") {

	/* Request Programm-Name from Database */
	name = ProgrammDatabase::getName(id);
}
Programm::~Programm(){
	id = 0;
}
void Programm::install(){
	std::string installcommand;
			if(ProgrammDatabase::getSecurityLevel(id)>= 1){
				installcommand = "pacman -S ";
				installcommand.append(name);
				installcommand.append("--logfile /.SchoolOS/Logs/Pacman.log --noconfirm");
			}else{
			installcommand = "pacman -U /.SchoolOS/.packages/";
			installcommand.append(name);
			installcommand.append(".pkg --logfile /.SchoolOS/Logs/Pacman.log --noconfirm");
			}
			system(installcommand.c_str());
}
void Programm::uinstall(){
		std::string installcommand = "pacman -R ";
		installcommand.append(name);
		installcommand.append(" --logfile /.SchoolOS/Logs/PacmanLog --noconfirm");
		system(installcommand.c_str());
}
uint16_t Programm::getId(){
	return id;
}
} /* namespace SchoolOS */
