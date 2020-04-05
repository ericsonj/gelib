import os
import subprocess
from os.path import basename
from pybuild import git
from pybuild import MKVARS


def getProjectSettings():
    return {
        'PROJECT_NAME': basename(os.getcwd()),
        'FOLDER_OUT':   'dist/Objects/'
    }


def getTargetsScript():
    PROJECT_NAME = basename(os.getcwd())
    FOLDER_OUT = 'dist/'
    TARGET = FOLDER_OUT + PROJECT_NAME

    TARGETS = {
        'TARGET': {
            'LOGKEY':  'OUT',
            'FILE':    TARGET,
            'SCRIPT':  [MKVARS.LD, '-o', '$@', MKVARS.OBJECTS, MKVARS.LDFLAGS]
        }
    }

    return TARGETS


def getCompilerSet():
    
    GCC_INCLUDES = []
    CEEDLING_INCLUDES = []
    try:
        res =  subprocess.check_output(["bash", "-c", "echo | gcc -Wp,-v -x c++ - -fsyntax-only &> /tmp/gccincs.tmp ; cat /tmp/gccincs.tmp |  grep '^[ ]*/usr.*'"])
        for line in res.splitlines():
            GCC_INCLUDES.append(line.decode('utf-8').strip())
    
        res = subprocess.check_output(['bash', '-c', "find /var/lib/gems $HOME/.gem/ -name 'unity.h' 2>&1 | grep '.*ceedling-[0-9\\.\\-]*/vendor/unity/src'"])
        for line in res.splitlines():
            CEEDLING_INCLUDES.append(os.path.dirname(line.decode('utf-8').strip()))
        
    except Exception as e:
        print(e)
    
    return {
        'CC':       'gcc',
        'CXX':      'g++',
        'LD':       'gcc',
        'AR':       'ar',
        'AS':       'as',
        'OBJCOPY':  'objcopy',
        'SIZE':     'size',
        'OBJDUMP':  'objdump',
        'INCLUDES': GCC_INCLUDES + CEEDLING_INCLUDES
    }


LIBRARIES = ['`pkg-config --libs ncurses`']


def getCompilerOpts():

    PROJECT_DEF = {
        'TESTABLE':     1
    }

    return {
        'MACROS': PROJECT_DEF,
        'MACHINE-OPTS': [
        ],
        'OPTIMIZE-OPTS': [
        ],
        'OPTIONS': [
        ],
        'DEBUGGING-OPTS': [
            '-g3'
        ],
        'PREPROCESSOR-OPTS': [
            '-MP',
            '-MMD'
        ],
        'WARNINGS-OPTS': [
        ],
        'CONTROL-C-OPTS': [
            '-std=gnu11'
        ],
        'GENERAL-OPTS': [
        ],
        'LIBRARIES': LIBRARIES
    }


def getLinkerOpts():
    return {
        'LINKER-SCRIPT': [
        ],
        'MACHINE-OPTS': [
        ],
        'GENERAL-OPTS': [
        ],
        'LINKER-OPTS': [
        ],
        'LIBRARIES': LIBRARIES
    }
