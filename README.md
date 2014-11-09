SchoolOS
========

SchoolOS is an OpenSource Operating System completly desingend for School use. SchoolOS hat multiple Parts. The idea behind SchoolOS is rather than having an Operating System, which is able to do everthing, and forbidd things for most user you have an operating System with only fuctions everyone is allowed to use (like the windowmanager) and download everything special a user is able to do as soon as he logs in. One final thing to note is that you are not having any kind of filemanager installed files are automaticly Managed by SchoolOS. This also applys for the mobile versions. SchoolOS just complety removes the camera app. Sounds awful doesn't it? But the reasons why students are not allowed to toy with their mobile in breaks is because they can make photos of person without their willing and the whole socialnetwork things start. But if the camera app is removed you can't take photos anymore. The teacher is able to allow you to use the camera app (which install is again) and as soon as you leave school the camera app is reinstalled. But in school you can now think of allowing mobiles again. (sounds not that awful?!) 

SchoolServer
============
The SchoolServer is a Server software designed to be the controler of all SchoolOS clients.(SchoolOSX, School-IOS, SchoolAndroid) All Data of all userers are saved on the Server. Permissions are being managed and so on.


SchoolOS-Client
================
A Arch Linux based Client for SchoolOS, which is designed to run on the computers which are located in the school
  
  SchoolOS-Client-Runtime
==========================
  The Runtime, which contains the remote FileSystem and the Network
  
  SchoolOS-Client-Login
========================
  This is called after a sucsessful login by the Runbtime with the allowed packages as arguments. This binary is the         only part of SchoolOS which is called witgh root permissions ( the default user is only allowed to execute the             SchoolOS-Client-Login binary with sudo). The SchoolOS-Client-Login binary installs all the programms that are parameters   and setups the Registry (only Programm with write permissions for the Registry)
  
  SchoolOSX
============
  CANCELD FOR NOW I USE THE AWSESOME WM AS A TEMPORARY REPLACEMENT, BECAUSE I AM BEHIND SCHEDULE :(

  SchoolOSX is the Module of SchoolOS wich runs on an School Computer. It has a Linux kernel and is a Windowmanager based    on the X window System (www.x.org) and is using XCB Libary (www.freedesktop.org). The windowmanager is suspose to be       tiled   but the Tiles are resizable and saveable to Layouts to save your favorit setups. 


School-IOS
==========
Not started yet :(

SchoolAndroid
=============
Not started yet :(

Changelog:
==========
0.0.0.x
-------
Early dev version won't be dokumented look at commits if you are intersted

