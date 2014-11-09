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
#include "boost/thread/shared_mutex.hpp"

#define Mount  "/home"
#define Normal "/.cache"
namespace SchoolOS {
	namespace FileSystem{
		namespace Objects{
			 class FileSystemElement{
				public:
				 FileSystemElement(std::string name):name(name),parent(nullptr),cached(0){

					}
				virtual ~FileSystemElement() {
					if(inFileSystem()){
						removeFromFileSystem();
					}
					}
					std::string getPath(){
						std::string path = getParent()->getPath().append("/").append(getName());
						return path;
					}
					std::string getName(){
						boost::shared_lock<boost::shared_mutex> lock(nameMutex);
						std::string tmp = name;
						lock.release();
						return tmp;

					}
					Objects::FileSystemElement *getParent(){
						boost::shared_lock<boost::shared_mutex> lock(parentMutex);
						Objects::FileSystemElement *tmp = parent;
						lock.release();
						return tmp;
					}

					void addToFileSystem(FileSystemElement* parent){

						boost::upgrade_lock<boost::shared_mutex> lock1(parentMutex);
						if(parent != nullptr){
							lock1.release();
							return;
						}
						boost::upgrade_to_unique_lock<boost::shared_mutex> lock3(lock1);
						this->parent = parent;
						lock1.release();
						boost::unique_lock<boost::shared_mutex> lock2(cacheMutex);
						cached = 0;
						lock2.release();
					}
					void removeFromFileSystem(){
						boost::upgrade_lock<boost::shared_mutex> lock1(parentMutex);
						if(parent == nullptr){
							lock1.release();
							return;
						}
						boost::upgrade_to_unique_lock<boost::shared_mutex> lock3(lock1);
						parent = nullptr;
						lock1.release();
						boost::unique_lock<boost::shared_mutex> lock2(cacheMutex);
						cached = 0;
						lock2.release();

					}
					virtual void changeName(std::string newname){
						boost::unique_lock<boost::shared_mutex> lock;
						if(isCached()){
							lock(cacheMutex);
							rename(getPath().c_str(),getParent()->getPath().append(newname).c_str());
							lock.release();
						}
						lock(nameMutex);
						name = newname;
						lock.release();
					}
					bool inFileSystem(){
						boost::shared_lock<boost::shared_mutex> lock(parentMutex);
						bool tmp = parent != nullptr;
						lock.release();
						return tmp;
					}
					bool isCached(){
						boost::shared_lock<boost::shared_mutex> lock(cacheMutex);
						bool tmp = cached;
						lock.release();
						return tmp;
					}
					void cache(){
						if(!inFileSystem())return;
						FileSystemElement *parent = getParent();
						if(!parent->isCached())parent->cache();
						boost::upgrade_lock<boost::shared_mutex> lock1(cacheMutex);
						if(cached == true){
							lock1.release();
							return;
						}
						boost::upgrade_to_unique_lock<boost::shared_mutex> lock2(lock1);
						cached = realCache();
						lock1.release();
					}
					virtual bool realCache() = 0;
				protected:
					std::string name;
					boost::shared_mutex nameMutex;
					FileSystemElement *parent;
					boost::shared_mutex parentMutex;
					bool cached;
					boost::shared_mutex cacheMutex;

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
					boost::shared_lock<boost::shared_mutex> lock(sizeMutex);
					__off_t tmp = size;
					lock.release();
					return tmp;
				}
				void resize(__off_t newsize){
					boost::unique_lock<boost::shared_mutex> lock(sizeMutex);
					size = newsize;
					lock.release();
				}
				private:
					__off_t size;
					boost::shared_mutex sizeMutex;
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
						boost::shared_lock<boost::shared_mutex> lock(filesMutex);
						std::vector<File*> tmp = files;
						lock.release();
						return tmp;
					}
					File* getFile(int index){
						boost::shared_lock<boost::shared_mutex> lock(filesMutex);
						File* tmp = files.at(index);
						lock.release();
						return tmp;
					}
					File* getFile(std::string name){
						boost::shared_lock<boost::shared_mutex> lock(filesMutex);

						int i = 0;
						while(i < files.size()){
							if(files.at(i)->getName()==name){
								File* tmp = files.at(i);
								lock.release();
								return tmp;
							}
							i++;
						}
						lock.release();
						return nullptr;
					}
					void addFile(File &file){
						boost::upgrade_lock<boost::shared_mutex> lock1(filesMutex);
						int i = 0;
						while(i < files.size()){
							if(files.at(i)->getName()==file.getName()){
								lock1.release();
								return;
							}
							i++;
						}
						boost::upgrade_to_unique_lock<boost::shared_mutex> lock2(lock1);
						File *tmp = new File(file.getName(),file.getSize());
						files.push_back(tmp);
						lock1.release();
						tmp->addToFileSystem(this);

					}
					void addFile(std::string name){
					boost::upgrade_lock<boost::shared_mutex> lock1(filesMutex);
					int i = 0;
					while(i < files.size()){
						if(files.at(i)->getName()==name){
							lock1.release();
							return;
						}
						i++;
					}
					boost::upgrade_to_unique_lock<boost::shared_mutex> lock2(lock1);

						File* tmp = new File(name);
						files.push_back(tmp );
						lock1.release();
						tmp->addToFileSystem(this);
					}
					void deleteFile(int i){
						boost::unique_lock<boost::shared_mutex> lock(filesMutex);
						delete files.at(i);
						files.erase(files.begin()+i);
						lock.release();
					}
					bool deleteFile(std::string name){
						boost::upgrade_lock<boost::shared_mutex> lock1(filesMutex);
						unsigned int i = 0;
						while(i < files.size()){
							if(files.at(i)->getName()==name){
								boost::unique_lock<boost::shared_mutex>  lock2(lock1);
								delete files.at(i);
								files.erase(files.begin()+i);
								lock1.release();
								return true;
							}
							i++;
						}
						lock1.release();
						return false;

					}
					std::vector<Directory*> getSubdirectorys(){
						boost::shared_lock<boost::shared_mutex> lock(directoryMutex);
						std::vector<Directory*> tmp = subdirectorys;
						lock.release();
						return tmp;
					}
					Directory* getSubdirectory(int i){
					boost::shared_lock<boost::shared_mutex> lock(directoryMutex);
					Directory* tmp;
					try{
						tmp = subdirectorys.at(i);
					}catch(std::out_of_range& e){
						tmp = nullptr;
					}
					lock.release();
					return tmp;
					}
					Directory* getSubdirectory(std::string name){
						boost::shared_lock<boost::shared_mutex> lock(directoryMutex);
						unsigned i = 0;
						while(i < subdirectorys.size()){
							if(subdirectorys.at(i)->getName()==name){
								Directory* tmp = subdirectorys.at(i);
								lock.release();
								return tmp;
							}
							i++;
						}
						lock.release();
						return nullptr;
					}




