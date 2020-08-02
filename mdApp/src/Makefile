TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = md
# md.dbd will be created and installed
DBD += md.dbd

# md.dbd will be made up from these files:
md_DBD += base.dbd

# Include dbd files from all support applications:
md_DBD += mddrv.dbd

# Add all the support libraries needed by this IOC
#md_LIBS += xxx

# md_registerRecordDeviceDriver.cpp derives from md.dbd
md_SRCS += md_registerRecordDeviceDriver.cpp
md_SRCS += mddrv.c

# Build the main IOC entry point on workstation OSs.
md_SRCS_DEFAULT += mdMain.cpp
md_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#md_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
md_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
