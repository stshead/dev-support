#!../../bin/linux-x86_64/md

## You may have to change md to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/md.dbd"
md_registerRecordDeviceDriver pdbbase

mytest("Hello", 35)

## Load record instances
dbLoadRecords("db/mddb.db")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=epics"