					void addDirectory(Directory& dir){
						addDirectory(new Directory(dir));
					}

					protected:
					void addDirectory(Directory* dir){
					boost::upgrade_lock<boost::shared_mutex> lock1(directoryMutex);
					unsigned i = 0;
					while(i < subdirectorys.size()){
						if(subdirectorys.at(i)->getName()==dir->getName()){
							lock1.release();
							return;
						}
						i++;
					}
					dir->addToFileSystem(this);
					boost::upgrade_to_unique_lock<boost::shared_mutex> lock2(lock1);
					subdirectorys.push_back(dir);
					lock1.release();
					}
					public:

					void addDirectory(std::string name){
						addDirectory(new Directory(name));
					}
					void deleteDirectory(int i){
						boost::upgrade_lock<boost::shared_mutex> lock1(directoryMutex);
						Directory* tmp;
						try{
							tmp = subdirectorys.at(i);
						}catch(std::out_of_range& e){
							lock1.release();
							return;
						}
						delete tmp;
						boost::upgrade_to_unique_lock<boost::shared_mutex> lock2(lock1);
						subdirectorys.erase(subdirectorys.begin()+i);
						lock1.release();
					}
					bool deleteDirectory(std::string name){
						boost::upgrade_lock<boost::shared_mutex> lock1(directoryMutex);
						unsigned int i = 0;
						while(i < subdirectorys.size()){
							if(subdirectorys.at(i)->getName()==name){
								boost::upgrade_to_unique_lock<boost::shared_mutex> lock2(lock1);
								delete subdirectorys.at(i);
								subdirectorys.erase(subdirectorys.begin()+i);
								lock1.release();
								return true;
							}
							i++;
						}
						lock1.release();

						return false;
					}
					virtual bool realCache(){
						//TODO download file from server
						return mkdir(std::string(Normal).append(getPath()).c_str(),0666);
					}
					protected:
					std::vector<File*> files;
					boost::shared_mutex filesMutex;
					std::vector<Directory*> subdirectorys;
					boost::shared_mutex directoryMutex;
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
	boost::shared_mutex NameCacheMutex;
	std::unordered_map<std::string,unsigned char> *FailCache = new std::unordered_map<std::string,unsigned char>();	//since every element uin this would have the value nullptr i decided to put them in a seperate map and remove them after the value reaches 0
	boost::shared_mutex FailCacheMutex;
	boost::mutex FailCacheCountdownMutex;
	unsigned char FailCacheCountdown = 50;
	void update(){
		FailCacheCountdownMutex.lock();
		if(--FailCacheCountdown<=0){
			FailCacheCountdown = 50;
			int i = 0;
			auto iter = FailCache->begin();
			while(i < FailCache->size()){
				if(--(iter->second) == 0)
				iter++;
				i++;


			}
		}
		FailCacheCountdownMutex.unlock();
	}
	void cache(const char * path,SchoolOS::FileSystem::Objects::FileSystemElement *ptr){
		update();
		boost::unique_lock<boost::shared_mutex> lock;
		if(ptr == nullptr){
			lock(FailCacheMutex);
			FailCache->emplace(path,FailCacheCountdown<20?2:1);
		}else{
			lock(NameCacheMutex);
			NameCache->emplace(path,ptr);
		}
		lock.release();

	}
	void remove(const char * path){
		update();
		boost::unique_lock<boost::shared_mutex> lock(NameCacheMutex);
		 if(NameCache->erase(path)< 1){
			 	lock.release();
			 	lock(FailCacheMutex);
				FailCache->erase(path);
				lock.release();
		 }else
			lock.release();


	}
	SchoolOS::FileSystem::Objects::FileSystemElement * get(const char *path){
		update();
		SchoolOS::FileSystem::Objects::FileSystemElement *res = nullptr;
		boost::shared_lock<boost::shared_mutex> lock(NameCacheMutex);

		try{
			res = NameCache->at(path);
			memset((void *)path,0,sizeof(char)*strlen(path));

		}catch(std::out_of_range& e){
			lock.release();
			lock(FailCacheMutex);
			try{
				unsigned char &ref = FailCache->at(path);
				ref = std::min(255,ref+1);
				memset((void *)path,0,sizeof(char)*strlen(path));


			}catch(std::out_of_range& e){
			}

			lock.release();
			return nullptr;

		}
		lock.release();
		return res;


	}
	void reCache(const char * newPath, const char *oldPath){
		update();
		boost::upgrade_lock< boost::shared_mutex > lock(NameCacheMutex);
		SchoolOS::FileSystem::Objects::FileSystemElement *elem;
		try{
			elem = NameCache->at(oldPath);
		}catch(std::out_of_range& e){
			lock.release();
			return;
		}
		boost::upgrade_to_unique_lock<boost::shared_mutex> writelock(lock);
		NameCache->erase(oldPath);
		NameCache->emplace(newPath,elem);
		lock.release();
	}


}
SchoolOS::FileSystem::Objects::Directory* getDirectory(const char *path){

	if(strlen(path) <= 1){
		if(strcmp(path,"/"))return SchoolOS::FileSystem::root;
		else return nullptr;
	}
	SchoolOS::FileSystem::Objects::FileSystemElement * CacheReturn = NameCache::get(path);
		if(*path == 0)
			return (SchoolOS::FileSystem::Objects::Directory *)CacheReturn;







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
				if(pos>=TotalSize){
					NameCache::cache(path,CurrentSubdirectorys.at(i));
					return CurrentSubdirectorys.at(i);
				}

				else continue;
			}
			i++;
		}

		// we should only end up here if the current string is not a name of one of the subdirectory of the current parentdirectory
		NameCache::cache(path,nullptr);
		return nullptr;

	}
	NameCache::cache(path,nullptr);
	return nullptr;

}

