{
  'targets': [
    {
      'target_name': 'object-wrap-demo-native',
      'sources': [ 'src/GameBoyAdvance.cpp', 'src/windows-helpers.cpp', "src/includes/lodepng/lodepng.cpp", "src/includes/lodepng/examples/bmp_buffer2png_buffer.cpp" ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7'
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      }
    }
  ]
}