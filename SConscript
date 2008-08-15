# -*- python -*-
# $Header$
# Authors: Heather Kelly <heather@milkyway.gsfc.nasa.gov>,David Chamont <chamont@poly.in2p3.fr>
# Version: mcRootData-00-22-00

Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('mcRootDataLib', depsOnly = 1)
mcRootDataRootcint = libEnv.Rootcint('mcRootData/mcRootData_rootcint', ['mcRootData/McIntegratingHit.h',
                                                                        'mcRootData/McParticle.h', 'mcRootData/McPositionHit.h',
                                                                        'mcRootData/McEvent.h', '/mcRootData/McTrajectory.h'])

mcRootData = libEnv.SharedLibrary('mcRootData', listFiles(['src/*.cxx']) + ['mcRootData/mcRootData_rootcint.cxx'])
progEnv.Tool('mcRootDataLib')

test_mcRootData = progEnv.Program('test_mcRootData', listFiles(['src/test/*.cxx']))

progEnv.Tool('registerObjects', package = 'mcRootData', libraries = [mcRootData], testApps = [test_mcRootData], includes = listFiles(['mcRootData/*.h']))