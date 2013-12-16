{
  'targets': [
    {
      'target_name': 'node_aria2',
      'sources': [
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
        }],
      ],
    }
  ]
}