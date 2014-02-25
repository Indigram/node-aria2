{
  'targets': [
    {
      'target_name': 'node_aria2',
      'sources': [
        'src/command.cc',
        'src/command/shutdown.cc',
        'src/command/config.cc',
        'src/command/add.cc',
        'src/command/start.cc',
        'src/event.cc',
        'src/event/complete.cc',
        'src/event/error.cc',
        'src/event/pause.cc',
        'src/event/progress.cc',
        'src/event/start.cc',
        'src/event/stop.cc',
        'src/download.cc',
        'src/aria2.cc',
        'src/binding.cc',
      ],
      'include_dirs': [
        'deps/aria2/src/includes',
      ],
      'dependencies': [
        'deps/aria2/aria2.gyp:aria2',
      ],
      'conditions': [
        [ 'OS=="mac"', {
          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS': ['-std=c++11', '-stdlib=libc++'],
            'MACOSX_DEPLOYMENT_TARGET': '10.8',
          },
        }],
        [ 'OS=="linux"', {
          'cflags_cc': ['-std=c++11'],
          'link_settings': {
            'libraries': [
              '-luuid',
            ],
          },
        }],
      ],
    }
  ]
}