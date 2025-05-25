tests_src = files(
  'ipc_trace_concurrent.c',
  'spinlock_fairness.c',
  'spinlock_processes.c',
  'spinlock_threads.c'
)

foreach t : tests_src
  test_name = t.stem()
  exe = executable(test_name, t,
    include_directories : include_directories('../v7/usr/sys/h'),
    c_args : ['-std=c23'],
    dependencies : [dependency('threads')]
  )
  test(test_name, exe)
endforeach

