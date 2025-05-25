project('v7unix', 'c')

bison_dep = dependency('bison', required : false)

if bison_dep.found()
  bison_prog = find_program('bison')
  sample_parser = custom_target(
    'sample_parser',
    input : 'tests/sample.y',
    output : 'sample.c',
    command : [bison_prog, '-o', '@OUTPUT@', '@INPUT@']
  )
endif
