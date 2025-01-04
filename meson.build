project(
    'lattice-simd',
    'cpp',
    version: '0.1.0',
    default_options: [
        'cpp_std=c++17',
        'buildtype=release',
    ],
)
add_project_arguments('-mavx2', language: 'cpp')
subdir('src')
