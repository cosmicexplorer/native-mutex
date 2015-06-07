{
  "targets": [
    {
      "target_name": "native-mutex",
      "sources": [ "native-mutex.cpp",
                   "mutex.cpp"],
      "conditions": [
        [
          'OS!="win"', {
            'cflags': [
              '-std=c++14'
            ]
          }
        ]
      ]
    }
  ]
}
