/*
 * FileSysten.cpp
 *	
 *  Created on: Sep 23, 2014
 *      Author: Pascal Kuthe
 */

#include "FileSystem.hpp"
#define _cplusplus
#define FUSE_USE_VERSION 26
#include "cstring"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "fcntl.h"
#include "errno.h"
#include "fuse/fuse.h"
#include "string"
#include "vector"
#include <fuse/fuse_opt.h>
#include "errno.h"
#include "unordered_map"
#include "boost/thread/mutex.hpp"
#define Mount  "/home"
#define Normal "/.cache"
namespace SchoolOS {
	namespace FileSystem{
		namespace Objects{
			 class FileSystemElement{
				public:
				 FileSystemElement(std::string name):name(name),parent(nullptr),cached(0){

					}
				~FileSystemElement() {

					}
					std::string getPath(){
						return parent->getPath().append("/").append(getName());
					}
					std::string getName(){
						return name;
					}
					Objects::FileSystemElement *getParent(){
						return parent;
					}

					void addToFileSystem(FileSystemElement* parent){
						this->parent = parent;
						cached = 0;
					}
					void removeFromFileSystem(){
						parent = nullptr;
						cached = 0;

					}
					virtual void changeName(std::string newname){
						int i = 0;
						if(cached){
							rename(getPath().c_str(),parent->getPath().append(newname).c_str());
						}
						name = newname;

					}
					bool inFileSystem(){
						return parent != nullptr;
					}
					bool isCached(){
						return cached;
					}
					void cache(){
						if(!inFileSystem())return;
						if(!parent->isCached())parent->cache();
						cached = realCache();

					}
					virtual bool realCache() = 0;
				protected:
					std::string name;
					FileSystemElement *parent;
					bool cached;
			};
			class File: public FileSystemElement{
				public:
					File(std::string name):FileSystemElement(name),size(0){

					}
					File(std::string name,__off_t size):FileSystemElement(name),size(size){

					}
					File(const ::File &NetworkFile):FileSystemElement(NetworkFile.name()),size(NetworkFile.size()){

					}
					bool realCache(){
						//TODO download file from server
						return creat(std::string(Normal).append(getPath()).c_str(),0666);
					}
				__off_t getSize(){
					return size;
				}
				private:
					__off_t size;
			};
			class Directory: public FileSystemElement{
				public:
					Directory(std::string name):FileSystemElement(name){															//Standard constructor
					}
					Directory(Directory &dir):FileSystemElement(dir.getName()),files(dir.getFiles()),subdirectorys(dir.getSubdirectorys()){//Copy constructor
					}
					Directory(const ::Directory &dir):FileSystemElement(dir.name()),files(dir.files_size()),subdirectorys(dir.subdirectorys_size()){																//Network Constructor
						int i = 0;
						while(i < dir.files_size()){
							files[i] = new File(dir.files(i));
							i++;
						}
						i = 0;
						while(i < dir.subdirectorys_size()){
							subdirectorys[i] = new Directory(dir.subdirectorys(i));
							i++;
						}
					}




					std::vector<File*> getFiles(){
						return files;
					}
					File* getFile(int index){
						return files.at(index);
					}
					File* getFile(std::string name){
						int i = 0;
						while(i < files.size()){
							if(files.at(i)->getName()==name)return files.at(i);
							i++;
						}
						return nullptr;
					}
					void addFile(File &file){
						File *tmp = new File(file.getName(),file.getSize());
						files.push_back(tmp);
						tmp->addToFileSystem(this);

					}
					void addFile(std::string name){
						File* tmp = new File(name);
						files.push_back(tmp );
						tmp->addToFileSystem(this);
					}
					void deleteFile(int i){
						files.at(i)->removeFromFileSystem();
						delete files.at(i);
						files.erase(files.begin()+i);
					}
					bool deleteFile(std::string name){
						unsigned int i = 0;
						while(i < files.size()){
							if(files.at(i)->getName()==name){
								deleteFile(i);
								return true;
							}
							i++;
						}
						return false;

					}
					std::vector<Directory*> getSubdirectorys(){
						return subdirectorys;
					}
					Directory* getSubdirectory(int i){
						return subdirectorys.at(i);
					}
					Directory* getSubdirectory(std::string name){
						unsigned i = 0;
						while(i < subdirectorys.size()){
							if(subdirectorys.at(i)->getName()==name)return subdirectorys.at(i);
							i++;
						}
						return nullptr;
					}




