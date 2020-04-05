from pybuild.Module import ModuleHandle, GCC_CompilerOpts


def getSrcs(mh: ModuleHandle):
	srcs = [
		'regex.c',
		're_fail.c'	
	]
	return mh.getSrcsByPath(srcs)

def getIncs(mh: ModuleHandle):
	return mh.getAllIncsC()


def getCompilerOpts(mh: ModuleHandle):
	opts = GCC_CompilerOpts(mh.getGeneralCompilerOpts())
	opts.setWarningdOpts(['-Wno-dangling-else'])
	return opts