bool isDirectory(const char * path){
	return getDirectory(path)!= nullptr;
}
SchoolOS::FileSystem::Objects::File* getFile(const char *path){
	SchoolOS::FileSystem::Objects::FileSystemElement * CacheReturn = NameCache::get(path);
		if(*path == 0)
			return (SchoolOS::FileSystem::Objects::File *)CacheReturn;


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
	if(dir == nullptr){
		NameCache::cache(path,nullptr);

		return nullptr;
	}
	delete dirPath;
	char * filePath = (char *)malloc((strlen(path)-i)*sizeof(char));
	memccpy(filePath,path,(i+1)*sizeof(char),(strlen(path)-i)*sizeof(char));
	SchoolOS::FileSystem::Objects::File *file = dir->getFile(filePath);
	delete filePath;
	NameCache::cache(path,file);

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
static int m_unlink (const char *path){
	SchoolOS::FileSystem::Objects::File *file = getFile(path);
	if(file == nullptr)return -ENOENT;
	((SchoolOS::FileSystem::Objects::Directory *)file->getParent())->deleteFile(file->getName());
	NameCache::remove(path);

}
static int m_rmdir (const char *path){
	SchoolOS::FileSystem::Objects::Directory *dir = getDirectory(path);
	if(dir == nullptr)return -ENOENT;
	((SchoolOS::FileSystem::Objects::Directory *)dir->getParent())->deleteDirectory(dir->getName());
	NameCache::remove(path);

	//TODO: Networkflush
}
static int m_mkdir(const char * path,mode_t){
	SchoolOS::FileSystem::Objects::Directory *parent = getDirectory(getParentPath(path));
	if(parent == nullptr)return -ENOENT;
	std::string  name = std::string(path).substr(parent->getPath().length()-1,strlen(path)-parent->getPath().length());
	if(parent->getSubdirectory(name.c_str()) != nullptr)return -ENOENT;
	parent->addDirectory(name.c_str());
	NameCache::cache(path,parent->getSubdirectory(name.c_str()));
	return 0;
	}
static int m_rename(const char * old,const char* newname){
	SchoolOS::FileSystem::Objects::File *tmp = getFile(old);
	if(tmp == 0)return -ENOENT;
	tmp->changeName(newname);
	NameCache::reCache(newname,old);

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
		m_oper.rename 	= m_rename;
		m_oper.unlink  	= m_unlink;
		m_oper.rmdir   	= m_rmdir;
		m_oper.mkdir	= m_mkdir;
		m_oper.readdir	= m_readdir;
		m_oper.read		= m_read;
		m_oper.write	= m_write;
		m_oper.open		= m_open;
//		m_oper.flush = 	= m_flush;//TODO: flush()
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
