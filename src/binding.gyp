{
  "targets": [
    {
      "target_name": "quick.db",
      "sources": [
        "main.cc"
      ],

      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<(module_root_dir)/include"
      ],

      "defines": [
        "NAPI_CPP_EXCEPTIONS"
      ],

      "conditions": [
        ["OS == 'win'", {
          "libraries": [
            "<(module_root_dir)/lib/sqlite3.lib"
          ], 
        }],

        ["OS == 'linux'", {
          "libraries": [
            "<(module_root_dir)/lib/libsqlite3.a"
          ]
        }]
      ],

      "msvs_settings": {
        "VCCLCompilerTool": {
          "AdditionalOptions": ["/EHsc"]
        }
      }
    }
  ]
}