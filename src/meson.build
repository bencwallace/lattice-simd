gtest = dependency('gtest')
benchmark = dependency('benchmark')

executable('test2d_consistency', 'test2d_consistency.cpp', dependencies: [gtest])
executable('test2d_scalar', 'test2d_scalar.cpp', dependencies: [gtest])
executable('test2d_v64', 'test2d_v64.cpp', dependencies: [gtest])
executable('test2d_v128', 'test2d_v128.cpp', dependencies: [gtest])

executable('bench2d_scalar', 'bench2d_scalar.cpp', dependencies: [benchmark])
executable('bench2d_v64', 'bench2d_v64.cpp', dependencies: [benchmark])
executable('bench2d_v128', 'bench2d_v128.cpp', dependencies: [benchmark])