					void addDirectory(Directory& dir){
						addDirectory(new Directory(dir));
					}

					protected:
					void addDirectory(Directory* dir){
						dir->addToFileSystem(this);

						subdirectorys.push_back(dir);
					}
					public:

					void addDirectory(std::string name){
						addDirectory(new Directory(name));
					}
					void deleteDirectory(int i){
						subdirectorys.at(i)->removeFromFileSystem();
						delete subdirectorys.at(i);
						subdirectorys.erase(subdirectorys.begin()+i);
					}
					bool deleteDirectory(std::string name){
						unsigned int i = 0;
						while(i < subdirectorys.size()){
							if(subdirectorys.at(i)->getName()==name){
								deleteDirectory(i);
								return true;
							}
							i++;
						}
						return false;
					}
					virtual bool realCache(){
						//TODO download file from server
						return mkdir(std::string(Normal).append(getPath()).c_str(),0666);
					}
					protected:
					std::vector<File*> files;
					std::vector<Directory*> subdirectorys;

			};

		}
		Objects::Directory* root;

	}


}  // namespace SchoolOS
namespace fuseNamespace{
char * getParentPath(const char *path){
	int i =  strlen(path)-2;//minus one fir the way arrays work and another -1 ,because there is never going to be a // and in case of directorys / at the end should be ignored
		bool isRunning = true;
		while(i <= 0&&isRunning){
			if(path[i] == '/')isRunning = false;
			i--;
		}
		if(i == 0)return nullptr;
		char * dirPath = (char *)malloc(i*sizeof(char));
		memcpy(dirPath,path,sizeof(char)*i);
		return dirPath;
}

namespace NameCache{

