from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()

# add dhtxx src files.
src += Glob('src/dhtxx.c')

if GetDepend('PKG_USING_DHTXX_SAMPLE'):
    src += Glob('examples/dhtxx_sample.c')

if GetDepend('PKG_DHTXX_USING_SENSOR_V1'):
    src += Glob('src/asair_dhtxx_sensor_v1.c')

if GetDepend('PKG_USING_DHTXX_SAMPLE_SENSOR_V1'):
    src += Glob('examples/dhtxx_sample_sensor_v1.c')

# add dhtxx include path.
path  = [cwd + '/inc']

# add src and include to group.
group = DefineGroup('dhtxx', src, depend = ['PKG_USING_DHTXX'], CPPPATH = path)

Return('group')