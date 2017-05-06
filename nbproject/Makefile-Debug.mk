#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/cppsource/App.o \
	${OBJECTDIR}/cppsource/Arena.o \
	${OBJECTDIR}/cppsource/BaseReferer.o \
	${OBJECTDIR}/cppsource/BlobFactory.o \
	${OBJECTDIR}/cppsource/ChipHandle.o \
	${OBJECTDIR}/cppsource/ChipPlan.o \
	${OBJECTDIR}/cppsource/Controller.o \
	${OBJECTDIR}/cppsource/Cursor.o \
	${OBJECTDIR}/cppsource/Device.o \
	${OBJECTDIR}/cppsource/Model.o \
	${OBJECTDIR}/cppsource/Neuron.o \
	${OBJECTDIR}/cppsource/Pansions.o \
	${OBJECTDIR}/cppsource/PlanGrid.o \
	${OBJECTDIR}/cppsource/PlanOwned.o \
	${OBJECTDIR}/cppsource/PlanPos.o \
	${OBJECTDIR}/cppsource/PlansDirectory.o \
	${OBJECTDIR}/cppsource/Puppet.o \
	${OBJECTDIR}/cppsource/Serializer.o \
	${OBJECTDIR}/cppsource/SpawnGroups.o \
	${OBJECTDIR}/cppsource/Spawners.o \
	${OBJECTDIR}/cppsource/TextEnterer.o \
	${OBJECTDIR}/cppsource/View.o \
	${OBJECTDIR}/cppsource/ViewResources.o \
	${OBJECTDIR}/cppsource/Wirable.o \
	${OBJECTDIR}/cppsource/Wire.o \
	${OBJECTDIR}/cppsource/main.o \
	${OBJECTDIR}/external/pugixml-1.6/src/pugixml.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../../../../devlib/SFML-2.2/lib ../../../../../devlib/SFML-2.2/lib/libsfml-audio-d.a ../../../../../devlib/SFML-2.2/lib/libsfml-graphics-d.a ../../../../../devlib/SFML-2.2/lib/libsfml-window-d.a ../../../../../devlib/SFML-2.2/lib/libsfml-system-d.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/neuronal.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/neuronal.exe: ../../../../../devlib/SFML-2.2/lib/libsfml-audio-d.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/neuronal.exe: ../../../../../devlib/SFML-2.2/lib/libsfml-graphics-d.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/neuronal.exe: ../../../../../devlib/SFML-2.2/lib/libsfml-window-d.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/neuronal.exe: ../../../../../devlib/SFML-2.2/lib/libsfml-system-d.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/neuronal.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/neuronal ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/cppsource/App.o: cppsource/App.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/App.o cppsource/App.cpp

${OBJECTDIR}/cppsource/Arena.o: cppsource/Arena.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Arena.o cppsource/Arena.cpp

${OBJECTDIR}/cppsource/BaseReferer.o: cppsource/BaseReferer.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/BaseReferer.o cppsource/BaseReferer.cpp

${OBJECTDIR}/cppsource/BlobFactory.o: cppsource/BlobFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/BlobFactory.o cppsource/BlobFactory.cpp

${OBJECTDIR}/cppsource/ChipHandle.o: cppsource/ChipHandle.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/ChipHandle.o cppsource/ChipHandle.cpp

${OBJECTDIR}/cppsource/ChipPlan.o: cppsource/ChipPlan.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/ChipPlan.o cppsource/ChipPlan.cpp

${OBJECTDIR}/cppsource/Controller.o: cppsource/Controller.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Controller.o cppsource/Controller.cpp

${OBJECTDIR}/cppsource/Cursor.o: cppsource/Cursor.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Cursor.o cppsource/Cursor.cpp

${OBJECTDIR}/cppsource/Device.o: cppsource/Device.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Device.o cppsource/Device.cpp

${OBJECTDIR}/cppsource/Model.o: cppsource/Model.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Model.o cppsource/Model.cpp

${OBJECTDIR}/cppsource/Neuron.o: cppsource/Neuron.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Neuron.o cppsource/Neuron.cpp

${OBJECTDIR}/cppsource/Pansions.o: cppsource/Pansions.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Pansions.o cppsource/Pansions.cpp

${OBJECTDIR}/cppsource/PlanGrid.o: cppsource/PlanGrid.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/PlanGrid.o cppsource/PlanGrid.cpp

${OBJECTDIR}/cppsource/PlanOwned.o: cppsource/PlanOwned.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/PlanOwned.o cppsource/PlanOwned.cpp

${OBJECTDIR}/cppsource/PlanPos.o: cppsource/PlanPos.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/PlanPos.o cppsource/PlanPos.cpp

${OBJECTDIR}/cppsource/PlansDirectory.o: cppsource/PlansDirectory.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/PlansDirectory.o cppsource/PlansDirectory.cpp

${OBJECTDIR}/cppsource/Puppet.o: cppsource/Puppet.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Puppet.o cppsource/Puppet.cpp

${OBJECTDIR}/cppsource/Serializer.o: cppsource/Serializer.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Serializer.o cppsource/Serializer.cpp

${OBJECTDIR}/cppsource/SpawnGroups.o: cppsource/SpawnGroups.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/SpawnGroups.o cppsource/SpawnGroups.cpp

${OBJECTDIR}/cppsource/Spawners.o: cppsource/Spawners.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Spawners.o cppsource/Spawners.cpp

${OBJECTDIR}/cppsource/TextEnterer.o: cppsource/TextEnterer.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/TextEnterer.o cppsource/TextEnterer.cpp

${OBJECTDIR}/cppsource/View.o: cppsource/View.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/View.o cppsource/View.cpp

${OBJECTDIR}/cppsource/ViewResources.o: cppsource/ViewResources.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/ViewResources.o cppsource/ViewResources.cpp

${OBJECTDIR}/cppsource/Wirable.o: cppsource/Wirable.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Wirable.o cppsource/Wirable.cpp

${OBJECTDIR}/cppsource/Wire.o: cppsource/Wire.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/Wire.o cppsource/Wire.cpp

${OBJECTDIR}/cppsource/main.o: cppsource/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/cppsource
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cppsource/main.o cppsource/main.cpp

${OBJECTDIR}/external/pugixml-1.6/src/pugixml.o: external/pugixml-1.6/src/pugixml.cpp 
	${MKDIR} -p ${OBJECTDIR}/external/pugixml-1.6/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../../../devlib/SFML-2.2/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/external/pugixml-1.6/src/pugixml.o external/pugixml-1.6/src/pugixml.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/neuronal.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