	std::unordered_map<std::string,SchoolOS::FileSystem::Objects::FileSystemElement *> *NameCache = new std::unordered_map<std::string,SchoolOS::FileSystem::Objects::FileSystemElement *>();
	boost::mutex NameCacheMutex;
	std::unordered_map<std::string,unsigned char> *FailCache = new std::unordered_map<std::string,unsigned char>();	//since every element uin this would have the value nullptr i decided to put them in a seperate map and remove them after the value reaches 0
	boost::mutex FailCacheMutex;
	boost::mutex FailCacheCountdownMutex;
	unsigned char FailCacheCountdown = 50;
	void update(){
		FailCacheCountdownMutex.lock();
		if(--FailCacheCountdown<=0){
			FailCacheCountdown = 50;
			int i = 0;
			while(i < FailCache->size()){
				if(--((FailCache->begin()+++i)->second) <= 0)FailCache->erase(FailCache->begin()+i);
			}
		}
		FailCacheCountdownMutex.unlock();
	}
	void cache(const char * path,SchoolOS::FileSystem::Objects::FileSystemElement *ptr){
		update();

		if(ptr == nullptr){
			FailCacheMutex.lock();
			FailCache->emplace(path,FailCacheCountdown<20?2:1);
			FailCacheMutex.unlock();
		}else{
			NameCacheMutex.lock();
			NameCache->emplace(path,ptr);
			NameCacheMutex.unlock();
		}
	}
	void remove(const char * path){
		update();
		NameCacheMutex.lock();
		 if(NameCache->erase(path)< 1){
				NameCacheMutex.unlock();
				FailCacheMutex.lock();
				FailCache->erase(path);
				FailCacheMutex.unlock();

		 }else
			 NameCacheMutex.lock();


	}
	SchoolOS::FileSystem::Objects::FileSystemElement * get(const char *path){
		update();
		SchoolOS::FileSystem::Objects::FileSystemElement *res = nullptr;
		NameCacheMutex.lock();

		try{
			res = NameCache->at(path);
		}catch(std::out_of_range& e){
			NameCacheMutex.unlock();
			FailCacheMutex.lock();
			try{
				unsigned char &ref = FailCache->at(path);
				ref = std::min(255,ref+1);

			}catch(std::out_of_range& e){
				memset((void *)path,0,sizeof(char)*strlen(path));
			}
			FailCacheMutex.unlock();
			return nullptr;

		}
		NameCacheMutex.unlock();
		return res;


	}
	void reCache(const char * newPath, const char *oldPath){
		update();
		NameCacheMutex.lock();
		SchoolOS::FileSystem::Objects::FileSystemElement *elem;
		try{
			elem = NameCache->at(oldPath);
		}catch(std::out_of_range& e){
			NameCacheMutex.unlock();
			return;
		}
		NameCache->erase(oldPath);
		NameCache->emplace(newPath,elem);
		NameCacheMutex.unlock();

	}


}
SchoolOS::FileSystem::Objects::Directory* getDirectory(const char *path){
	if(strlen(path) <= 1){
		if(strcmp(path,"/"))return SchoolOS::FileSystem::root;
		else return nullptr;
	}








	int i;



	const int TotalSize = strlen(path);
	int CurrentSize;

	int pos = 1;

	char * tmp;
	int ReaderSize;
	bool ReaderRunning;


	SchoolOS::FileSystem::Objects::Directory* currentDirecetory = SchoolOS::FileSystem::root;
	std::vector<SchoolOS::FileSystem::Objects::Directory*> CurrentSubdirectorys;



	while(pos < TotalSize){
		//read the string to either the next / or the end of the string (\)
		CurrentSize = TotalSize-pos;
		ReaderRunning = true;
		ReaderSize = pos;
		while(ReaderRunning&&ReaderSize<CurrentSize){
			ReaderRunning = path[ReaderSize+++pos] != '/';	/* +++:		DEAL WITH THAT!!! XD*/
		}
		tmp = (char*)malloc(sizeof(char)*ReaderSize);
		if(ReaderSize>= CurrentSize)memccpy(tmp,path,pos,ReaderSize);
		else{
			memccpy(tmp,path,pos,ReaderSize-1);
			tmp[ReaderSize-1] = '\0';
		}
		pos += ReaderSize;

		CurrentSubdirectorys = currentDirecetory->getSubdirectorys();
		i = 0;
		while(i < CurrentSubdirectorys.size()){
			if(strcasecmp(CurrentSubdirectorys.at(i)->getName().c_str(),tmp)){
				currentDirecetory = CurrentSubdirectorys.at(i);
				delete tmp;
				if(pos>=TotalSize)return CurrentSubdirectorys.at(i);

				else continue;
			}
			i++;
		}

		// we should only end up here if the current string is not a name of one of the subdirectory of the current parentdirectory
		return nullptr;
	}
	return nullptr;
}

bool isDirectory(const char * path){
	return getDirectory(path)!= nullptr;
}
SchoolOS::FileSystem::Objects::File* getFile(const char *path){
	int i =  strlen(path)-2;//minus one fir the way arrays work and another -1 ,because there is never going to be a // and in case of directorys / at the end should be ignored
		bool isRunning = true;
		while(i <= 0&&isRunning){
			if(path[i] == '/')isRunning = false;
			i--;
		}
		if(i == 0)return nullptr;
		char * dirPath = (char *)malloc(i*sizeof(char));
		memcpy(dirPath,path,sizeof(char)*i);
	SchoolOS::FileSystem::Objects::Directory *dir = getDirectory(dirPath);
	if(dir == nullptr)return nullptr;
	delete dirPath;
	char * filePath = (char *)malloc((strlen(path)-i)*sizeof(char));
	memccpy(filePath,path,(i+1)*sizeof(char),(strlen(path)-i)*sizeof(char));
	SchoolOS::FileSystem::Objects::File *file = dir->getFile(filePath);
	delete filePath;
	NameCache::cache(file->getPath().c_str(),file);
	return file;
}
bool isFile(const char *path){
	return getFile(path)!= nullptr;
}
//returns whather path is within check (It doesn't matter whather path is a directory or a file or doesn't even exist if you care then you have to check that on your own (There enough methods for that))
bool contains(const char *path,const char *check){
	int tmp = strlen(check);
	if(tmp > strlen(path))return false;
	return strncasecmp(path,check,tmp);
}

static int m_getattr(const char *path, struct stat *stbuf)
	{
		memset(stbuf, 0, sizeof(struct stat));
		if(isDirectory(path)){
			//A directory:
			stbuf->st_mode = S_IFDIR | 0666;
			stbuf->st_nlink = 2;
			return 0;
		}
		SchoolOS::FileSystem::Objects::File * tmp = getFile(path);
		if(tmp == nullptr){
			//A file:
			stbuf->st_mode = S_IFREG | 0666;
			stbuf->st_nlink = 1;
			stbuf->st_size = getFile(path)->getSize();
			return 0;
		}

		return -ENOENT;

	}
static int m_unlink (const char *){

}
static int m_rmdir (const char *path){
	SchoolOS::FileSystem::Objects::Directory *dir = getDirectory(path);
	if(dir == nullptr)return -ENOENT;
	((SchoolOS::FileSystem::Objects::Directory *)dir->getParent())->deleteDirectory(dir->getName());

	//TODO: Networkflush
}
static int m_mkdir(const char * path,mode_t){
	SchoolOS::FileSystem::Objects::Directory *parent = getDirectory(getParentPath(path));
	if(parent == nullptr)return -ENOENT;
	std::string  name = std::string(path).substr(parent->getPath().length()-1,strlen(path)-parent->getPath().length());
	if(parent->getSubdirectory(name.c_str()) != nullptr)return -ENOENT;
	parent->addDirectory(name.c_str());
		return 0;
	}
static int m_rename(const char * old,const char* newname){
	SchoolOS::FileSystem::Objects::File *tmp = getFile(old);
	if(tmp == 0)return -ENOENT;
	tmp->changeName(newname);

	return 0;
}
static int m_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
            off_t offset, struct fuse_file_info *fi)
	{
		SchoolOS::FileSystem::Objects::Directory *dir = getDirectory(path);

		if (dir == nullptr)
			return -ENOENT;

		std::vector<SchoolOS::FileSystem::Objects::Directory *> childDirecotrys = dir->getSubdirectorys();
		struct stat *stbuf = (struct stat*) malloc(sizeof(struct stat));	//This can be reused because we set it to 0 at beginning of getattr anyway
		memset(stbuf, 0, sizeof(struct stat));
		stbuf->st_mode = S_IFDIR;

		int i = offset;

		while(i < childDirecotrys.size()){
			filler(buf, childDirecotrys.at(i)->getName().c_str(), stbuf, 1);

			i++;
		}
		stbuf->st_mode = S_IFREG;
		std::vector<SchoolOS::FileSystem::Objects::File *> childFiles = dir->getFiles();
		int diramount = i;
		while(i < childFiles.size()+diramount){
					filler(buf, childFiles.at(i)->getName().c_str(), stbuf, 1);

					i++;
				}
		delete stbuf;

		filler(buf, "..", NULL, 0);


	//filler(buf, hello_path + 1, NULL, 0);

		return 0;
	}

	static int m_open(const char *path, struct fuse_file_info *fi)
	{
		//if (strcmp(path, hello_path) != 0)
		//	return -ENOENT;

		if ((fi->flags & 3) != O_RDONLY)
			return -EACCES;

		return 0;
	}

	static int m_read(const char *path, char *buf, size_t size, off_t offset,
			      struct fuse_file_info *fi)
	{
		size_t len;
		(void) fi;
		//if(strcmp(path, hello_path) != 0)
		//	return -ENOENT;

	//	len = strlen(hello_str);
		if (offset < len) {
			if (offset + size > len)
				size = len - offset;
		//	memcpy(buf, hello_str + offset, size);
		} else
			size = 0;

		return size;
	}

	//OK I have to make a real talk now. I love open source especially linux and most gnu projects but WTF GNU G++ YOU REALLY MAKE ME DEFINE EVERY SINGLE METHOD IN THIS STUPID STRUCTURE THAT IS ABSULUTLY IMPOSSIBLE
	/*static struct fuse_operations m_oper = {
			.getattr = m_getattr,
//			.readlink = nullptr,
			.getdir = nullptr,
			.mknod = nullptr,
			.mkdir = m_mkdir,
			.unlink = m_unlink,
			.rmdir = m_rmdir,
			//.flag_nullpath_ok = 1,
			};*/
	static fuse_operations m_oper;
	void initFuse(){
		m_oper.getattr 	= m_getattr;
		m_oper.unlink  	= m_unlink;
		m_oper.rmdir   	= m_rmdir;
		m_oper.mkdir	= m_mkdir;
		m_oper.rename 	= m_rename;
		m_oper.flush = 	= m_flush;//TODO: flush()
	}
	void runFuse(char *root){
		char** fuse_args = new char* [1];
				fuse_args[0] = root;
				fuse_main_real(1,(char**)fuse_args,&m_oper,0,nullptr);
	}
}
namespace SchoolOS{
	namespace FileSystem{



		void initFileSystem(const Directory & from){
			fuseNamespace::initFuse();

			root = new Objects::Directory(from);

			fuseNamespace::runFuse(Mount);//TODO multithreading init
		}
	}
}
