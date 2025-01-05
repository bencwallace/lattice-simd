gtest = dependency('gtest')
benchmark = dependency('benchmark')

executable('test2d', 'test2d.cpp', dependencies: [gtest])

executable('bench2d_scalar', 'bench2d_scalar.cpp', dependencies: [benchmark])
executable('bench2d_v64', 'bench2d_v64.cpp', dependencies: [benchmark])
executable('bench2d_v128', 'bench2d_v128.cpp', dependencies: [benchmark])
