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
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/52a4beb5/Node.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f2

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/_ext/76fcd107/NodeTests.o

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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libesgraph.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libesgraph.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libesgraph.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libesgraph.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libesgraph.a

${OBJECTDIR}/_ext/52a4beb5/Node.o: ../../../src/Node.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/52a4beb5
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../include -I../../../external/glm -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/52a4beb5/Node.o ../../../src/Node.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f2: ${TESTDIR}/_ext/76fcd107/NodeTests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS}   


${TESTDIR}/_ext/76fcd107/NodeTests.o: ../../../tests/src/NodeTests.cpp 
	${MKDIR} -p ${TESTDIR}/_ext/76fcd107
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -I../../../include -I../../../external/glm -I../../../external/googletest/googletest/include -I../../../tests/include -I../../../external/googletest/googletest -I. -MMD -MP -MF "$@.d" -o ${TESTDIR}/_ext/76fcd107/NodeTests.o ../../../tests/src/NodeTests.cpp


${OBJECTDIR}/_ext/52a4beb5/Node_nomain.o: ${OBJECTDIR}/_ext/52a4beb5/Node.o ../../../src/Node.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/52a4beb5
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/52a4beb5/Node.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -I../../../include -I../../../external/glm -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/52a4beb5/Node_nomain.o ../../../src/Node.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/52a4beb5/Node.o ${OBJECTDIR}/_ext/52a4beb5/Node_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f2 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
