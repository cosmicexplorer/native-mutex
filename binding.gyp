{
  "targets": [
    {
      "target_name": "pthread-mutex",
      "sources": [ "pthread-mutex.cpp",
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
